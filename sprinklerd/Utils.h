/* 
 * File:   Utils.h
 * Author: kkemp
 *
 * Created on March 22, 2014, 10:57 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#define DAEMON_NAME "sprinklerd"


#include <boost/shared_ptr.hpp>
#include <string>

typedef void (*CycleCompleteHandler)();
typedef void (*HttpResponseHandler)(boost::shared_ptr<std::stringstream> response);

class Utils
{
public:
    Utils();
    Utils(const Utils& orig);
    virtual ~Utils();


    static std::string to_string(int x);
private:

};

#endif	/* UTILS_H */

