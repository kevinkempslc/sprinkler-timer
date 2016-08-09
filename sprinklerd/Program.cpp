/* 
 * File:   Program.cpp
 * Author: kkemp
 * 
 * Created on March 7, 2014, 7:26 AM
 */

#include "Program.h"
#include "ConfigFile.h"
#include "SprinklerTimer.h"
#include "Zone.h"
#include <assert.h>
#include <string>
#include <boost/signals2.hpp>
#include "Utils.h"

using namespace std;

namespace SprinklerTimeNS {

static boost::mutex programMutex;
const char* dayz[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

Program::Program(int programNumber)
{
    startTime = 0;
    running = false;
    activeZone = -1;
    programThread = NULL;
    programCompletedHandler = NULL;
    
    this->programNumber = programNumber;

    string key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Mode");
    string x = ConfigFile::getValue(key);
    programMode = (ProgramMode)atoi(x.c_str());
    
    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Interval");
    x = ConfigFile::getValue(key);
    interval = atoi(x.c_str());
    
    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Start");
    x = ConfigFile::getValue(key);
    startTime = atoi(x.c_str());
    
    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Sunday");
    x = ConfigFile::getValue(key);
    days[0] = atoi(x.c_str());

    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Monday");
    x = ConfigFile::getValue(key);
    days[1] = atoi(x.c_str());

    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Tuesday");
    x = ConfigFile::getValue(key);
    days[2] = atoi(x.c_str());

    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Wednesday");
    x = ConfigFile::getValue(key);
    days[3] = atoi(x.c_str());

    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Thursday");
    x = ConfigFile::getValue(key);
    days[4] = atoi(x.c_str());

    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Friday");
    x = ConfigFile::getValue(key);
    days[5] = atoi(x.c_str());

    key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Saturday");
    x = ConfigFile::getValue(key);
    days[6] = atoi(x.c_str());

    for (int i = 0; i < 8; i++)
    {
        key = "program";
        key.append(Utils::to_string(programNumber));
        key.append("zone");
        key.append(Utils::to_string(i + 1));
        x = ConfigFile::getValue(key);
        zones[i] = atoi(x.c_str());
    }
}

Program::~Program()
{
}

/**
 * Use the function to run in a manual mode
 */
void Program::runAsync(CycleCompleteHandler completedHandler)    // This function is only call from the main thread
{
    assert(programThread == NULL);
    
    try
    {
        // Create worker threads for the I/O.
        this->programCompletedHandler = completedHandler;
        programMutex.lock();
        running = true;
        programMutex.unlock();
        
        while (programThread != NULL)
            boost::this_thread::sleep(boost::posix_time::milliseconds(500));
            
        programThread = new boost::thread(boost::bind(programThreadFunc, this));
    }
    catch (boost::system::system_error &e)
    {
        // something went wrong...
        boost::system::error_code ec = e.code();
    }
}

/**
 Use this function when running a program from the timer loop
 */
void Program::runSync()     // This function is only called from a worker thread,
{
    programMutex.lock();
    running = true;
    programMutex.unlock();
    programThreadFunc(this);
    programMutex.lock();
    running = false;
    programMutex.unlock();
}

void Program::stop()
{
    if (running)
    {
        if (activeZone != -1)
        {
            Zone *zone = SprinklerTimer::getZone(activeZone);
            zone->stop();
        }
        programMutex.lock();
        activeZone = -1;
        running = false;
        programMutex.unlock();

        while (programThread != NULL)
            boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    }
}

void Program::setMode(const ProgramMode &mode)
{
    programMutex.lock();
    programMode = mode;
    programMutex.unlock();

    string key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Mode");
    ConfigFile::setValue(key, Utils::to_string((int)mode));
}

void Program::enableDay(unsigned int day, bool on)
{
    assert(day <= 7);

    programMutex.lock();
    days[day] = on;
    programMutex.unlock();
    
    string key = "program";
    key.append(Utils::to_string(programNumber));
    key.append(dayz[day]);
    ConfigFile::setValue(key, Utils::to_string((int)on));
}

void Program::setStartTime(unsigned int startTime) 
{ 
    programMutex.lock();
    this->startTime = startTime; 
    programMutex.unlock();

    string key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Start");
    ConfigFile::setValue(key, Utils::to_string(startTime));
}

void Program::enableZone(unsigned int zone, bool on)
{
    assert(zone < NUMZONES);

    programMutex.lock();
    zones[zone] = on;
    programMutex.unlock();
    
    string key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("zone");
    key.append(Utils::to_string(zone + 1));
    ConfigFile::setValue(key, Utils::to_string((int)on));
}

void Program::setInterval(unsigned int interval)
{
    assert(interval >= 1 && interval <= 7);
   
    programMutex.lock();
    this->interval = interval;
    programMutex.unlock();

    string key = "program";
    key.append(Utils::to_string(programNumber));
    key.append("Interval");
    ConfigFile::setValue(key, Utils::to_string(interval));
}

ProgramMode Program::getProgramMode()
{
    programMutex.lock();
    ProgramMode p = programMode;
    programMutex.unlock();
    
    return p;
}

bool Program::getDayEnabled(unsigned int day)
{
    assert(day <= 7);

    programMutex.lock();
    bool daze = days[day];
    programMutex.unlock();
    
    return daze;
}

bool Program::getZoneEnabled(unsigned int zone)
{
    assert(zone < NUMZONES);

    programMutex.lock();
    bool z = zones[zone];
    programMutex.unlock();

    return z;
}

unsigned int Program::getInterval()
{
    programMutex.lock();
    int p = interval;
    programMutex.unlock();

    return p;
}

unsigned int Program::getDuration()
{
    unsigned duration = 0;
    
    for (int zone = 0; zone < NUMZONES; zone++)
    {
        if (zones[zone])
        {
            Zone *z = SprinklerTimer::getZone(zone);
            duration += z->getDuration();
        }
    }
    
    return duration;
}

void Program::programThreadFunc(Program *program)
{
    assert(program != NULL);

    //std::cout << "Program " << Utils::to_string(program->programNumber) << " On" << std::endl;
    for (int i = 0; i < NUMZONES && program->running; i++)
    {
        programMutex.lock();
        bool zoneEnabled = program->zones[i];
        programMutex.unlock();
        if (zoneEnabled)
        {
            Zone *zone = SprinklerTimer::getZone(i);
            programMutex.lock();
            program->activeZone = i;
            programMutex.unlock();
            zone->runSync();
        }
    }
    programMutex.lock();
    program->activeZone = -1;
    programMutex.unlock();
    
    if (program->programThread)
    {
        delete program->programThread;
        program->programThread = NULL;
    }
    if (program->programCompletedHandler != NULL)
        (*program->programCompletedHandler)();
    
    program->programCompletedHandler = NULL;
}

}
