/* 
 * File:   Config.h
 * Author: kkemp
 *
 * Created on March 18, 2014, 9:26 AM
 */

#ifndef CONFIGFILE_H
#define	CONFIGFILE_H

#include <string>

class ConfigFile
{
public:
    ConfigFile();
    virtual ~ConfigFile();
    
    static std::string getValue(const std::string& key, const char* defaultValue = NULL);
    static void setValue(const std::string& key, const std::string& value);
private:
    ConfigFile(const ConfigFile& orig);

};

#endif	/* CONFIGFILE_H */
