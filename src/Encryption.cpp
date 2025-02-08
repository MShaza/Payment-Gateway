#include <string>
#include "Encryption.h"
    /**
 * functionName :   encryptData
 * Parameters   :   data, encryption key
 * Return Value :   encrypted data
 * Notes        :   perform the encryption on the given data
*/
std::string Encryption::encryptData(std::string &data, std::string &encyrptionKey){
    std::string encryptData;
    for(int i = 0; i< data.size(); i++){
        encryptData[i] = data[i] ^ encyrptionKey[i % encyrptionKey.size()]; 
    }
    return encryptData;
}
    /**
 * functionName :   decryptData
 * Parameters   :   data, decryption key
 * Return Value :   encrypted data
 * Notes        :   perform the decryption on the given data
*/
std::string Encryption::decryptData(std::string &data, std::string &decryptionKey){
    std::string decryptData;
    for(int i = 0; i< data.size(); i++){
        decryptData[i] = data[i] ^ decryptionKey[i % decryptionKey.size()]; 
    }
    return decryptData;
}
