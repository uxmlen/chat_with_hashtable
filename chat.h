#pragma once

#include <string.h>

#include "sha1.h"

#define SIZE 10
#define LOGINLENGTH 10

class Chat {
public:
    Chat();
    bool login(char login[LOGINLENGTH], char* pass, int pass_length);
    void unreg(char login[LOGINLENGTH]);
    void reg(char login[LOGINLENGTH], char* pass, int pass_length);

    friend void test(Chat& c);

private:
    enum Status { free, engaged, deleted };

    struct AuthData {
        AuthData()
            : login("")
            , pass_sha1_hash(0)
            , status(Status::free)
        {
        }

        ~AuthData()
        {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }

        AuthData(char _login[LOGINLENGTH], uint* sh1)
        {
            memcpy(login, _login, LOGINLENGTH);
            pass_sha1_hash = sh1;
            status = Status::engaged;
        }

        AuthData& operator=(const AuthData& other)
        {
            memcpy(login, other.login, LOGINLENGTH);

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            status = other.status;

            return *this;
        }

        char login[LOGINLENGTH];
        uint* pass_sha1_hash;
        Status status;
    };

    int hfunc_quad(char login[LOGINLENGTH], int offset);
    int hfunc_mult(int val);
    void resize();
    void add(char login[LOGINLENGTH], uint* hash);

    AuthData* data_;
    int data_count_;
    int mem_size_;
};
