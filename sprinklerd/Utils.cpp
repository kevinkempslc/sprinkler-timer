/* 
 * File:   Utils.cpp
 * Author: kkemp
 * 
 * Created on March 22, 2014, 10:57 AM
 */

#include "Utils.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

Utils::Utils()
{
}

Utils::Utils(const Utils& orig)
{
}

Utils::~Utils()
{
}

std::string Utils::to_string(int x)
{
   char buffer[33];
   
   sprintf(buffer, "%d", x);
   
   return std::string(buffer);
}

