cmake_minimum_required(VERSION 3.10)
project(PaymentGateway)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)
# Find Boost Libraries
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Threads REQUIRED)
add_library(Encryption STATIC src/Encryption.cpp)
add_executable(PaymentGateway main.cpp src/transactionHandler.cpp src/Server.cpp)
target_link_libraries(PaymentGateway Encryption Boost::system Boost::filesystem Threads::Threads)