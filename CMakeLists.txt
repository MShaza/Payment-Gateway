cmake_minimum_required(VERSION 3.10)
project(PaymentGateway)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

# Set MySQL paths manually 
set(MySQL_INCLUDE_DIR "/opt/homebrew/include/mysql")
set(MySQL_LIBRARY "/opt/homebrew/lib/libmysqlclient.dylib")
find_package(OpenSSL REQUIRED)
# Set openssl paths manually 
#set(openssl_INCLUDE_DIR "/opt/homebrew/opt/openssl@3/include/openssl")

# Include MySQL headers and link libraries
include_directories(${MySQL_INCLUDE_DIR})
#link_directories(${MySQL_LIBRARY})
# Include openssl headers and link libraries
include_directories(${OPENSSL_INCLUDE_DIR})



# Find Boost Libraries (Fix Policy Warning)
cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Threads REQUIRED)

# Add Encryption Library
add_library(Encryption STATIC src/Encryption.cpp)

# Add PaymentGateway Executable
add_executable(PaymentGateway main.cpp src/transactionHandler.cpp src/Server.cpp src/Database.cpp)

# Link Libraries (MySQL, Boost, Threads)
target_link_libraries(PaymentGateway Encryption Boost::system Boost::filesystem Threads::Threads ${MySQL_LIBRARY} OpenSSL::SSL OpenSSL::Crypto)
