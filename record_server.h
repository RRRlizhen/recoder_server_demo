#ifndef _RECORD_SERVER_H
#define _RECORD_SERVER_H

#include  "conf.h"

namespace MYWORK {
    typedef websocketpp::server<websocketpp::config::asio> server;
    // pull out the type of mesent by our config
    typedef server::message_ptr message_ptr;
    
    struct parameter {
        std::string rtmp_url;
        Json::Value clientData;
        parameter(std::string rtmp_url, Json::Value clientData) {
            this->rtmp_url = rtmp_url;
            this->clientData = clientData;
        }
    };
    
}

#endif
