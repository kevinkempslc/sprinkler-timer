/*
 * File:   SprinklerServiceHandler.h
 * Author: jadams
 *
 * Created on Jul 29, 2014, 8:37 PM
 */

#ifndef SPRINKLERSERVICEHANDLER_H
#define SPRINKLERSERVICEHANDLER_H

#include "SprinklerTimer.h"
#include "SprinklerService.h"

using namespace  ::SprinklerServerThrift;

namespace SprinklerTimeNS
{

class SprinklerServiceHandler : virtual public SprinklerServiceIf
{
public:
    SprinklerServiceHandler(SprinklerTimer *timer);

    bool runTimer();
    bool shutdown();
    bool checkSession(const bool startSession);
    bool runProgram(const int32_t programNumber);
    bool setProgramStartTime(const int32_t hours, const int32_t minutes);
    void selectProgram(::SprinklerServerThrift::BulkData& _return);
    void sendAll(::SprinklerServerThrift::BulkData& _return);
    void sendUpdate(::SprinklerServerThrift::UpdateData& _return);
    bool selectEvenMode();
    bool selectOddMode();
    bool selectDayMode();
    bool setDays(const std::vector<Day::type> & days);
    bool selectIntervalMode();
    bool setInterval(const int64_t interval);
    bool runZone(const int32_t zoneNumber);
    bool setZones(const std::vector<int32_t> & zones);
    bool setZoneName(const int32_t zoneNumber, const std::string& zoneName);
    bool setZoneDuration(const int32_t zoneNumber, const int32_t duration);

    static ::SprinklerServerThrift::RunState::type translateRunStateToThrift(SprinklerTimeNS::RunState runState);
    static ::SprinklerServerThrift::ProgramMode::type translateProgramModeToThrift(SprinklerTimeNS::ProgramMode programMode);

private:

    SprinklerTimer *sprinklerTimer;

};

} // namespace SprinklerTimeNS

#endif /* SPRINKLERSERVICEHANDLER_H */
