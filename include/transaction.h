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
void transactionRecipt( Transaction &tranxData){
        std::cout<<"Transaction Id                  "<<tranxData.transactionId;
        std::cout<<"Transaction Amount              "<<tranxData.transactionAmount;
        std::cout<<"Transaction Status              "<<tranxData.transactionStatus;
        std::cout<<"Transaction Time Stamp          "<<tranxData.transactionTimeStamp;     

}






#endif