#include "Server.h"
#include "Transaction.h"
#include "Database.h"


int main() {
    try {
        // Create Boost ASIO IO Context
        boost::asio::io_context io_context;
        if (!Database::connectToDatabase()) {
            std::cerr << "[ERROR] Failed to connect to the database. Exiting..." << std::endl;
            return 1;
        }
        // Start the server on port 8080
        std::shared_ptr<Server> server = std::make_shared<Server>(io_context, 8080);
        server->run();

        // Run the event loop (keeps the server running)
        io_context.run();
        Database::closeConnection();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
    
    return 0;
}