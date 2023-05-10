#include <iostream>

#include "chat.h"
#include "sha1.h"

int main()
{
    Chat chat;
    chat.reg((char*)"user1", (char*)"1234566", sizeof("1234566"));
    chat.reg((char*)"user33", (char*)"333", sizeof("333"));
    chat.reg((char*)"user3", (char*)"qwe3", sizeof("qwe3"));
    chat.reg((char*)"new_user", (char*)"333", sizeof("333"));
    chat.reg((char*)"new_use2", (char*)"3332", sizeof("3332"));

    chat.unreg((char*)"user1");
    chat.unreg((char*)"user3");

    std::cout << (chat.login((char*)"user33", (char*)"qwe3", sizeof("333")) ? "success" : "failure") << std::endl;
    std::cout << (chat.login((char*)"user33", (char*)"333", sizeof("333")) ? "success" : "failure") << std::endl;
    std::cout << (chat.login((char*)"user1", (char*)"1234566", sizeof("1234566")) ? "success" : "failure") << std::endl;
    chat.unreg((char*)"user33");
    std::cout << (chat.login((char*)"user33", (char*)"333", sizeof("333")) ? "success" : "failure") << std::endl;
    std::cout << (chat.login((char*)"new_user", (char*)"333", sizeof("333")) ? "success" : "failure") << std::endl;
    return 0;
}
