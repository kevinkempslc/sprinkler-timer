namespace java SprinklerServer.thrift
namespace cpp SprinklerServerThrift

enum RunState 
{
    RS_AUTO = 1,
    RS_MANUAL = 2,
    RS_RAIN = 3,
    RS_OFF = 4
}

enum Day
{
    SUNDAY = 1,
    MONDAY = 2,
    TUESDAY = 3,
    WEDNESDAY = 4,
    THURSDAY = 5,
    FRIDAY = 6,
    SATURDAY = 7
}

enum ProgramMode
{
    DAY_OF_WEEK = 1,
    EVEN_DAYS = 2,
    ODD_DAYS = 3,
    DAY_INTERVAL = 4
}

struct Zone
{
    1: i32 number,
    2: i32 duration,
    3: string name,
    4: bool enabledForCurrentProgram,
}

struct BulkData
{
    1: RunState runState,
    2: bool startTimeOverlap,
    3: i32 currentProgram,
    4: i32 startHour,
    5: i32 startMin,
    6: ProgramMode programMode,
    7: i32 programInterval,
    8: list<Day> programDays,
    9: list<Zone> programZones,
}

struct UpdateData
{
    1: i32 hour,
    2: i32 minute,
    3: i32 activeProgram, // Looks like it's assignable to 0xff, but seems to represent program index?
    4: i32 activeZone, // Guessing this is similar as activeProgram; haven't had time to look up
    5: i32 manualProgram, // See above
    6: i32 manualZone, // See above
    7: RunState runState,
    8: bool startTimeOverlap,
    9: list<i32> todaysPrograms,
}

// This seems to have been largely subsumed by encoding the commands into specific
// methods in the service section.
/*enum Command
{
    RunTimer = 1,
    RunZone = 2,
    RunProgram = 3,
    SelectProgram = 4,
    SetZoneName = 5,
    SetZoneDuration = 6,
    SelectDayMode = 7,
    SelectEvenMode = 8,
    SelectOddMode = 9,
    SelectIntervalMode = 10,
    SetInterval = 11,
    SetDays = 12,
    SetZones = 13,
    SetStartTime = 14,
    SendAll = 15,
    SendUpdate = 16,
    Shutdown = 17,
    CheckSession = 18
}*/

service SprinklerService
{
    // Run the sprinkler timer
    bool runTimer(),
    // Shut down the sprinkler pi
    bool shutdown(),
    // Check the session
    bool checkSession(1:bool startSession),
    
    // Run a program manually
    bool runProgram(1:i32 programNumber),
    bool setProgramStartTime(1:i32 hours, 2:i32 minutes),
    BulkData selectProgram(),
    
    BulkData sendAll(),
    UpdateData sendUpdate(),
    
    bool selectEvenMode(),
    bool selectOddMode(),
    
    bool selectDayMode(),
    bool setDays(1:list<Day> days),
    
    bool selectIntervalMode(),
    bool setInterval(1:i64 interval),
    
    // Run a zone manually
    bool runZone(1:i32 zoneNumber),
    // Set which zones are run in the currently selected program
    bool setZones(1:list<i32> zones),
    bool setZoneName(1:i32 zoneNumber, 2:string zoneName),
    bool setZoneDuration(1:i32 zoneNumber, 2:i32 duration),
}