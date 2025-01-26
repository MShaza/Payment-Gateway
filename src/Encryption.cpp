#include <string>
#include "Encryption.h"
std::string encryptData(std::string &data, std::string &encyrptionKey){
    std::string encryptData;
    for(int i = 0; i< data.size(); i++){
        encryptData[i] = data[i] ^ encyrptionKey[i % encyrptionKey.size()]; 
    }
    return encryptData;
}