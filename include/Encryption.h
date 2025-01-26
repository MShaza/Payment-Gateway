#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <string>
#include <iostream>
class Encryption{
    public:
std::string static encryptData(std::string &data, std::string &encyrptionKey);
std::string static decryptData(std::string &data, std::string &decryptionKey);

};


#endif