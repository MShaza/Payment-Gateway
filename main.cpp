#include "Server.h"
#include "Transaction.h"

int main() {
    try {
        // Create Boost ASIO IO Context
        boost::asio::io_context io_context;

        // Start the server on port 8080
        std::shared_ptr<Server> server = std::make_shared<Server>(io_context, 8080);
        server->run();

        // Run the event loop (keeps the server running)
        io_context.run();

    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
    
    return 0;
}