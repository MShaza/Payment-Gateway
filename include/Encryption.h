#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <string>
#include <iostream>
#include <openssl/evp.h>


class Encryption{
    public:
std::string static encryptData(std::string &data);
std::string static decryptData(std::string &data);
void static generateKeys();
std::string static getKey(bool encryption);

};


#endif