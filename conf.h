#ifndef _CONF_H
#define _CONF_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <pthread.h>
#include <boost/bind.hpp>

#include "json/json.h"
#include "ParseConf.h"
#include "RtmpServer.h"
#include "ThreadPool.h"
#include "record_server.h"


#define PRE_CHATROOM_USERID "_"
#define RTMP_URL "rtmp://54.222.199.175:7776/flvplayback/"
#define POOL_SIZE 10000 


#endif

