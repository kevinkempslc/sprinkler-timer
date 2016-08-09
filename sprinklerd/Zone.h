/* 
 * File:   Zone.h
 * Author: kkemp
 *
 * Created on March 15, 2014, 4:51 AM
 */

#ifndef ZONE_H
#define	ZONE_H

#include <string>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "Utils.h"

namespace SprinklerTimeNS
{

class Zone
{
public:
    Zone(unsigned int zoneNumber);
    Zone(const Zone& orig);
    virtual ~Zone();
    
    inline unsigned char getDuration() { return duration; }
    inline const std::string& getName() { return name; }
    void setName(const std::string& name);
    void setDuration(unsigned char duration);
    void runAsync(CycleCompleteHandler completedHandler);
    void runSync();
    void stop();
    inline bool isRunning() { return running; }
    
private:
    std::string name;
    unsigned char duration;
    unsigned int zoneNumber;
    bool running;
    boost::thread *zoneThread;
    CycleCompleteHandler completedHandler;
    static void zoneThreadFunc(Zone *zone);
    
    void turnRelayOn();
    void turnRelayOff();
};

}

#endif	/* ZONE_H */

