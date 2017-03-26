# sprinkler-timer

Sprinkler timer program designed to run on a Raspberry Pi, using a C++ timer service and Java/Tomcat web front-end for control.

## Requirements
* Tomcat 6 (https://tomcat.apache.org/download-60.cgi)
* Apache Thrift 0.9.3 (https://thrift.apache.org/)
* Java (currently configured to use 8)

## Getting started

On the Raspberry Pi

1. Install boost using `sudo apt-get install libboost1.55-all`
2. Create a link to the tomcat directory and name it /home/pi/tomcat.
3. Create a link to the daemon directory and call it /home/pi/timer.
4. Copy the rc.local and sprinklerd.conf files in the source directory to the /etc directory on the Pi.
5. Notes on network setup...
    1. On the router, reserve an IP address for the Pi.
    2. Leave the Pi setup for using DHCP.
    3. Configure the router to port forward port 8080 to the Pi.
6. Install Tomcat. This is easy, just download tomcat to the raspberry pi. You can put the SprinklerTimer.war file in the tomcat webapps directory and tomcat will automatically deploy it. You also need to some roles and a user to `[tomcat folder]/conf/tomcat-users.xml`. In the `<tomcat-users>` element, add... 
    1. `<role rolename="manager-gui"/><user username="admin" password="mypassword" roles="manager-gui"/>`
    2. Replace "mypassword" with your own.
    3. To run tomcat, execute `[tomcat folder]/bin/startup.sh`.
    4. To stop tomcat, execute `[tomcat folder]/bin/shutdown.sh`.
7. To install thrift:
    1. Copy the thrift tarball to the pi and untar
    2. Install required packages (see https://thrift.apache.org/docs/install/debian)
        1. Run the following command
                `# sudo apt-get install automake bison flex g++ git libboost1.55-all-dev libevent-dev libssl-dev libtool make pkg-config`
        2. I ignored the optional packages
    3. Build thrift (see https://thrift.apache.org/docs/BuildingFromSource)
        1. thrift-0.9.3 did not appear to have a boostrap.sh script, so I skipped that.
        2. Run `./configure`. Didn't need anything else, I don't think
        3. Run make
            1. Ran into some problems with the make process in the tests; it choked on some things. After being unable to make it work for some time, I moved on and the make install seemed to work fine.
            2. Run `make install`. Should put the thrift libraries in `/usr/local/lib`
    4. Run `$ sudo ldconfig`. Updates the system with the newly installed libraries
8. Build the thrift libraries for Java
    1. Navigate to `<thrift extraction directory>/lib/java`
    2. If you have ant, run ant.
    3. If you're using IntelliJ community edition:
        1. Open IntelliJ
        2. File->New->Project from Existing Sources...
        3. Navigate to the thrift java directory from a.
        4. Select "Create project from existing sources" radio button
        5. Name it whatever you want (I went with ThriftJava)
        6. After project creation, go to View->Tool Windows->Ant build
        7. Click the + button and add the build.xml file in the thrift directory
        8. Hit the play button to run the build
9. To compile the code in sprinklerd, run `make CONF=Release` from the sprinklerd directory on the pi. The debug makefile is not yet working with Thrift. Modifications to the makefile for Thrift were made in nbproject/Makefile-Release.mk.
