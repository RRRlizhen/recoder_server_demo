#include <iostream>
#include "User.h"

namespace MYWORK {
    User::User(const std::string &id, const std::string &role) {
            this->user_id = id;
            this->user_role = role;
        }
    User::User(const User& u) {
        this->user_id = u.user_id;
        this->user_role = u.user_role;
    }
}
