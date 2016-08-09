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

/* This is a simple example showing how to send mail using libcurl's SMTP
 * capabilities. It builds on the smtp-mail.c example to add authentication
 * and, more importantly, transport security to protect the authentication
 * details from being snooped.
 *
 * Note that this example requires libcurl 7.20.0 or above.
 */

#define FROM    "<from@example.com>"
#define TO      "<to@example.com>"
//#define CC      "<cc@example.com>"

static const char *payload_text_template[] = {
    "Date: %s\r\n",
    "To: " TO "\r\n",
    "From: " FROM "(Sprinkler Timer)\r\n",
    /*"Cc: " CC "(CC Person)\r\n",*/
    "Subject: Sprinkler System: %s\r\n",
    "\r\n", /* empty line to divide headers from body, see RFC5322 */
    "%s\r\n",
    "\r\n",
};

static char payload_text[8][100];

SendMail::SendMail()
{
}

SendMail::SendMail(const SendMail& orig)
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
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;
    char date[80];

    time_t now = time(0);
    tm* localtm = localtime(&now);
    strftime (date, 80,"%a, %d %b %Y %T %z",localtm);
    sprintf(payload_text[0], (const char*) payload_text_template[0], date);
    // NOTE: This count limit for i needs to be adjusted according to how many
    // elements are in payload_text_template.
    for (int i = 1; i <= 4; i++)
        strcpy(payload_text[i], payload_text_template[i]);
    sprintf(payload_text[4], payload_text_template[4], subject);
    sprintf(payload_text[6], payload_text_template[6], msg);
    upload_ctx.lines_read = 0;

    curl = curl_easy_init();
    if (curl)
    {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, "username");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dummy");

        /* This is the URL for your mailserver. Note the use of port 587 here,
         * instead of the normal SMTP port (25). Port 587 is commonly used for
         * secure mail submission (see RFC4403), but you should use whatever
         * matches your server configuration. */
        curl_easy_setopt(curl, CURLOPT_URL, "hostname");

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
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);

        /* Add two recipients, in this particular case they correspond to the
         * To: and Cc: addressees in the header, but they could be any kind of
         * recipient. */
        recipients = curl_slist_append(recipients, TO);
        //recipients = curl_slist_append(recipients, CC);
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