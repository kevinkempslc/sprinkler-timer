/* 
 * File:   SprinklerTimer.cpp
 * Author: kkemp
 * 
 * Created on March 7, 2014, 9:49 AM
 */

#include "SprinklerTimer.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/container/vector.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <stdio.h>

#include <string>
#include <time.h>
#include <syslog.h>
#include <ctime>

#include "Utils.h"
#include "GPIO.h"
#include "ConfigFile.h"

namespace SprinklerTimeNS
{

    Zone* SprinklerTimer::zones[NUMZONES];
    int SprinklerTimer::manualProgram = -1;
    int SprinklerTimer::manualZone = -1;
    int SprinklerTimer::activeProgram = -1;
    bool SprinklerTimer::dateSet = false;
    int  SprinklerTimer::loginTime = 0;
    RunState SprinklerTimer::runState = RS_OFF;
    RunState SprinklerTimer::savedRunState = RS_OFF;
    SendMail *SprinklerTimer::sm = NULL;

    boost::asio::io_service SprinklerTimer::io_service;
    
    static boost::mutex timerMutex;

    SprinklerTimer::SprinklerTimer()
    : timerThread(NULL), getTimeThread(NULL), currentProgram(0)
    {
        std::string emailUsername = ConfigFile::getValue("emailUsername", "");
        std::string emailPassword = ConfigFile::getValue("emailPassword", "");
        std::string emailURL = ConfigFile::getValue("emailURL", "");
        std::string emailTo = ConfigFile::getValue("emailTo", "");
        std::string emailFrom = ConfigFile::getValue("emailFrom", "");
        std::string emailCC = ConfigFile::getValue("emailCC", "");

        sm = new SendMail(emailUsername, emailPassword, emailURL, emailTo, emailFrom, emailCC);
        
        // Create the Program objects
        for (unsigned int i = 0; i < NUMPROGRAMS; i++)
            programs[i] = new Program(i + 1);

        // create the Zone objects
        for (unsigned int i = 0; i < NUMZONES; i++)
        {
            zones[i] = new Zone(i + 1);
        }
        // Configure the GPIOs
        GPIO::initGPIO();

        // Restore the run state to what it previously was.
        if (ConfigFile::getValue("running", "no").compare("yes") == 0)
        {
            timerMutex.lock();
            runState = RS_AUTO;
            timerMutex.unlock();
        }
        try
        {
            // Create worker thread for the timer.
            timerThread = new boost::thread(boost::bind(timerThreadFunc, this));
        }
        catch (boost::system::system_error &e)
        {
            // something went wrong...

            boost::system::error_code ec = e.code();
        }
        
        sendStartupEMail();
    }

    SprinklerTimer::~SprinklerTimer()
    {
        // Clean up
        cancelAll();

        timerThread->interrupt();
        delete timerThread;

        for (int i = 0; i < NUMZONES; i++)
        {
            delete zones[i];
        }
        for (unsigned int i = 0; i < NUMPROGRAMS; i++)
        {
            delete programs[i];
        }
        GPIO::closeGPIO();
    }

    bool SprinklerTimer::checkSession(unsigned char begin)
    {
        // Get the current time
        time_t theTime;
        theTime = time((time_t*) 0);
        struct tm *tminfo = localtime(&theTime);

        int currentTime = tminfo->tm_hour * 60 + tminfo->tm_min;
                
        if (begin != 0)
        {
            loginTime = currentTime;
        }
        else
        {
            if (currentTime - loginTime > 30)
            {
                loginTime = 0;
                return false;
            }
            else
            {
                loginTime = currentTime;
            }
        }
        
        return true;
    }
    
    void SprinklerTimer::sendStartupEMail()
    {
        std::string msg = "Sprinkler system started.\r\nRun Mode: ";
        switch (runState)
        {
            case RS_OFF:
                msg += "Off";
                break;
            case RS_AUTO:
                msg += "Running";
                break;
            case RS_RAIN:
                msg += "Rain";
                break;
            case RS_MANUAL:
                msg += "Manual";
                break;
        }
        sm->send("INFO", msg.c_str());
    }
    
    /**
     * Return the Zone object for the given zone index.
     * @param zoneIndex
     * @return 
     */
    Zone* SprinklerTimer::getZone(unsigned int zoneIndex)
    {
        return zones[zoneIndex];
    }

    /**
     * Toggles the timer on and off. 
     * @return nothing
     */
    void SprinklerTimer::startStopProgramTimer()
    {
        timerMutex.lock();
        RunState rs = runState;
        timerMutex.unlock();

        if (rs == RS_MANUAL)
            return;

        switch (rs)
        {
            case RS_OFF:
                timerMutex.lock();
                runState = RS_AUTO;
                timerMutex.unlock();

                ConfigFile::setValue("running", "yes");

                break;
            
            case RS_AUTO:
                // Timer was already running, turn it off.
                cancelAll();
                timerMutex.lock();
                runState = RS_RAIN;
                timerMutex.unlock();
                ConfigFile::setValue("running", "no");
                break;
                break;

            case RS_RAIN:
                // Timer was already running, turn it off.
                cancelAll();
                ConfigFile::setValue("running", "no");
                break;
        }
    }

    /**
     * Run a zone manually
     * @param zone
     * @return 
     */
    bool SprinklerTimer::runZoneManually(unsigned int zone)
    {
        assert(zone < NUMZONES);

        timerMutex.lock();
        RunState rs = runState;
        timerMutex.unlock();

        if (rs == RS_MANUAL && manualZone == zone)
        {
            // Stop the zone.
            cancelAll();
            assert(savedRunState != RS_MANUAL);
            timerMutex.lock();
            runState = savedRunState;
            timerMutex.unlock();

            return false;
        }
        else
        {
            // Stop everything (program timer or manual zones or manual programs)
            cancelAll();

            // Run the zone asynchronously. Assign a completion handler to
            // reset the manual state.
            timerMutex.lock();
            runState = RS_MANUAL;
            manualZone = zone;
            if (rs != RS_MANUAL)
                savedRunState = rs;
            timerMutex.unlock();
            zones[zone]->runAsync((CycleCompleteHandler) zoneCompleteHandler);

            return true;
        }

    }

    /**
     * Runs a program manually
     * @param program
     * @return 
     */
    bool SprinklerTimer::runProgramManually(unsigned int program)
    {
        assert(program < NUMPROGRAMS);

        timerMutex.lock();
        RunState rs = runState;
        timerMutex.unlock();

        if (rs == RS_MANUAL && program == manualProgram)
        {
            // Stop the program
            cancelAll();
            timerMutex.lock();
            runState = savedRunState;
            timerMutex.unlock();

            return false;
        }
        else
        {
            // Stop everything (program timer or manual zones or manual programs)
            cancelAll();

            // Run the program asynchronously. Assign a completion handler to
            // reset the manual state.
            programs[program]->runAsync((CycleCompleteHandler) programCompleteHandler);
            timerMutex.lock();
            activeProgram = manualProgram = program;
            runState = RS_MANUAL;
            if (rs != RS_MANUAL)
                savedRunState = rs;
            timerMutex.unlock();

            return true;
        }

    }

    /**
     * Select the next program. This determines which program is being edited.
     * @return 
     */
    ::SprinklerServerThrift::BulkData SprinklerTimer::nextProgram()
    {
        if (++currentProgram == NUMPROGRAMS)
            currentProgram = 0;

        return sendAll();
    }

    /**
     * Assign a name to a zone.
     * @param zone
     * @param name
     * @return 
     */
    bool SprinklerTimer::setZoneName(unsigned int zone, const std::string& name)
    {
        assert(zone < NUMZONES);

        zones[zone]->setName(name);

        std::string key("zone");
        key.append(Utils::to_string(zone + 1));
        key.append("name");
        ConfigFile::setValue(key, name);

        return true;
    }

    /**
     * Specify the zones run duration in minutes
     * @param zone
     * @param duration
     * @return 
     */
    bool SprinklerTimer::setZoneDuration(unsigned int zone, unsigned char duration)
    {
        assert(zone < NUMZONES);

        zones[zone]->setDuration(duration);

        return true;
    }

    /**
     * Sets a program's run mode (which days to run)
     * @param mode
     * @return 
     */
    bool SprinklerTimer::setProgramMode(ProgramMode mode)
    {
        assert(currentProgram < NUMPROGRAMS);

        programs[currentProgram]->setMode(mode);

        return true;
    }

    /**
     * Set frequency of days to run program (every day, ever other, ever third etc.)
     * @param interval
     * @return 
     */
    bool SprinklerTimer::setProgramInterval(unsigned int interval)
    {
        assert(interval >= 1 && interval <= 7);

        programs[currentProgram]->setInterval(interval);

        return true;
    }

    /**
     * Sets with days of the week to run. Single bits the days byte specify the days.
     * Sunday is bit 0, Monday is bit 1 etc.
     * @param days
     * @return 
     */
    bool SprinklerTimer::setProgramDays(const std::vector< ::SprinklerServerThrift::Day::type> &days)
    {
        // Disable all days, then enable the ones we've been sent.
        // Not very efficient, but the best I can think of at the moment (tired)
        // std::vector isn't very nice to work with.
        for(int i = 0; i < 7; i++)
        {
            programs[currentProgram]->enableDay(i, false);
        }

        for(int i = 0; i < days.size(); i++)
        {
            // The Day enum is 1-based, so subtract one to convert to 0-based
            programs[currentProgram]->enableDay(days[i] - 1, true);
        }

        return true;
    }

    /**
     * Sets with zones are run in the current program. Bit 0 is program 1 and so on.
     * @param zones
     * @return 
     */
    bool SprinklerTimer::setProgramZones(const std::vector<int32_t> & zones)
    {
        // Disable all zones, then enable the ones we've been sent
        for(int i = 0; i < NUMZONES; i++)
        {
            programs[currentProgram]->enableZone(i, false);
        }

        for(int i = 0; i < zones.size(); i++)
        {
            programs[currentProgram]->enableZone(zones[i], true);
        }

        return true;
    }

    /**
     * Sets the start time for a program.
     * @param hours
     * @param minutes
     * @return 
     */
    bool SprinklerTimer::setProgramStartTime(unsigned int hours, unsigned int minutes)
    {
        assert(hours <= 23);
        assert(minutes < 60);

        programs[currentProgram]->setStartTime(hours * 60 + minutes);

        return !checkForOverlap();
    }

    /**
     * Check of program times overlap.
     * @return 
     */
    bool SprinklerTimer::checkForOverlap()
    {
        unsigned int ranges[4][2];

        for (int i = 0; i < 4; i++)
        {
            ranges[i][0] = programs[i]->getStartTime();
            ranges[i][1] = ranges[i][0] + programs[i]->getDuration();
        }

        for (int i = 0; i < 3; i++)
        {
            // ignore zone j if start time is 0
            if (ranges[i][0] == 0)
                continue;

            // ignore zone i if duration is 0
            if (ranges[i][0] == ranges[i][1])
                continue;

            for (int j = i + 1; j < 4; j++)
            {
                if (i == j)
                    continue;

                // Check if programs are on same day. If not, then obviously no overlap.
                if (programs[i]->getProgramMode() == EvenDays &&
                        programs[j]->getProgramMode() == OddDays)
                    continue;
                if (programs[j]->getProgramMode() == EvenDays &&
                        programs[i]->getProgramMode() == OddDays)
                    continue;
                
                // ignore zone j if start time is 0
                if (ranges[j][0] == 0)
                    continue;

                // ignore zone j if duration is 0
                if (ranges[j][0] == ranges[j][1])
                    continue;

                if (ranges[j][0] >= ranges[i][0] && ranges[j][0] < ranges[i][1])
                    return true;
                if (ranges[j][1] >= ranges[i][0] && ranges[j][1] <= ranges[i][1])
                    return true;
                if (ranges[j][0] <= ranges[i][0] && ranges[j][1] >= ranges[i][1])
                    return true;
            }
        }

        return false;
    }

    /**
     * Cancels the run timer, or any zone or program that is running.
     * Always call this before starting another one.
     */
    void SprinklerTimer::cancelAll()
    {
        // Reset all state variables.
        timerMutex.lock();
        runState = RS_OFF;
        activeProgram = -1;
        manualZone = -1;
        manualProgram = -1;
        timerMutex.unlock();

        // Stop all programs
        for (int i = 0; i < NUMPROGRAMS; i++)
            programs[i]->stop();

        // Stop all zones
        for (int i = 0; i < NUMZONES; i++)
        {
            zones[i]->stop();
        }
    }

    /**
     * Returns an update to the client.
     * @return 
     */
    ::SprinklerServerThrift::UpdateData SprinklerTimer::sendUpdate()
    {
        ::SprinklerServerThrift::UpdateData updateData;
        time_t theTime;
        theTime = time((time_t*) 0);
        struct tm *tminfo = localtime(&theTime);

        if (loginTime != 0)
            checkSession(1);
        
        updateData.hour = tminfo->tm_hour;
        updateData.minute = tminfo->tm_min;

        for (int i = 0; i < NUMPROGRAMS; i++)
        {
            if (todaysPrograms[i])
            {
                updateData.todaysPrograms.push_back(i);
            }
        }

        RunState localRunState;
        timerMutex.lock();
        localRunState = runState;
        updateData.startTimeOverlap = checkForOverlap();
        updateData.activeProgram = activeProgram;
        timerMutex.unlock();

        updateData.runState = SprinklerTimer::translateRunStateToThrift(localRunState);

        if (updateData.activeProgram != -1)
            updateData.activeZone = programs[updateData.activeProgram]->getActiveZone();
        else
            updateData.activeZone = -1;

        timerMutex.lock();
        updateData.manualProgram = manualProgram;
        updateData.manualZone = manualZone;
        timerMutex.unlock();

        return updateData;
    }

    /**
     * Returns all data to the client
     * @return 
     */
    ::SprinklerServerThrift::BulkData SprinklerTimer::sendAll()
    {
        ::SprinklerServerThrift::BulkData bulkData;
        bulkData.currentProgram = currentProgram;

        RunState localRunState;
        timerMutex.lock();
        localRunState = runState;
        timerMutex.unlock();

        bulkData.runState = SprinklerTimer::translateRunStateToThrift(localRunState);
        bulkData.startTimeOverlap = checkForOverlap();
        bulkData.programMode = SprinklerTimer::translateProgramModeToThrift(programs[currentProgram]->getProgramMode());
        bulkData.startHour = programs[currentProgram]->getStartTime() / 60;
        bulkData.startMin = programs[currentProgram]->getStartTime() % 60;

        // Translate the integer-representation of days (0-based) in the Program
        // class to the Thrift Enum-based representation of days (1-based)
        for(int i = 0; i < 7; i++)
        {
            if(programs[currentProgram]->getDayEnabled(i))
                bulkData.programDays.push_back(static_cast< ::SprinklerServerThrift::Day::type>(i + 1));
        }

        for (int i = 0; i < NUMZONES; i++)
        {
            bool enabledForProgram = false;
            if (programs[currentProgram]->getZoneEnabled(i))
                enabledForProgram = true;

            ::SprinklerServerThrift::Zone zone;
            zone.number = i;
            zone.duration = getZone(i)->getDuration();
            zone.name = getZone(i)->getName();
            zone.enabledForCurrentProgram = enabledForProgram;
            
            bulkData.programZones.push_back(zone);
        }

        bulkData.programInterval = programs[currentProgram]->getInterval();

        updateTimeOfDay();

        return bulkData;
    }

    /**
     * Checks if there is any zones currently turned on.
     * @return 
     */
    bool SprinklerTimer::isZoneOrProgramRunning()
    {
        for (int i = 0; i < NUMZONES; i++)
        {
            if (zones[i] && getZone(i)->isRunning())
                return true;
        }

        return false;
    }

    /**
     * This is the program timer thread function
     * @param timer
     */
    void SprinklerTimer::timerThreadFunc(SprinklerTimer *timer)
    {
        bool didTimeUpdate = false;
        
        assert(timer != NULL);

        while (1) // loop forever
        {
            try
            {
                timerMutex.lock();
                RunState rs = timer->runState;
                timerMutex.unlock();

                // Get the current time
                time_t theTime;
                theTime = time((time_t*) 0);
                struct tm *tminfo = localtime(&theTime);

                int currentTime = tminfo->tm_hour * 60 + tminfo->tm_min;
                
                // Update the time every night at midnight.
                if (currentTime == 0 && !didTimeUpdate)
                {
                    didTimeUpdate = true;
                    dateSet = false;
                    timer->updateTimeOfDay();

                    if (rs == RS_RAIN)
                    {
                        timer->runState = RS_AUTO;
                        rs = RS_AUTO;
                    }

                    if (rs == RS_OFF)
                        sm->send("NOTICE", "This is a reminder that the sprinklers are turned off.");
                    
                    if (timer->checkForOverlap())
                        sm->send("NOTICE", "There is an overlap in the timer programs.");
                }
                if (currentTime != 0)
                    didTimeUpdate = false;

                if (rs == RS_AUTO)
                {
                    // Skip if a program is already running
                    if (!timer->isZoneOrProgramRunning())
                    {
                        // Check each program to see if one of them is ready to run
                        for (int i = 0; i < NUMPROGRAMS; i++)
                        {
                            timer->todaysPrograms[i] = 0;   

                            // See if the program should be run today.
                            switch (timer->programs[i]->getProgramMode())
                            {
                                case DayOfWeek:
                                    if (!timer->programs[i]->getDayEnabled(tminfo->tm_wday))
                                        continue;
                                    break;
                                case EvenDays:
                                    if ((tminfo->tm_wday % 2) != 0)
                                        continue;
                                    break;
                                case OddDays:
                                    if ((tminfo->tm_wday % 2) == 0)
                                        continue;
                                    break;
                                case DayInterval:
                                    int x = timer->programs[i]->getInterval();
                                    if ((tminfo->tm_yday % x) != 0)
                                        continue;
                            }
                            timer->todaysPrograms[i] = 1;

                            // Is it time to start?
                            if ((timer->programs[i]->getStartTime() == currentTime) && (currentTime != 0))
                            {
                                char buf[100];
                                
                                // Run the program...
                                timerMutex.lock();
                                timer->activeProgram = i;
                                timerMutex.unlock();
                                timer->programs[i]->runSync();
                                timerMutex.lock();
                                timer->activeProgram = -1;
                                timerMutex.unlock();
                            }
                        }
                    }
                }
                // Wait 15 seconds before going on
                boost::this_thread::sleep(boost::posix_time::milliseconds(15000));
            }
            catch (boost::system::system_error &e)
            {
            }
        }
    }

    /**
     * Callback function for zone completed signal
     */
    void SprinklerTimer::zoneCompleteHandler()
    {
        timerMutex.lock();
        manualZone = -1;
        runState = savedRunState;
        timerMutex.unlock();
        
        assert(runState != RS_MANUAL);
    }

    /**
     * Callback function for program completed signal
     */
    void SprinklerTimer::programCompleteHandler()
    {
        timerMutex.lock();
        activeProgram = manualProgram = -1;
        runState = savedRunState;
        timerMutex.unlock();

        assert(runState != RS_MANUAL);
    }

    ::SprinklerServerThrift::RunState::type SprinklerTimer::translateRunStateToThrift(SprinklerTimeNS::RunState runState)
    {
        if(runState == SprinklerTimeNS::RS_AUTO)
            return ::SprinklerServerThrift::RunState::RS_AUTO;
        else if(runState == SprinklerTimeNS::RS_MANUAL)
            return ::SprinklerServerThrift::RunState::RS_MANUAL;
        else if(runState == SprinklerTimeNS::RS_RAIN)
            return ::SprinklerServerThrift::RunState::RS_RAIN;
        else
            return ::SprinklerServerThrift::RunState::RS_OFF;
    }
    
    ::SprinklerServerThrift::ProgramMode::type SprinklerTimer::translateProgramModeToThrift(SprinklerTimeNS::ProgramMode programMode)
    {
        if(programMode == SprinklerTimeNS::DayOfWeek)
            return ::SprinklerServerThrift::ProgramMode::DAY_OF_WEEK;
        else if(programMode == SprinklerTimeNS::EvenDays)
            return ::SprinklerServerThrift::ProgramMode::EVEN_DAYS;
        else if(programMode == SprinklerTimeNS::OddDays)
            return ::SprinklerServerThrift::ProgramMode::ODD_DAYS;
        else if(programMode == SprinklerTimeNS::DayInterval)
            return ::SprinklerServerThrift::ProgramMode::DAY_INTERVAL;
    
        return ::SprinklerServerThrift::ProgramMode::DAY_OF_WEEK;
    }

    void SprinklerTimer::updateTimeOfDay()
    {
        if (getTimeThread == NULL)
        {
            try
            {
                // Create worker thread for the timer.
                getTimeThread = new boost::thread(boost::bind(getTimeThreadFunc, this));
            }
            catch (boost::system::system_error &e)
            {
                // something went wrong...

                boost::system::error_code ec = e.code();
            }
        }
    }

    // This function provides a thread to asynchronously request the time of day
    // from a web service.
    void SprinklerTimer::getTimeThreadFunc(SprinklerTimer *timer)
    {
        // using www.earthtools.org/timezone/40.75/-111.88

        // TODO: get lat/lon from config file. The could go on the System page.
        
        // Request the time of day information from a web service.
        HttpClient getclient (
                io_service,
                "new.earthtools.org",
                "/timezone/40.75/-111.88",
                timeResponseHandler);

	io_service.run();
	//timer->io_service.run();

        // Give the service 10 seconds to respond.
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
        
        // reset the io service so it can be resused.
        //timer->io_service.reset();
        
        // Delete the thread object.
        //delete timer->getTimeThread;
        //timer->getTimeThread = NULL;
    }

    /**
     * Handles the HTTP request for the time of day web service.
     * @param response: contains and xml string from the web service.
     */
    void SprinklerTimer::timeResponseHandler(boost::shared_ptr<std::stringstream> response)
    {
#if 0        
        std::string line;
        
        // parse the string line by line to find the localtime element.
        while (std::getline(*response, line))
        {
            std::size_t index = line.find("<localtime>");
            if (index != std::string::npos)
            {
                // found "<localtime>, extract the inner text
                index += 11;
                std::size_t endindex = line.find("</localtime>");
                std::string localtm = line.substr(index, endindex - index);

                // Parse the time string into a tm struct
                struct tm tm;
                strptime(localtm.c_str(), "%d %b %Y %T", &tm);

                // TODO: get DST option from config file. The could go on the System page.
                tm.tm_isdst = 1;
                // Add an hour for DST
                if (tm.tm_hour == 23)
                    tm.tm_hour = 0;
                else
                    tm.tm_hour++;

                // prepare a timeval struct to use with settimeofday()
                time_t t = mktime(&tm);
                struct timeval tv;

                tv.tv_sec = t;
                tv.tv_usec = 0;

                // Now set the time of day. Program must be running in the
                // root account to work.
                    dateSet = true;
                int result = settimeofday(&tv, NULL);
                if (result == 0)
                    dateSet = true;
                break;
            }
        }
#endif
    }
}
