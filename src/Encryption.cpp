#include <string>
#include "Encryption.h"
std::string Encryption::encryptData(std::string &data, std::string &encyrptionKey){
    std::string encryptData;
    for(int i = 0; i< data.size(); i++){
        encryptData[i] = data[i] ^ encyrptionKey[i % encyrptionKey.size()]; 
    }
    return encryptData;
}
std::string Encryption::decryptData(std::string &data, std::string &decryptionKey){
    std::string decryptData;
    for(int i = 0; i< data.size(); i++){
        decryptData[i] = data[i] ^ decryptionKey[i % decryptionKey.size()]; 
    }
    return decryptData;
}
