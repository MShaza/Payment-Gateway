#include <string>
#include <openssl/rand.h>
#include <openssl/err.h>
#include "Encryption.h"
#include <stdexcept>
#include <fstream>
#include <filesystem>
    /**
 * functionName :   generateKeys
 * Parameters   :   none
 * Return Value :   none
 * Notes        :   Generate key for encryption
*/
void Encryption::generateKeys(){
    std::cout << "[DEBUG - generateKeys] Enter function"<<std::endl;
    const int KEY_LENGTH = 32; // AES-256
    unsigned char key[KEY_LENGTH];
    if (RAND_bytes(key, KEY_LENGTH) != 1) {
        throw std::runtime_error("Failed to generate secure key");
    }
    std::string keyGenerated(reinterpret_cast<char*>(key),KEY_LENGTH);
    std::ofstream keyFile("encryption_key.bin", std::ios::binary);
    keyFile.write(keyGenerated.data(), keyGenerated.size());
    keyFile.close();
     std::cout << "[DEBUG - generateKeys] Exit function"<<std::endl;
}
    /**
 * functionName :   getKey
 * Parameters   :   none
 * Return Value :   string
 * Notes        :   perform the encryption on the given data
*/
std::string Encryption::getKey(bool encryption){
    std::cout << "[DEBUG - getKey] Enter function"<<std::endl;
    std::string key;
    // check if file exist
    std::string fileName = "encryption_key.bin";
    if(!std::filesystem::exists(fileName)){
        if(encryption){
        generateKeys();
        }
        else{
            return key;
        }
    }
    std::ifstream keyFile("encryption_key.bin", std::ios::binary);
    key =std::string((std::istreambuf_iterator<char>(keyFile)), std::istreambuf_iterator<char>());
    keyFile.close(); 
    std::cout << "[DEBUG - getKey] Exit function"<<std::endl;
    return key;
}
    /**
 * functionName :   encryptData
 * Parameters   :   data, encryption key
 * Return Value :   encrypted data
 * Notes        :   perform the encryption on the given data
*/
std::string Encryption::encryptData(std::string &data){
    std::cout << "[DEBUG - encryptData] Enter function"<<std::endl;
    std::string encyrptionKey = getKey(true);
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    unsigned char iv[16]; // IV 128 bit
    if (!ctx)  std::cout << "[DEBUG - encryptData] Could create context"<<std::endl;
    //throw std::runtime_error("Failed to create encryption context");
    // Encrypt data using AES-256-CBC
    //
    std::cout << "[DEBUG - encryptData] Encryption context created"<<std::endl;

    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        std::cout << "[DEBUG - encryptData] IV generation failed"<<std::endl;
        //std::cerr << "IV generation failed\n";
    }
    std::cout << "[DEBUG - encryptData] IV Generated"<<std::endl;
    //Initialization encryption with CBC mode and IV
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(encyrptionKey.data()), iv) != 1){
        EVP_CIPHER_CTX_free(ctx);
        std::cout << "[DEBUG - encryptData] Encryption initialization failed"<<std::endl;
        //throw std::runtime_error("Encryption initialization failed");
    }
    std::cout << "[DEBUG - encryptData] Encryption initialized"<<std::endl;
    //Encrypt data
    std::string cipherTxt;
    int chipherLen = 0;
    int len;
    const std::string actual_ciphertext = data.substr(EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
    cipherTxt.resize(data.size() + EVP_MAX_BLOCK_LENGTH);
    if(EVP_EncryptUpdate(ctx,reinterpret_cast<unsigned char*>(&cipherTxt[0]),&len,reinterpret_cast<const unsigned char*>(data.data()), data.size()) != 1){
        EVP_CIPHER_CTX_free(ctx);
         std::cout << "[DEBUG - encryptData] Encryption failed"<<std::endl;
         //throw std::runtime_error("Encryption failed");
    }
        std::cout << "[DEBUG - encryptData] Encryption successfully"<<std::endl;
    chipherLen = len; 
    // finalzing the encryption
    if(EVP_EncryptFinal_ex(ctx,reinterpret_cast<unsigned char*>(&cipherTxt[0] + len), &len) != 1){
        EVP_CIPHER_CTX_free(ctx);
         std::cout << "[DEBUG - encryptData] Encryption finalization failed"<<std::endl;
        //throw std::runtime_error("Encryption finalization failed");
    }
    std::cout << "[DEBUG - encryptData] Encryption finalized"<<std::endl;
    chipherLen +=  len;
    cipherTxt.resize(chipherLen);

    cipherTxt = std::string(reinterpret_cast<char*>(iv), EVP_CIPHER_iv_length(EVP_aes_256_cbc())) + cipherTxt;

    EVP_CIPHER_CTX_free(ctx);
     std::cout << "[DEBUG - encryptData] Exit function successfully"<<std::endl;
    return cipherTxt;
}
    /**
 * functionName :   decryptData
 * Parameters   :   data, decryption key
 * Return Value :   encrypted data
 * Notes        :   perform the decryption on the given data
*/
std::string Encryption::decryptData(std::string &data){
    std::cout << "[DEBUG - decryptData] Enter function"<<std::endl;
    std::string decryptedText;
    std::string decryptionKey = getKey(false);
    if(decryptionKey.empty()){
        //throw std::runtime_error("Key not found");
        std::cout << "[DEBUG - decryptData] Key not found"<<std::endl;
        return decryptedText;

    }
    if(data.size() < EVP_CIPHER_iv_length(EVP_aes_256_cbc())){
       // throw std::runtime_error("Invalid ciphertext");
    }
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)  std::cout << "[DEBUG - decryptData] Failed to create decryption context"<<std::endl;//throw std::runtime_error("Failed to create decryption context");

    std::cout << "[DEBUG - decryptData] Decryption context created"<<std::endl;

    std::string iv_str = data.substr(0, EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
    const unsigned char* iv = reinterpret_cast<const unsigned char*>(iv_str.data());

    //Initialization the decryption
    if(EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, reinterpret_cast<const unsigned char*>(decryptionKey.data()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        //throw std::runtime_error("Failed to initialize decryption");
        std::cout << "[DEBUG - decryptData] Decryption Initialization failed"<<std::endl;
    }
    std::cout << "[DEBUG - decryptData] Decryption Initialized"<<std::endl;
    int decryptedTextLen = 0;
    int len;
    decryptedText.resize(data.size());

    const std::string actualData =  data.substr(EVP_CIPHER_iv_length(EVP_aes_256_cbc()));  

    //perform devryption
    if(EVP_DecryptUpdate(ctx,
    reinterpret_cast<unsigned char*>(&decryptedText[0]), &len, reinterpret_cast<const unsigned char*>(actualData.data()), actualData.size()) != 1){
        EVP_CIPHER_CTX_free(ctx);
        //throw std::runtime_error("Decryption failed");
        std::cout << "[DEBUG - decryptData] Decryption failed"<<std::endl;
    }
    std::cout << "[DEBUG - decryptData] Decryption Successfully"<<std::endl;
    decryptedTextLen = len;
    //finalizing the decryption
    if(EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&decryptedText[decryptedTextLen]), &len) != 1){
        EVP_CIPHER_CTX_free(ctx);
        //throw std::runtime_error("Decryption finalization failed");
        std::cout << "[DEBUG - decryptData] Decryption finalization failed"<<std::endl;
    }
    std::cout << "[DEBUG - decryptData] Decryption finalized"<<std::endl;
    decryptedTextLen +=len;
    decryptedText.resize(decryptedTextLen);
    //EVP_CIPHER_CTX_free(ctx);
    std::cout << "[DEBUG - decryptData] Function exit successfully "<<std::endl;
    return decryptedText;
}
