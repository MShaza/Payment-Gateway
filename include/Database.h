#ifndef DATABASE_H
#define DATABASE_H
#include "Transaction.h"
#include <string>
#include <mysql.h>
class Database{
public:
    static bool connectToDatabase();
    static bool insertTransaction(const Transaction& tranx);
    static Transaction getTransactionById(const std::string& transactionId);
    static void closeConnection();
    static bool updateStatus(const Transaction &tranx);
private:
    static MYSQL* conn;
};
#endif