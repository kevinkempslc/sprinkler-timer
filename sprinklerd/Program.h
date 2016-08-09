/* 
 * File:   Program.h
 * Author: kkemp
 *
 * Created on March 7, 2014, 7:26 AM
 */

#ifndef PROGRAM_H
#define	PROGRAM_H

#include "Zone.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>

#define NUMZONES 8

namespace SprinklerTimeNS
{
    
    
enum ProgramMode
{
    DayOfWeek,
    EvenDays,
    OddDays,
    DayInterval
};

class Program
{
public:
    Program(int programNumber);
    virtual ~Program();
    
    void setMode(const ProgramMode &mode);
    void enableDay(unsigned int day, bool on);
    void enableZone(unsigned int zone, bool on);
    void setInterval(unsigned int interval);
    ProgramMode getProgramMode();
    bool getDayEnabled(unsigned int day);
    bool getZoneEnabled(unsigned int zone);
    unsigned int getInterval();
    void setStartTime(unsigned int startTime);
    inline unsigned int getStartTime() { return startTime; }
    unsigned int getDuration();
    void runAsync(CycleCompleteHandler completedHandler);
    void runSync();
    void stop();
    inline bool isRunning() { return running; }
    inline unsigned char getActiveZone() { return activeZone; }
    
private:
    Program(const Program& orig);

    ProgramMode programMode;
    boost::thread *programThread;
    static void programThreadFunc(Program *program);
    CycleCompleteHandler programCompletedHandler;
    bool days[7];
    bool zones[NUMZONES];
    bool running;
    int programNumber;
    unsigned int startTime;
    unsigned int interval;
    int activeZone;
};
}
#endif	/* PROGRAM_H */

