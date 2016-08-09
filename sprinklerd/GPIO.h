/* 
 * File:   gpio.h
 * Author: kkemp
 *
 * Created on March 23, 2014, 6:36 AM
 */

#ifndef GPIO_H
#define	GPIO_H

class GPIO
{
public:
    GPIO();
    virtual ~GPIO();
    static void initGPIO();
    static void closeGPIO();
    static int exportPin(int pin);
    static int unexportPin(int pin);
    static int setDirection(int pin, int dir);
    static int readPin(int pin);
    static int writePin(int pin, int value);
    
private:
    GPIO(const GPIO& orig);

    static int pinMap[8];
};

#endif	/* GPIO_H */

