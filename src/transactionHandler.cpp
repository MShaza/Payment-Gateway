#include "Transaction.h"
#include "Encryption.h"
#include <random>
#include <sstream>
   /**
 * functionName :   generateTransactionId
 * Parameters   :   None
 * Return Value :   transaction Id
 * Notes        :   genetare the transaction Id
*/
std::string generateTransactionId(){
    std::ostringstream oss;
    std::srand(std::time(nullptr));
    oss<<"TRANX"<<std::rand() % 100000;
    return oss.str();
}
    /**
 * functionName :   generateTransaction
 * Parameters   :   card Number, tranbsaction Amount, encryption key
 * Return Value :   transaction object
 * Notes        :   genetare the transaction and obejct
*/
Transaction generateTransaction(std::string cardNumber, double transctionAmount, std::string &encryptionkey){
    Transaction tranx;
    tranx.transactionId =  generateTransactionId();
    tranx.transactionCardNumber = Encryption::encryptData(cardNumber,encryptionkey);
    tranx.transactionAmount =  transctionAmount;
    tranx.transactionStatus = "Pending";
    tranx.transactionTimeStamp =  std::time(nullptr);  // Capture current time
    return tranx;

}
    /**
 * functionName :   generateTransaction
 * Parameters   :   transaction Object, decryption key
 * Return Value :   None
 * Notes        :   process the transaction with dummy logic
*/
void processTransaction(Transaction &tranx, std::string decryptionKey){
    tranx.transactionStatus =  (tranx.transactionAmount <= 1000.00) ? "DENIED": "SUCCESSFUL"; // Dummy login, replace with the actual one.
    tranx.transactionCardNumber =  Encryption::decryptData(tranx.transactionCardNumber, decryptionKey);
    transactionRecipt(tranx);
    return;

}
    /**
 * functionName :   transactionRecipt
 * Parameters   :   None
 * Return Value :   None
 * Notes        :   genertae the recipt of transaction
*/
void transactionRecipt(Transaction &tranxData){
        std::cout<<"Transaction Id                  "<<tranxData.transactionId<<std::endl;
        std::cout<<"Transaction Amount              "<<tranxData.transactionAmount<<std::endl;
        std::cout<<"Transaction Status              "<<tranxData.transactionStatus<<std::endl;
        std::cout<<"Transaction Time Stamp          "<<tranxData.transactionTimeStamp<<std::endl;     

}