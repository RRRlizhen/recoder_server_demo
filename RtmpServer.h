#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#ifndef _RTMPSERVER_H
#define _RTMPSERVER_H

#include "librtmp/rtmp.h"
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"
#include "ParseConf.h"
#include "User.h"

namespace MYWORK {
    class RtmpServer {
        public:
            RtmpServer(const std::string &addr, const std::string &save = "./tmp/", const std::string &media = "", const std::string &room = "", const std::string &pre = "_", User *user = NULL);
            RtmpServer(const ParseConf& pc, const std::string &pre);

            virtual ~RtmpServer(); 
                
            void set_save_dir(const std::string &save);
            void set_room_id(const std::string &room);
            void set_user(const User &user);
            void set_user(const std::string &user_id, const std::string &user_role);
            void set_media_id(const std::string &media);
            void set_rtmp_url(const std::string &addr);
            bool record();    

        private:
            std::string rtmp_url;
            std::string save_dir;
            std::string room_id;
            std::string prefix;
            User* user;
            std::string media_id;
    }; 
}


#endif
