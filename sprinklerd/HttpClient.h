/* 
 * File:   HttpClient.cpp
 * Author: kkemp
 * 
 * Created on March 23, 2014, 4:06 PM
 */

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "Utils.h"

using boost::asio::ip::tcp;

class HttpClient
{
public:

    HttpResponseHandler responseHandler;
    boost::shared_ptr<std::stringstream> responseStream;
    
    HttpClient(boost::asio::io_service& io_service,
            const std::string& server, const std::string& path, HttpResponseHandler responseHandler = NULL);

private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;

    void handle_resolve(const boost::system::error_code& err,
            tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err);
    void handle_write_request(const boost::system::error_code& err);
    void handle_read_status_line(const boost::system::error_code& err);
    void handle_read_headers(const boost::system::error_code& err);
    void handle_read_content(const boost::system::error_code& err);
};

#endif  // HTTPCLIENT_H
