#include "SprinklerTimer.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "Utils.h"
#include "thrift/SprinklerService.h"
#include "thrift/SprinklerServiceHandler.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/shared_ptr.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::SprinklerServerThrift;

using namespace std;

int main(int argc, char *argv[]) 
{
    //Set our Logging Mask and open the Log
//    setlogmask(LOG_UPTO(LOG_NOTICE));
//    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
//
//    syslog(LOG_INFO, "Entering Daemon");
//    closelog ();

// TODO: Change this to if 1 when ready to daemonize.
#if 0
    
    pid_t pid, sid;

   //Fork the Parent Process
    pid = fork();

    if (pid < 0) 
    { 
        exit(EXIT_FAILURE); 
    }

    //We got a good pid, Close the Parent Process
    if (pid > 0) 
    { 
        exit(EXIT_SUCCESS); 
    }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) 
    { 
        exit(EXIT_FAILURE); 
    }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) 
    { 
        exit(EXIT_FAILURE); 
    }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
#endif
    //----------------
    //Main Process
    //----------------

    printf("Constructing server...\n");
    SprinklerTimeNS::SprinklerTimer *timer = new SprinklerTimeNS::SprinklerTimer();
    
    int port = 9100;
    shared_ptr<SprinklerTimeNS::SprinklerServiceHandler> handler(new SprinklerTimeNS::SprinklerServiceHandler(timer));
    shared_ptr<TProcessor> processor(new SprinklerServiceProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    printf("Starting the server...\n");
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    printf("Done.\n");

    return 0;
}
