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
    std::cout << "[DEBUG - generateTransactionId] Enter function"<< std::endl;
    std::ostringstream oss;
    //std::srand(std::time(nullptr));
    //oss<<"TRANX"<<std::rand() % 100000;
    std::random_device rd;                  // Non-deterministic random number generator (uses hardware entropy)
    std::mt19937 gen(rd());                 // Mersenne Twister engine (high-quality PRNG), seeded with `rd`
    std::uniform_int_distribution<> dis(10000, 99999); // Uniform distribution between 10,000 and 99,999
    oss << "TRANX" << dis(gen);
    std::cout << "[DEBUG - generateTransactionId] Exit function successfully"<< std::endl;
    return oss.str();
}
    /**
 * functionName :   generateTransaction
 * Parameters   :   card Number, tranbsaction Amount, encryption key
 * Return Value :   transaction object
 * Notes        :   genetare the transaction and obejct
*/
Transaction generateTransaction(std::string cardNumber, double transctionAmount){
    std::cout << "[DEBUG - generateTransaction] Enter function"<< std::endl;
    Transaction tranx;
    tranx.transactionId =  generateTransactionId();
    std::cout << "[DEBUG - generateTransaction] CARD NUMBER ["<<cardNumber <<"]"<< std::endl;
    tranx.transactionCardNumber = Encryption::encryptData(cardNumber);
    //std::cout << "[DEBUG - generateTransaction] CARD NUMBER ["<<tranx.transactionCardNumber <<"]"<< std::endl;
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
const std::string processTransactions(Transaction &tranx){
    std::cout << "[DEBUG - processTransactions] Enter function"<<std::endl;
    tranx.transactionStatus =  (tranx.transactionAmount <= 1000.00) ? "DENIED": "SUCCESSFUL"; // Dummy login, replace with the actual one.
    tranx.transactionCardNumber =  Encryption::decryptData(tranx.transactionCardNumber);
    transactionRecipt(tranx);
    std::cout << "[DEBUG - processTransactions] Exit function"<<std::endl;
    return tranx.transactionStatus;

}
    /**
 * functionName :   transactionRecipt
 * Parameters   :   None
 * Return Value :   None
 * Notes        :   genertae the recipt of transaction
*/
void transactionRecipt(Transaction &tranxData){
     std::cout << "[DEBUG - processTransactions] Enter function"<<std::endl;
        std::cout<<"Transaction Id                  "<<tranxData.transactionId<<std::endl;
        std::cout<<"Transaction Amount              "<<tranxData.transactionAmount<<std::endl;
        std::cout<<"Transaction Status              "<<tranxData.transactionStatus<<std::endl;
        std::cout<<"Transaction Time Stamp          "<<tranxData.transactionTimeStamp<<std::endl;    
         std::cout << "[DEBUG - processTransactions] Exit function"<<std::endl; 

}