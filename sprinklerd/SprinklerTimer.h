/* 
 * File:   SprinklerTimer.h
 * Author: kkemp
 *
 * Created on March 7, 2014, 9:49 AM
 */
#ifndef SPRINKLERTIMER_H
#define	SPRINKLERTIMER_H

#include "Program.h"
#include "HttpClient.h"
#include "SendMail.h"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include "thrift/SprinklerServer_types.h"

namespace SprinklerTimeNS 
{
const static int NUMPROGRAMS = 4;
    

enum RunState 
{
    RS_AUTO,
    RS_MANUAL,
    RS_RAIN,
    RS_OFF
};

struct BulkData
{
    unsigned char runState;                 // 0
    unsigned char startTimeOverlap;         // 1
    unsigned char currentProgram;           // 2
    unsigned char startHour;                // 3
    unsigned char startMin;                 // 4
    unsigned char programMode;              // 5
    unsigned char programInterval;          // 6
    unsigned char programDays;              // 7
    unsigned char programZones;             // 8
    unsigned char zoneDuration[NUMZONES];   // 9
    unsigned char zoneName[NUMZONES][21];   // 17
};

struct UpdateData
{
    unsigned char hour;
    unsigned char minute;
    unsigned char activeProgram;
    unsigned char activeZone;
    unsigned char manualProgram;
    unsigned char manualZone;
    unsigned char runState;
    unsigned char startTimeOverlap;
    unsigned char todaysPrograms;
};

class SprinklerTimer
{
public:
    SprinklerTimer();
    virtual ~SprinklerTimer();

    void startStopProgramTimer();
    bool runZoneManually(unsigned int zone);
    bool runProgramManually(unsigned int program);
    ::SprinklerServerThrift::BulkData nextProgram();
    bool setZoneName(unsigned int zone, const std::string& name);
    bool setZoneDuration(unsigned int zone, unsigned char duration);
    bool setProgramMode(ProgramMode mode);
    bool setProgramInterval(unsigned int interval);
    bool setProgramDays(const std::vector< ::SprinklerServerThrift::Day::type> &days);
    bool setProgramZones(const std::vector<int32_t> & zones);
    bool setProgramStartTime(unsigned int hours, unsigned int minutes);
    bool checkSession(unsigned char begin);
    static Zone* getZone(unsigned int zoneIndex);
    ::SprinklerServerThrift::BulkData sendAll();
    ::SprinklerServerThrift::UpdateData sendUpdate();
    static Zone* zones[];
    
private:
    static ::SprinklerServerThrift::RunState::type translateRunStateToThrift(SprinklerTimeNS::RunState runState);
    static ::SprinklerServerThrift::ProgramMode::type translateProgramModeToThrift(SprinklerTimeNS::ProgramMode programMode);

    void updateTimeOfDay();
    void sendStartupEMail();
    Program* programs[4];
    //::SprinklerServerThrift::BulkData bulkData;
    //::SprinklerServerThrift::UpdateData updateData;
    boost::thread *timerThread;
    boost::thread *getTimeThread;
    unsigned int currentProgram;
    static SendMail *sm;
    static RunState runState;
    static RunState savedRunState;
    static int activeProgram;
    static int manualProgram;
    static int manualZone;
    static bool dateSet;
    static boost::asio::io_service io_service;
    static int loginTime;
    unsigned char todaysPrograms[NUMPROGRAMS];
    

    static void timeResponseHandler(boost::shared_ptr<std::stringstream> response);
    
    SprinklerTimer(const SprinklerTimer& orig);
    void cancelAll();
    bool isZoneOrProgramRunning();
    static void timerThreadFunc(SprinklerTimer *controller);
    static void getTimeThreadFunc(SprinklerTimer *controller);
    static void httpTimeout(SprinklerTimer *timer);
    
    static void programCompleteHandler();
    static void zoneCompleteHandler();
    bool checkForOverlap();
};

}

#endif	/* SPRINKLERTIMER_H */

