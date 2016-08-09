/* 
 * File:   gpio.cpp
 * Author: kkemp
 * 
 * Created on March 23, 2014, 6:36 AM
 */

#include "GPIO.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stddef.h>

#define BUFFER_MAX 3
#define IN  0
#define OUT 1
#define LOW  0
#define HIGH 1
#define DIRECTION_MAX 35

// Available pins on RPi
// 2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27, with 28, 29, 30, 31 additionally available on the P5 header)

int GPIO::pinMap[8];

GPIO::GPIO()
{
}

GPIO::~GPIO()
{
}

void GPIO::initGPIO()
{
    pinMap[0] = 4;      // p1-07
    pinMap[1] = 22;     // p1-15
    pinMap[2] = 15;     // p1-10
    pinMap[3] = 17;     // p1-11
    pinMap[4] = 18;     // p1-12
    pinMap[5] = 23;     // p1-16
    pinMap[6] = 24;     // p1-18
    pinMap[7] = 25;     // p1-22
    
    for (int i = 0; i < 8; i++)
    {
        exportPin(i);
        setDirection(i, OUT);
        writePin(i, 1);
    }
    
    // setup gpio 2 and 3 as inputs.
//    exportPin(2);   // p1-03
//    exportPin(3);   // p1-05
//    setDirection(2, IN);
//    setDirection(3, IN);
}

void GPIO::closeGPIO()
{
    for (int i = 0; i < 8; i++)
    {
        unexportPin(i);
    }
    unexportPin(2);
    unexportPin(3);
}

int
GPIO::exportPin(int pin)
{
	char buffer[BUFFER_MAX];
    size_t bytes_written;
	int fd;
	
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		//fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pinMap[pin]);
	write(fd, buffer, bytes_written);
	close(fd);

	return(0);
}

int
GPIO::unexportPin(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		//fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pinMap[pin]);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

int
GPIO::setDirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pinMap[pin]);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		//fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		//fprintf(stderr, "Failed to set direction!\n");
        close(fd);
		return(-1);
	}

	close(fd);
	return(0);
}

int
GPIO::readPin(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pinMap[pin]);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		//fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		//fprintf(stderr, "Failed to read value!\n");
        close(fd);
        
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

int
GPIO::writePin(int pin, int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pinMap[pin]);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		//fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		//fprintf(stderr, "Failed to write value!\n");
        close(fd);
        
		return(-1);
	}

	close(fd);
	return(0);
}
