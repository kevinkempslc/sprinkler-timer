/* 
 * File:   Config.cpp
 * Author: kkemp
 * 
 * Created on March 18, 2014, 9:26 AM
 */

#include "ConfigFile.h"
#include <fstream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

using namespace std;

#define CONFIGFILE "/etc/sprinklerd.conf"
#define TEMPFILE "/etc/sprinklerd.conf.temp"
//#define CONFIGFILE "/home/kkemp/NetBeansProjects/sprinklerd/dist/Debug/GNU-Linux-x86/sprinklerd.conf"
//#define TEMPFILE "/home/kkemp/NetBeansProjects/sprinklerd/dist/Debug/GNU-Linux-x86/sprinklerd.conf.temp"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

std::string ConfigFile::getValue(const std::string& key, const char* defaultValue)
{
    ifstream infile(CONFIGFILE);
    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            std::string delimiter = "=";

            size_t pos = 0;
            std::string token;
            if ((pos = line.find(delimiter)) != std::string::npos) 
            {
                token = line.substr(0, pos);
                if (token == key)
                {
                    line.erase(0, pos + delimiter.length());
                    infile.close();
                    
                    return line;
                }
            }
        }
        infile.close();
    }
    
    if (defaultValue == NULL)
        return "";
    
    return defaultValue;
}

void ConfigFile::setValue(const std::string& key, const std::string& value)
{
    bool updated = false;
    ifstream infile(CONFIGFILE);

    int outfile = open(TEMPFILE, O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
    if (outfile <= 0)
    {
        switch (errno) {
          case EACCES:  printf ("Permission denied.\n"); break;
          case EMFILE:  printf ("No file handle available.\n"); break;
          case ENOENT:  printf ("File or path not found.\n"); break;
            default:      printf ("Unknown error: %d\n", errno); break;
        }
        
        infile.close();
        
        return;
    }
    if (infile.is_open()) 
    {
        string line;
        
        while (getline(infile, line))
        {
            std::string delimiter = "=";

            size_t pos = 0;
            std::string token;
            if ((pos = line.find(delimiter)) != std::string::npos) 
            {
                token = line.substr(0, pos);
                if (token != key)
                {
                    if (write(outfile, line.c_str(), line.length()) < line.length())
                        break;
                    if (write(outfile, "\n", 1) < 1)
                        break;
                }
                else
                {
                    if (write(outfile, token.c_str(), token.length()) < token.length())
                        break;
                    if (write(outfile, "=", 1) < 1)
                        break;
                    if (write(outfile, value.c_str(), value.length()) < value.length())
                        break;
                    if (write(outfile, "\n", 1) < 1)
                        break;
                    updated = true;
                }
            }
        }
        if (!updated)
        {
            write(outfile, key.c_str(), key.length());
            write(outfile, "=", 1);
            write(outfile, value.c_str(), value.length());
            write(outfile, "\n", 1);
        }
        fsync(outfile);
        infile.close();
        close(outfile);
        remove(CONFIGFILE);
        rename(TEMPFILE, CONFIGFILE);
    }
}

