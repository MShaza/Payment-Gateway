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
    _acceptor.async_accept([&](boost::system::error_code ec, boost::asio::ip::tcp::socket _socket){
            if(!ec){
                handleRequest(_socket);
            }
            else{
                doAccept();
            }

    });   
} 
/**
 * functionName :   handleRequest
 * Parameters   :   socket object  of boost asio library   
 * Return Value :   None
 * Notes        :   recieve the reaquest recieved by the server
*/
void Server::handleRequest(asio::ip::tcp::socket &_socket){
    auto self =  shared_from_this();
    //to mange http request wee nedd buffer form beast
    beast::flat_buffer vBuffer;
    //now we read the data sen dfrom teh client
    beast::http::async_read(_socket, vBuffer,isRequest,[self, this, _socket = std::move(_socket)](boost::system::error_code ec, std::size_t bytesTransferre){
        // check if there are no error
        if(!ec){
            std::string responseBody;
            property_tree::ptree jsonBody;
            //get teh json file
            property_tree::read_json(isRequest.body(), jsonBody);
            http::response<http::string_body> response;
            try{
                 // check the API & method type
                  if((isRequest.target() == "/payment/initiate") && (isRequest.method() == http::verb::post) ){
                    // Initiate function call
                    responseBody = initiateTransaction(jsonBody);
                    response.result(http::status::ok);
                    }
                    else if((isRequest.target() == "/payment/process") && (isRequest.method() == http::verb::post) ){
                    //call the function
                    response.result(http::status::ok);
                    }
                    else {
                        responseBody = "{\"status\":\"error\", \"message\":\"Invalid API endpoint\"}";
                        response.result(http::status::not_found);
                    }
                }
            catch(const property_tree::ptree_error &e){
                responseBody = "{\"status\":\"error\", \"message\":\"Invalid JSON format\"}";
                response.result(http::status::bad_request);  // 400 Bad Request
            }
            catch (std::exception &e){
                responseBody = "{\"status\":\"error\", \"message\":\"" + std::string(e.what()) + "\"}";
                response.result(http::status::internal_server_error);  // 500 Internal Server Error
                // send response functrion
            }
            response.set(http::field::content_type, "application/json");
            response.body() = responseBody;
            response.prepare_payload();
            http::async_write(socket, response, [socket =  std::move(socket)](boost::system::error_code, std::size_t){});
        }
    });
}
    /**
 * functionName :   initiateTransaction
 * Parameters   :   boost beat property_tree object
 * Return Value :   string response
 * Notes        :   create the trabsaction object and genertae trabnsaction Id
*/
std::string Server::initiateTransaction(property_tree::ptree &jsonBody){
    //pasrse the JSON
    std::string key = "paymentgateway";
    std::string carßdNumber =  jsonBody.get<std::string>("card_number");
    double paymentAmount = jsonBody.get<double>("payment_amount");
    Transaction tranx = generateTransaction(carßdNumber, paymentAmount,key);// create transaction
    transactions[tranx.transactionId] = tranx;
    return "{\"status\":\"success\", \"transaction_id\":\"" + tranx.transactionId + "\"}";
}
