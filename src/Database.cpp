#include "Database.h"
#include <iostream>
 MYSQL* Database::conn = nullptr;
 bool Database::connectToDatabase(){
    conn = mysql_init(nullptr);
    if(!conn){
        std::cerr << "[ERROR] MySQL initialization failed!" << std::endl;
        return false;
    }
    if(!mysql_real_connect(conn, "localhost", "Muhammad", "Askari15h", "PAYMENTGATEWAY", 3306, nullptr, 0)){
        std::cerr << "[ERROR] MySQL connection failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    else{
        std::cout << "[DEBUG] Connected to MySQL database!" << std::endl;
    return true;
    }
 }
void Database::closeConnection(){
     if (conn) {
        mysql_close(conn);
        conn = nullptr;
        std::cout << "[DEBUG] MySQL connection closed!" << std::endl;
    }
}
bool Database::insertTransaction(const Transaction &tranx){
    //check if connection is establishes
    
    if(!conn){
        return false;
    }
    //creatre query string
    std::string dbQuery =  "INSERT INTO TRANSACTION (transaction_id, card_number, transaction_amount, transaction_status) VALUES ('" 
        + tranx.transactionId + "', '" 
        + tranx.transactionCardNumber + "', " 
        + std::to_string(tranx.transactionAmount) + ", '"
        + tranx.transactionStatus + "')";
        //execute the query
    if(mysql_query(conn, dbQuery.c_str())){ // return 0 if query executed correctly
         std::cerr << "[ERROR] Failed to insert transaction: " << mysql_error(conn) << std::endl;
        return false;
    }
     std::cout << "[DEBUG] Transaction inserted successfully!" << std::endl;
    return true;
}
Transaction Database::getTransactionById(const std::string& transaction_id){
    Transaction tranx;
    if(!conn) return tranx;
    std::string dbQuery = "SELECT transaction_id, card_number, transaction_amount, transaction_status FROM TRANSACTION WHERE transaction_id = '" + transaction_id + "'";
    if(mysql_query(conn, dbQuery.c_str())){
        std::cerr<<"[ERROR] Failed to fetch the transaction "<< mysql_error(conn) << std::endl;
    }
    MYSQL_RES* res =  mysql_store_result(conn);
    MYSQL_ROW row;
    if((row =  mysql_fetch_row(res))){
        tranx.transactionId = row[0];
        tranx.transactionCardNumber = row[1];
        //std::cout << "[DEBUG] Transaction Query successfully!" <<"["<<row[2]<<"]" <<std::endl;
        tranx.transactionAmount = std::stod(row[2]);

    }
    mysql_free_result(res);
    return tranx;
}
bool Database::updateStatus(const Transaction& tranx){
    if(!conn) return false;
    std::string dbQuery = "UPDATE TRANSACTION SET transaction_status = '" + tranx.transactionStatus + "' WHERE transaction_id = '" + tranx.transactionId + "'";
    if(mysql_query(conn, dbQuery.c_str())){ // return 0 id executed
    std::cerr<<"[ERROR] Failed to fetch the transaction "<< mysql_error(conn) << std::endl;
        return false;
    }
    else{
        std::cout << "[DEBUG] Transaction inserted successfully!" << std::endl;
        return true;
    }
}