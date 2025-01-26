#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>
#include <ctime>
struct Transaction
{
    std::string transactionId;
    std::string transactionCardNumber;
    std::string transactionStatus;
    double transactionAmount;
    std::time_t transactionTimeStamp; 
};
void transactionRecipt(Transaction &tranxData);
Transaction generateTransaction(std::string cardNumber, double transctionAmount, std::string &encryptionkey);
void processTransaction(Transaction &tranx, std::string decryptionKey);





#endif