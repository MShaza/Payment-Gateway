#include "Server.h"
#include "Encryption.h"

    Server::Server(asio::io_context& io_context, int port)
    : _acceptor(io_context, boost::asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      _socket(io_context) {
    // You can add additional initialization here if needed
    std::cout << "Server started on port " << port << std::endl;
}
void Server::run(){
    doAccept();
}
/**
 * functionName :   doAccept
 * Parameters   :   None   
 * Return Value :   None
 * Notes        :   Accept the incomming clients
*/
void Server::doAccept(){
    auto self = shared_from_this();  // Capture `Server` instance properly

    _acceptor.async_accept(
        [self](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) mutable {
            if (!ec) {
                // Ensure the socket is properly moved into `handleRequest`
                self->handleRequest(std::move(socket));
            }
            self->doAccept();  //Continue accepting new connections
        });
} 
/**
 * functionName :   handleRequest
 * Parameters   :   socket object  of boost asio library   
 * Return Value :   None
 * Notes        :   recieve the reaquest recieved by the server
*/
void Server::handleRequest(boost::asio::ip::tcp::socket socket) {
    auto self = shared_from_this();
    auto sharedSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));
    auto buffer = std::make_shared<beast::flat_buffer>();
    auto request = std::make_shared<http::request<http::string_body>>();
    auto response = std::make_shared<http::response<http::string_body>>();

    std::cout << "[DEBUG] Waiting for client request..." << std::endl;

    http::async_read(*sharedSocket, *buffer, *request,
        [self, sharedSocket, buffer, request, response](boost::system::error_code ec, std::size_t bytesTransferred) mutable {
            if (ec) {
                std::cerr << "[ERROR] Failed to read request: " << ec.message() << std::endl;
                return;
            }

            std::cout << "[DEBUG] Received request: " << request->target() << std::endl;

            property_tree::ptree jsonBody;
            std::string responseBody;

            try {
                std::stringstream ss(request->body());
                property_tree::read_json(ss, jsonBody);

                if (request->target() == "/payment/initiate" && request->method() == http::verb::post) {
                    std::cout << "[DEBUG] Processing /payment/initiate request..." << std::endl;
                    responseBody = self->initiateTransaction(jsonBody);
                    response->result(http::status::ok);
                } 
                else if (request->target() == "/payment/process" && request->method() == http::verb::post) {
                    std::cout << "[DEBUG] Processing /payment/process request..." << std::endl;
                    responseBody = self->processTransaction(jsonBody);
                    response->result(http::status::ok);
                } 
                else {
                    std::cerr << "[ERROR] Invalid API endpoint" << std::endl;
                    responseBody = "{\"status\":\"error\", \"message\":\"Invalid API endpoint\"}";
                    response->result(http::status::not_found);
                }
            } 
            catch (const property_tree::ptree_error&) {
                std::cerr << "[ERROR] Invalid JSON format" << std::endl;
                responseBody = "{\"status\":\"error\", \"message\":\"Invalid JSON format\"}";
                response->result(http::status::bad_request);
            } 
            catch (const std::exception& e) {
                std::cerr << "[ERROR] Exception in request processing: " << e.what() << std::endl;
                responseBody = "{\"status\":\"error\", \"message\":\"" + std::string(e.what()) + "\"}";
                response->result(http::status::internal_server_error);
            }

            response->set(http::field::content_type, "application/json");
            response->body() = responseBody;
            response->prepare_payload();

            std::cout << "[DEBUG] Sending response..." << std::endl;

            http::async_write(*sharedSocket, *response,
                [sharedSocket, response](boost::system::error_code ec, std::size_t) mutable {
                    if (ec) {
                        std::cerr << "[ERROR] Failed to send response: " << ec.message() << std::endl;
                    } else {
                        std::cout << "[DEBUG] Response sent successfully!" << std::endl;
                    }
                    sharedSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                });
        });
}
 
    /**
 * functionName :   initiateTransaction
 * Parameters   :   boost beat property_tree object
 * Return Value :   string response
 * Notes        :   create the transaction object and genertae transaction Id
*/
std::string Server::initiateTransaction(property_tree::ptree &jsonBody){
    //pasrse the JSON
    std::string key = "paymentgateway";
    std::string cardNumber =  jsonBody.get<std::string>("card_number");
    double paymentAmount = jsonBody.get<double>("payment_amount");
    Transaction tranx = generateTransaction(cardNumber, paymentAmount,key);// create transaction
    transactions[tranx.transactionId] = tranx;
    return "{\"status\":\"success\", \"transaction_id\":\"" + tranx.transactionId + "\"}";
}
    /**
 * functionName :   processTransaction
 * Parameters   :   boost beat property_tree object
 * Return Value :   string response
 * Notes        :   proecess the transaction
*/
std::string Server::processTransaction(property_tree::ptree &jsonBody){
    std::string key = "paymentgateway";
    std::string transactionId = jsonBody.get<std::string>("transaction_id");
    if(transactions.find(transactionId) != transactions.end()){
        Transaction &tranx = transactions[transactionId];
        std::string key = "paymentgateway";
        processTransactions(tranx, key);
    return "{\"status\":\"" + tranx.transactionStatus + "\", \"transaction_id\":\"" + tranx.transactionId + "\"}";
    } 
    return "{\"status\":\"error\", \"message\":\"Transaction not found\"}";
}