#include "transaction.h"
#include "Encryption.h"
#include <random>
#include <sstream>
std::string generateTransactionId(){
    std::ostringstream oss;
    std::srand(std::time(nullptr));
    oss<<"TRANX"<<std::rand() % 100000;
    return oss.str();
}

void generateTransaction(std::string tranxId, std::string cardNumber, double transctionAmount, std::string &encryptionkey){
    Transaction tranx;
    tranx.transactionId =  generateTransactionId();
   tranx.transactionCardNumber = encryptData(cardNumber,encryptionkey);
    tranx.transactionAmount =  transctionAmount;
    tranx.transactionStatus = "Pending";
    tranx.transactionTimeStamp =  std::time(nullptr);  // Capture current time

}