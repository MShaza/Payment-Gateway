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

Transaction generateTransaction(std::string cardNumber, double transctionAmount, std::string &encryptionkey){
    Transaction tranx;
    tranx.transactionId =  generateTransactionId();
    tranx.transactionCardNumber = Encryption::encryptData(cardNumber,encryptionkey);
    tranx.transactionAmount =  transctionAmount;
    tranx.transactionStatus = "Pending";
    tranx.transactionTimeStamp =  std::time(nullptr);  // Capture current time
    return tranx;

}
void processTransaction(Transaction &tranx, std::string decryptionKey){
    tranx.transactionStatus =  (tranx.transactionAmount <= 1000.00) ? "DENIED": "SUCCESSFUL"; // Dummy login, replace with the actual one.
    tranx.transactionCardNumber =  Encryption::decryptData(tranx.transactionCardNumber, decryptionKey);
    transactionRecipt(tranx);
    return;

}

void transactionRecipt(Transaction &tranxData){
        std::cout<<"Transaction Id                  "<<tranxData.transactionId<<std::endl;
        std::cout<<"Transaction Amount              "<<tranxData.transactionAmount<<std::endl;
        std::cout<<"Transaction Status              "<<tranxData.transactionStatus<<std::endl;
        std::cout<<"Transaction Time Stamp          "<<tranxData.transactionTimeStamp<<std::endl;     

}