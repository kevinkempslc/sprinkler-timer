/* 
 * File:   SendMail.cpp
 * Author: kkemp
 * 
 * Created on April 22, 2014, 7:29 PM
 */
#include "SendMail.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <assert.h>

#include <iostream>

/* This is a simple example showing how to send mail using libcurl's SMTP
 * capabilities. It builds on the smtp-mail.c example to add authentication
 * and, more importantly, transport security to protect the authentication
 * details from being snooped.
 *
 * Note that this example requires libcurl 7.20.0 or above.
 */

static const char *payload_text_template[] = {
    "Date: %s\r\n",
    "To: %s\r\n",
    "From: %s(Sprinkler Timer)\r\n",
    "Cc: %s\r\n",
    "Subject: Sprinkler System: %s\r\n",
    "\r\n", /* empty line to divide headers from body, see RFC5322 */
    "%s\r\n",
    "\r\n",
};

static char payload_text[8][100];

SendMail::SendMail(std::string username, std::string password, std::string url, std::string toEmail, std::string fromEmail, std::string ccEmail) :
    username(username),
    password(password),
    url(url),
    toEmail(toEmail),
    fromEmail(fromEmail),
    ccEmail(ccEmail)
{
}

SendMail::SendMail(const SendMail& orig) :
    username(orig.username),
    password(orig.password),
    url(orig.url),
    toEmail(orig.toEmail),
    fromEmail(orig.fromEmail),
    ccEmail(orig.ccEmail)
{
}

SendMail::~SendMail()
{
}

struct upload_status
{
    int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *) userp;
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1) || upload_ctx->lines_read == 8)
    {
        return 0;
    }

    data = (const char*) payload_text[upload_ctx->lines_read];

    if (data != 0)
    {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}

int SendMail::send(const char* subject, const char *msg)
{
//#if 0

    // These parameters are critical; if they weren't given to us, we can't proceed.
    if(username.empty() || password.empty() || toEmail.empty() || url.empty())
    {
        std::cout << "One or more critical email parameters (emailUsername, emailPassword, emailURL, emailTo) are missing from the config file; unable to send email. "
                  << "subject: " << subject << "message: " << msg << std::endl;
        return -1;
    }

    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;
    char date[80];

    time_t now = time(0);
    tm* localtm = localtime(&now);
    strftime (date, 80,"%a, %d %b %Y %T %z",localtm);

    // Ideally we wouldn't be hardcoding the individual text template indices here, but
    // right now we're just trying to get it to work. We'll make an improvement pass later.
    int payloadTextIndex = 0;
    sprintf(payload_text[payloadTextIndex++], (const char*) payload_text_template[0], date);
    sprintf(payload_text[payloadTextIndex++], payload_text_template[1], toEmail.c_str());
    sprintf(payload_text[payloadTextIndex++], payload_text_template[2], fromEmail.c_str());
    // payloadTextIndex should now be 3
    if(!ccEmail.empty())
        sprintf(payload_text[payloadTextIndex++], payload_text_template[3], ccEmail.c_str());
    // If we were given a ccEmail, payloadTextIndex will now be 4; if not, 3.
    sprintf(payload_text[payloadTextIndex++], payload_text_template[4], subject);
    strcpy(payload_text[payloadTextIndex++], payload_text_template[5]);
    sprintf(payload_text[payloadTextIndex++], payload_text_template[6], msg);
    strcpy(payload_text[payloadTextIndex++], payload_text_template[7]);
    upload_ctx.lines_read = 0;

    curl = curl_easy_init();
    if (curl)
    {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

        /* This is the URL for your mailserver. Note the use of port 587 here,
         * instead of the normal SMTP port (25). Port 587 is commonly used for
         * secure mail submission (see RFC4403), but you should use whatever
         * matches your server configuration. */
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        /* In this example, we'll start with a plain text connection, and upgrade
         * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
         * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
         * will continue anyway - see the security discussion in the libcurl
         * tutorial for more details. */
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long) CURLUSESSL_ALL);

        /* If your server doesn't have a valid certificate, then you can disable
         * part of the Transport Layer Security protection by setting the
         * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
         * That is, in general, a bad idea. It is still better than sending your
         * authentication details in plain text though.
         * Instead, you should get the issuer certificate (or the host certificate
         * if the certificate is self-signed) and add it to the set of certificates
         * that are known to libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See
         * docs/SSLCERTS for more information. */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        //    curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

        /* Note that this option isn't strictly required, omitting it will result in
         * libcurl sending the MAIL FROM command with empty sender data. All
         * autoresponses should have an empty reverse-path, and should be directed
         * to the address in the reverse-path which triggered them. Otherwise, they
         * could cause an endless loop. See RFC 5321 Section 4.5.5 for more details.
         */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmail.c_str());

        /* Add two recipients, in this particular case they correspond to the
         * To: and Cc: addressees in the header, but they could be any kind of
         * recipient. */
        recipients = curl_slist_append(recipients, toEmail.c_str());

        // Append the CC email if we were given one
        if(!ccEmail.empty())
            recipients = curl_slist_append(recipients, ccEmail.c_str());

        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* We're using a callback function to specify the payload (the headers and
         * body of the message). You could just use the CURLOPT_READDATA option to
         * specify a FILE pointer to read from. */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Since the traffic will be encrypted, it is very useful to turn on debug
         * information within libcurl to see what is happening during the transfer.
         */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* Free the list of recipients */
        curl_slist_free_all(recipients);

        /* Always cleanup */
        curl_easy_cleanup(curl);
    }

    return (int) res;
//#endif
    
//    return 0;
}