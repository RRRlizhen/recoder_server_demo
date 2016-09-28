#include <string>


#ifndef _USER_H
#define _USER_H


namespace MYWORK {
    struct User {
        User(const std::string &id, const std::string &role); 
        User(const User& u); 
        std::string user_id;
        std::string user_role;
    };
}

#endif
