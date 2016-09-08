/* 
 * File:   SendMail.h
 * Author: kkemp
 *
 * Created on April 22, 2014, 7:29 PM
 */

#ifndef SENDMAIL_H
#define	SENDMAIL_H

#include <stdlib.h>
#include <signal.h>

#include <string>

class SendMail {
public:
    SendMail(std::string username, std::string password, std::string url, std::string toEmail, std::string fromEmail, std::string ccEmail);
    SendMail(const SendMail& orig);
    virtual ~SendMail();
    
    int send(const char* subject, const char *msg);
    
private:
    std::string username;
    std::string password;
    std::string url;
    std::string toEmail;
    std::string fromEmail;
    std::string ccEmail;
};

#endif	/* SENDMAIL_H */

