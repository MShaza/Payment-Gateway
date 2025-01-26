#include "transaction.h"
int main(){
    Transaction tranx;
    std::string key = "paymentgateway";
    std::string cardNumber;
    double transactionAmount;
    std::cout<<"Welcome to the payment gateaway system"<<std::endl;
    std::cout<<"Enter Your Card Number"<<std::endl;
    std::cin>>cardNumber;
    std::cout<<"Enter Transaction Amount"<<std::endl;
    std::cin>>transactionAmount;
    tranx = generateTransaction(cardNumber,transactionAmount, key);
    processTransaction(tranx, key);

}