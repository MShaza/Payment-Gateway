#include "Database.h"
#include <iostream>
 MYSQL* Database::conn = nullptr;
     /**
 * functionName :   connectToDatabase
 * Parameters   :   None
 * Return Value :   bool if db connected
 * Notes        :   connects to db
*/
 bool Database::connectToDatabase(){
     std::cout << "[DEBUG - connectToDatabase] Enter function"<<std::endl;
    conn = mysql_init(nullptr);
    if(!conn){
        std::cerr << "[ERROR] MySQL initialization failed!" << std::endl;
         std::cout << "[DEBUG - connectToDatabase] Exit function"<<std::endl;
        return false;
    }
    if(!mysql_real_connect(conn, "localhost", "Muhammad", "Askari15h", "PAYMENTGATEWAY", 3306, nullptr, 0)){
        std::cerr << "[ERROR] MySQL connection failed: " << mysql_error(conn) << std::endl;
         std::cout << "[DEBUG - connectToDatabase] Exit function"<<std::endl;
        return false;
    }
    else{
        std::cout << "[DEBUG] Connected to MySQL database!" << std::endl;
         std::cout << "[DEBUG - connectToDatabase] Exit function"<<std::endl;
    return true;
    }
 }
      /**
 * functionName :   closeConnection
 * Parameters   :   None
 * Return Value :   none
 * Notes        :   close the connection
*/
void Database::closeConnection(){
     std::cout << "[DEBUG - closeConnection] Enter function"<<std::endl;
     if (conn) {
        mysql_close(conn);
        conn = nullptr;
        std::cout << "[DEBUG] MySQL connection closed!" << std::endl;
         std::cout << "[DEBUG - closeConnection] Exit function"<<std::endl;
    }
}
      /**
 * functionName :   insertTransaction
 * Parameters   :   transaction object
 * Return Value :   bool 
 * Notes        :   insert data to db
*/
bool Database::insertTransaction(const Transaction &tranx){
    //check if connection is establishes
    std::cout << "[DEBUG - insertTransaction] Enter function"<<std::endl;
    if(!conn){
        return false;
        std::cout << "[DEBUG - insertTransaction] Exit function"<<std::endl;
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
          std::cout << "[DEBUG - insertTransaction] Exit function"<<std::endl;
        return false;
    }
     std::cout << "[DEBUG] Transaction inserted successfully!" << std::endl;
      std::cout << "[DEBUG - insertTransaction] Exit function"<<std::endl;
    return true;
}
      /**
 * functionName :   getTransactionById
 * Parameters   :   transaction id
 * Return Value :   transaction object 
 * Notes        :   get the data from db
*/
Transaction Database::getTransactionById(const std::string& transaction_id){
     std::cout << "[DEBUG - getTransactionById] Enter function"<<std::endl;
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
    std::cout << "[DEBUG - getTransactionById] Exit function"<<std::endl;
    return tranx;
}
      /**
 * functionName :   updateStatus
 * Parameters   :   transaction object
 * Return Value :   bool
 * Notes        :   update status file in the db
*/
bool Database::updateStatus(const Transaction& tranx){
    std::cout << "[DEBUG - updateStatus] Enter function"<<std::endl;
    if(!conn) return false;
    std::string dbQuery = "UPDATE TRANSACTION SET transaction_status = '" + tranx.transactionStatus + "' WHERE transaction_id = '" + tranx.transactionId + "'";
    if(mysql_query(conn, dbQuery.c_str())){ // return 0 id executed
    std::cerr<<"[ERROR] Failed to fetch the transaction "<< mysql_error(conn) << std::endl;
        return false;
        std::cout << "[DEBUG - updateStatus] Exit function"<<std::endl;
    }
    else{
        std::cout << "[DEBUG] Transaction inserted successfully!" << std::endl;
        std::cout << "[DEBUG - updateStatus] Exit function"<<std::endl;
        return true;
    }
}