/* 
 * File:   Zone.cpp
 * Author: kkemp
 * 
 * Created on March 15, 2014, 4:51 AM
 */

#include "Zone.h"
#include "ConfigFile.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <assert.h>
#include "Utils.h"
#include "GPIO.h"

using namespace std;

namespace SprinklerTimeNS
{
static boost::mutex zoneMutex;

Zone::Zone(unsigned int zoneNumber)
{
    running = false;
    zoneThread = NULL;
    completedHandler = NULL;
    this->zoneNumber = zoneNumber;

    std::string key("zone");
    key.append(Utils::to_string(zoneNumber));
    key.append("name");
    this->name = ConfigFile::getValue(key);

    key = "zone";
    key.append(Utils::to_string(zoneNumber));
    key.append("duration");
    string d = ConfigFile::getValue(key);
    this->duration = atoi(d.c_str());
}

Zone::Zone(const Zone& orig)
{
    if (&orig != this)
    {
        this->duration = orig.duration;
        this->name = orig.name;
        this->zoneNumber = orig.zoneNumber;
    }
}

Zone::~Zone()
{
}

/**
 * Use the function to run in a manual zone mode
 */
void Zone::runAsync(CycleCompleteHandler completedHandler)
{
    assert(zoneThread == NULL);

    if (duration > 0)
    {
        try
        {
            // Create worker threads for the I/O.
            this->completedHandler = completedHandler;
            
            while (zoneThread != NULL)
                boost::this_thread::sleep(boost::posix_time::milliseconds(500));

            zoneThread = new boost::thread(boost::bind(zoneThreadFunc, this));
            zoneMutex.lock();
            running = true;
            zoneMutex.unlock();
        }
        catch (boost::system::system_error &e)
        {
            // something went wrong...

            boost::system::error_code ec = e.code();
        }
    }
}

/***
 Use this function when running via a program
 */
void Zone::runSync()
{
    zoneThreadFunc(this);
}

void Zone::stop()
{
    zoneMutex.lock();
    running = false;
    zoneMutex.unlock();
    turnRelayOff();
    
    while (zoneThread != NULL)
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
}

void Zone::setName(const std::string& name) 
{ 
    this->name = name; 

    string key = "zone";
    key.append(Utils::to_string(zoneNumber));
    key.append("name");
    ConfigFile::setValue(key, name);
}

void Zone::setDuration(unsigned char duration) 
{ 
    zoneMutex.lock();
    this->duration = duration; 
    zoneMutex.unlock();

    string key = "zone";
    key.append(Utils::to_string(zoneNumber));
    key.append("duration");
    ConfigFile::setValue(key, Utils::to_string((int)duration));
}

void Zone::zoneThreadFunc(Zone *zone)
{
    assert(zone != NULL);

    zone->turnRelayOn();
    
    boost::asio::io_service io;
    zoneMutex.lock();
    zone->running = true;
    zoneMutex.unlock();
    
    int counter = zone->duration * 60;
    
    while (zone->running && counter > 0)
    {
        // Wait 1 second before going on
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        counter--;
    }
    zoneMutex.lock();
    zone->running = false;
    zoneMutex.unlock();
     zone->turnRelayOff();
    if (zone->completedHandler != NULL)
    {
        (*zone->completedHandler)();
        zone->completedHandler = NULL;
    }
     
    if (zone->zoneThread)
    {
        delete  zone->zoneThread;
        zone->zoneThread = NULL;
    }
}

void Zone::turnRelayOn()
{
        GPIO::writePin(zoneNumber - 1, 0);
}

void Zone::turnRelayOff()
{
        GPIO::writePin(zoneNumber - 1, 1);
}

}

