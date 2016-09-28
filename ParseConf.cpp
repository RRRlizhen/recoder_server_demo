#include <iostream>
#include "ParseConf.h"


//using namespace std;

namespace MYWORK {
    ParseConf::ParseConf(const std::string &rtmp_url, const Json::Value &clientData) {
        this->rtmp_url = rtmp_url;
        try {
            type = clientData["type"].asString();
        } catch (std::exception& e) {
            fprintf(stderr, "type parse error %s \n", e.what());
        }
        try {
            media_id = clientData["media_id"].asString();
        } catch (std::exception& e) {
            fprintf(stderr, "media_id parse error %s \n", e.what());
        }
        try {
            save_dir = clientData["save_dir"].asString();
        } catch (std::exception& e) {
            fprintf(stderr, "save_dir parse error %s \n", e.what());
        } 
        try {
            room_id = clientData["room_id"].asString();
        } catch (std::exception& e) {
            fprintf(stderr, "room_id parse error %s \n", e.what());    
        }

        try {
            user_id = clientData["user_id"].asString();
        } catch (std::exception& e) {
            fprintf(stderr, "user_id parse error %s \n", e.what());
        }
        
        try {
            user_role = clientData["user_role"].asString(); 
        } catch (std::exception& e) {
            fprintf(stderr, "user_role parse error %s \n", e.what());
        }

        if(0 == save_dir.compare("")) {
            save_dir = "./tmp";
        }
    }

    std::string ParseConf::get_type() const {
        return type;
     }

    std::string ParseConf::get_media_id() const {
        return media_id; 
    }

    std::string ParseConf::get_save_dir() const {
        return save_dir;
    }

    std::string ParseConf::get_room_id() const {
        return room_id;
    }

    std::string ParseConf::get_user_id() const {
        return user_id;
    }

    std::string ParseConf::get_user_role() const {
        return user_role;
    }

    std::string ParseConf::get_rtmp_url() const {
        return rtmp_url;
    }

}
