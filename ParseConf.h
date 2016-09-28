#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#ifndef _RECORD_H
#define _RECORD_H

#include "json/json.h" 


namespace MYWORK{
    class ParseConf {
        public:
            ParseConf(const std::string &rtmp_url, const Json::Value &clientData); 
            std::string get_type() const;    
            std::string get_media_id() const;
            std::string get_save_dir() const;
            std::string get_room_id() const;
            std::string get_user_id() const;
            std::string get_user_role() const;
            std::string get_rtmp_url() const;
            virtual ~ParseConf(void) {
            }

        private:
            ParseConf& operator=(const ParseConf &);
            ParseConf(const ParseConf &);

        private:
            std::string type;
            std::string media_id;
            std::string save_dir;
            std::string room_id;
            std::string user_id;
            std::string user_role;
            std::string rtmp_url;
    };
}

#endif
