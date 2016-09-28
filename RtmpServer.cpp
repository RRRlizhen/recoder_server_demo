#include <iostream>
#include "RtmpServer.h"


namespace MYWORK {
    RtmpServer::RtmpServer(const std::string &addr, const std::string &save, const std::string &media, const std::string &room, const std::string &pre, User *user) : rtmp_url(addr), save_dir(save), media_id(media), room_id(room), prefix(pre), user(user) {

    }

    RtmpServer::RtmpServer(const ParseConf& pc, const std::string &pre) {
        this->rtmp_url = pc.get_rtmp_url();    
        this->save_dir = pc.get_save_dir();
        this->room_id = pc.get_room_id();
        this->prefix = pre;
        this->user = new User(pc.get_user_id(), pc.get_user_role());
        this->media_id = pc.get_media_id();
    }

    bool RtmpServer::record() {
        const char* save_dir_local = const_cast<char *>(this->save_dir.c_str());
        if (NULL == opendir(save_dir_local)) {
            int status;
            status = mkdir(save_dir_local, S_IRWXU | S_IRWXG | S_IRWXG | S_IROTH | S_IXOTH);
            if (0 != status) {
                RTMP_LogPrintf("Created Save Dir Error.\n");
                return false;
            }
        } 

        std::string file_prefix("chatroom_");
        std::string file_path("");
        std::string addr;
        if (0 == media_id.compare("")) {
            if(0 == room_id.compare("") || 0 == user->user_id.compare("") || 0 == user->user_role.compare("")) {
                RTMP_LogPrintf("room_id or user_id paramater Error. \n");
                return false;
            }
            file_path = save_dir + "/" + file_prefix + room_id + prefix + user->user_id + prefix + user->user_role + ".flv";
            addr = rtmp_url + room_id + prefix + user->user_id;
        } else {
            file_path = save_dir + "/" + file_prefix + media_id + prefix + user->user_role + ".flv";
            addr = rtmp_url + media_id;
        } 
        FILE *fp = fopen(file_path.c_str(), "wb+");
        if (!fp) {
            RTMP_LogPrintf("Open File Error.\n");
            return false;
        }
        int bufsize = 1024 * 1024 * 10;
        char *buf = (char *)malloc(bufsize);
        try {
            memset(buf, 0, bufsize);
        } catch (std::exception& e) {
            RTMP_LogPrintf("Memset Error.\n");
            return false;
        }
        RTMP *rtmp = RTMP_Alloc();
        if (NULL == rtmp) {
            RTMP_LogPrintf("RTMP Init Error.\n");
            return false;
        }
        RTMP_Init(rtmp);
        rtmp->Link.timeout = 10;
        rtmp->Link.lFlags |= RTMP_LF_LIVE;
        
        if (!RTMP_SetupURL(rtmp, const_cast<char *>(addr.c_str()))) {
            RTMP_Log(RTMP_LOGERROR, "Setup URL Error\n");
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
            return false;
        }
        
        RTMP_SetBufferMS(rtmp, 3600 * 1000);

        if (!RTMP_Connect(rtmp, NULL)) {
            RTMP_Log(RTMP_LOGERROR, "Connect Error\n");
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
            return false;
        }

        if(!RTMP_ConnectStream(rtmp, 0)) {
            RTMP_Log(RTMP_LOGERROR, "Connect Stream Error\n");
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
            return false;
        }
        int nRead = 0;
        nRead = RTMP_Read(rtmp, buf, bufsize);
        while (nRead) {
            //houqi bu zhen
            fwrite(buf, 1, nRead, fp);    
            nRead = RTMP_Read(rtmp, buf, bufsize);
            RTMP_LogPrintf("Received %5d Bytes \n", nRead);
        }

        if(fp) {
            fclose(fp);
        }
        if(buf) {
            free(buf);
        } 
        if(rtmp) {
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
            rtmp = NULL;
        }
        return true;
    }


    void RtmpServer::set_save_dir(const std::string &save) {
        this->save_dir = save;
    }
    void RtmpServer::set_room_id(const std::string &room) {
        this->room_id = room; 
    }
    void RtmpServer::set_user(const std::string &user_id, const std::string &user_role) {
        this->user = new User(user_id, user_role);
    }
    void RtmpServer::set_user(const User& user) {
        this->user = new User(user);
    }
    void RtmpServer::set_media_id(const std::string &media) {
        this->media_id = media;
    }
    void RtmpServer::set_rtmp_url(const std::string &addr) {
        this->rtmp_url = addr;
    }

    RtmpServer::~RtmpServer() {
        delete user; 
    }
}
