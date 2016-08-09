#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ConfigFile.o \
    ${OBJECTDIR}/GPIO.o \
	${OBJECTDIR}/HttpClient.o \
	${OBJECTDIR}/Program.o \
	${OBJECTDIR}/SendMail.o \
	${OBJECTDIR}/SprinklerTimer.o \
	${OBJECTDIR}/Utils.o \
	${OBJECTDIR}/Zone.o \
    ${OBJECTDIR}/SprinklerService.o \
    ${OBJECTDIR}/SprinklerServiceHandler.o \
    ${OBJECTDIR}/SprinklerServer_constants.o \
    ${OBJECTDIR}/SprinklerServer_types.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lcurl -lboost_system -lboost_thread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sprinklerd

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sprinklerd: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sprinklerd ${OBJECTFILES} ${LDLIBSOPTIONS} -pthread -L/usr/lib/arm-linux-gnueabihf/ -L/usr/local/lib -lthrift -lboost_thread -lboost_system -s

${OBJECTDIR}/ConfigFile.o: ConfigFile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ConfigFile.o ConfigFile.cpp

${OBJECTDIR}/GPIO.o: GPIO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GPIO.o GPIO.cpp

${OBJECTDIR}/HttpClient.o: HttpClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HttpClient.o HttpClient.cpp

${OBJECTDIR}/Program.o: Program.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Program.o Program.cpp

${OBJECTDIR}/SendMail.o: SendMail.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SendMail.o SendMail.cpp

${OBJECTDIR}/SprinklerTimer.o: SprinklerTimer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SprinklerTimer.o SprinklerTimer.cpp

${OBJECTDIR}/Utils.o: Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Utils.o Utils.cpp

${OBJECTDIR}/Zone.o: Zone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Zone.o Zone.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/include/boost -I./ -Ithrift -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/SprinklerService.o: thrift/SprinklerService.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/local/include/thrift -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SprinklerService.o thrift/SprinklerService.cpp

${OBJECTDIR}/SprinklerServiceHandler.o: thrift/SprinklerServiceHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/local/include/thrift -I./ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SprinklerServiceHandler.o thrift/SprinklerServiceHandler.cpp

${OBJECTDIR}/SprinklerServer_constants.o: thrift/SprinklerServer_constants.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/local/include/thrift -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SprinklerServer_constants.o thrift/SprinklerServer_constants.cpp

${OBJECTDIR}/SprinklerServer_types.o: thrift/SprinklerServer_types.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -s -I/usr/local/include/thrift -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SprinklerServer_types.o thrift/SprinklerServer_types.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sprinklerd

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
