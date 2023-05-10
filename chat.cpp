#include <cstring>
#include <iostream>

#include "chat.h"

Chat::Chat()
{
    data_count_ = 0;
    data_ = nullptr;
    mem_size_ = 8;
    data_ = new AuthData[mem_size_];
}

void Chat::reg(char login[LOGINLENGTH], char* pass, int pass_length)
{
    uint* hash = sha1(pass, pass_length);
    add(login, hash);
}

void Chat::unreg(char login[LOGINLENGTH])
{

    int index, i = 0;
    for (; i < mem_size_; i++) {
        index = hfunc_quad(login, i);
        if (data_[index].status == Status::free)
            return;
        else if (data_[index].status == Status::engaged
            && !memcmp(login, data_[index].login, LOGINLENGTH))
            break;
    }
    if (i >= mem_size_) {
        return;
    }

    data_[index].status = Status::deleted;
}

bool Chat::login(char login[LOGINLENGTH], char* pass, int pass_length)
{

    int index = 0;
    int i = 0;

    for (; i < mem_size_; ++i) {
        index = hfunc_quad(login, i);
        if (data_[index].status == Status::free)
            return false;
        else if (data_[index].status == Status::engaged
            && !memcmp(login, data_[index].login, LOGINLENGTH))
            break;
    }

    if (i >= mem_size_)
        return false;

    uint* hash = sha1(pass, pass_length);

    bool result
        = !memcmp(data_[index].pass_sha1_hash, hash, SHA1HASHLENGTHBYTES);
    delete[] hash;

    return result;
}

int Chat::hfunc_quad(char login[LOGINLENGTH], int offset)
{
    long sum = 0;
    for (int i = 0; i < LOGINLENGTH; i++) {
        sum += login[i];
    }

    return (hfunc_mult(sum) + offset * offset) % mem_size_;
}

int Chat::hfunc_mult(int val)
{
    const double A = 0.7;
    return int(mem_size_ * (A * val - int(A * val)));
}

void Chat::resize()
{

    AuthData* save_data = data_;
    int old_size = mem_size_;

    mem_size_ *= 2;
    data_count_ = 0;
    data_ = new AuthData[mem_size_];
    for (int i = 0; i < mem_size_; ++i) {
        data_[i] = AuthData();
    }

    for (int i = 0; i < old_size; i++) {
        AuthData& old_data = save_data[i];
        if (old_data.status == Status::engaged) {
            uint* sha_hash_copy = new uint[SHA1HASHLENGTHUINTS];
            memcpy(sha_hash_copy, old_data.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            add(old_data.login, sha_hash_copy);
        }
    }

    delete[] save_data;
}

void Chat::add(char login[LOGINLENGTH], uint* hash)
{
    int index;
    int i;

    for (i = 0; i < mem_size_; i++) {
        index = hfunc_quad(login, i);
        if (data_[index].status == Status::free) {
            break;
        }
    }
    if (i >= mem_size_) {
        resize();
        add(login, hash);
    } else {
        data_[index] = AuthData(login, hash);
        data_count_++;
    }
}
