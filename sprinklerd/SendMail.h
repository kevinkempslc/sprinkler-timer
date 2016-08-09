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

class SendMail {
public:
    SendMail();
    SendMail(const SendMail& orig);
    virtual ~SendMail();
    
    int send(const char* subject, const char *msg);
    
private:
};

#endif	/* SENDMAIL_H */

