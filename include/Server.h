#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree//json_parser.hpp>
#include <string>
#include <map>
#include "Transaction.h"
namespace http = boost::beast::http; // for making http request
namespace asio = boost::asio; // for network programming and making connection
namespace beast = boost::beast; 
namespace property_tree = boost::property_tree; // for  json
/**
 * className   Server
 * Notes       class that handles the request to the server
*/
class Server : public std::enable_shared_from_this<Server>
{
private:
    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::socket _socket;
    std::map<std::string, Transaction> transactions; // In-memory transaction database
    http::request<http::string_body> isRequest; // get the request from the clinet
public:
    Server(boost::asio::io_context &_context, int port);
    void run();
private:
    void doAccept();
    void handleRequest(boost::asio::ip::tcp::socket& _socket);
    std::string initiateTransaction(property_tree::ptree &jsonBody);
    std::string processTransaction(property_tree::ptree &jsonBody);

};
#endif