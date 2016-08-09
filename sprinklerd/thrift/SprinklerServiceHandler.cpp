/*
 * File:   SprinklerServiceHandler.cpp
 * Author: jadams
 *
 * Created on Jul 29, 2014, 8:41 PM
 */

#include "SprinklerServiceHandler.h"

using namespace  ::SprinklerServerThrift;

namespace SprinklerTimeNS
{

SprinklerServiceHandler::SprinklerServiceHandler(SprinklerTimer *timer)
{
    sprinklerTimer = timer;
}

bool SprinklerServiceHandler::runTimer()
{
    if(sprinklerTimer->checkSession(false))
    {
        sprinklerTimer->startStopProgramTimer();
        return true;
    }

    return false;
}

bool SprinklerServiceHandler::shutdown()
{
    system("shutdown -h now");
    return true;
}

bool SprinklerServiceHandler::checkSession(const bool startSession)
{
    return sprinklerTimer->checkSession(startSession);
}

bool SprinklerServiceHandler::runProgram(const int32_t programNumber)
{
    if(sprinklerTimer->checkSession(false))
        return sprinklerTimer->runProgramManually(programNumber);

    return false;
}

bool SprinklerServiceHandler::setProgramStartTime(const int32_t hours, const int32_t minutes)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramStartTime(hours, minutes);
    }

    return false;
}

void SprinklerServiceHandler::selectProgram(::SprinklerServerThrift::BulkData& _return)
{
    if(sprinklerTimer->checkSession(false))
    {
        _return = sprinklerTimer->nextProgram();
    }
}

void SprinklerServiceHandler::sendAll(::SprinklerServerThrift::BulkData& _return)
{
    if(sprinklerTimer->checkSession(false))
    {
        _return = sprinklerTimer->sendAll();
    }
}

void SprinklerServiceHandler::sendUpdate(::SprinklerServerThrift::UpdateData& _return)
{
    if(sprinklerTimer->checkSession(false))
    {
        _return = sprinklerTimer->sendUpdate();
    }
}

bool SprinklerServiceHandler::selectEvenMode()
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramMode(EvenDays);
    }

    return false;
}

bool SprinklerServiceHandler::selectOddMode()
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramMode(OddDays);
    }

    return false;
}

bool SprinklerServiceHandler::selectDayMode()
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramMode(DayOfWeek);
    }

    return false;
}

bool SprinklerServiceHandler::setDays(const std::vector<Day::type> & days)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramDays(days);
    }

    return false;
}

bool SprinklerServiceHandler::selectIntervalMode()
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramMode(DayInterval);
    }

    return false;
}

bool SprinklerServiceHandler::setInterval(const int64_t interval)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramInterval(interval);
    }

    return false;
}

bool SprinklerServiceHandler::runZone(const int32_t zoneNumber)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->runZoneManually(zoneNumber);
    }

    return false;
}

bool SprinklerServiceHandler::setZones(const std::vector<int32_t> & zones)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setProgramZones(zones);
    }

    return false;
}

bool SprinklerServiceHandler::setZoneName(const int32_t zoneNumber, const std::string& zoneName)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setZoneName(zoneNumber, zoneName);
    }

    return false;
}

bool SprinklerServiceHandler::setZoneDuration(const int32_t zoneNumber, const int32_t duration)
{
    if(sprinklerTimer->checkSession(false))
    {
        return sprinklerTimer->setZoneDuration(zoneNumber, duration);
    }

    return false;
}

::SprinklerServerThrift::RunState::type SprinklerServiceHandler::translateRunStateToThrift(SprinklerTimeNS::RunState runState)
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

::SprinklerServerThrift::ProgramMode::type SprinklerServiceHandler::translateProgramModeToThrift(SprinklerTimeNS::ProgramMode programMode)
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

} // namespace SprinklerTimeNS
