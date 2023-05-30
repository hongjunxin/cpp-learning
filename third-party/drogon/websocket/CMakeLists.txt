cmake_minimum_required(VERSION 3.7)
project(drogon_websocket VERSION 0.0.1)

set(SERVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/server.cc)
set(CLIENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cc)

find_package(OpenSSL REQUIRED)
find_package(Drogon REQUIRED)
find_package(plog REQUIRED)

# build server.out
add_executable(server.out ${SERVER_SOURCES})
set_target_properties(server.out PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME server.out)
target_link_libraries(server.out PRIVATE 
    OpenSSL::SSL 
    Drogon::Drogon
    plog::plog
    junction
    turf)

# build client.out
add_executable(client.out ${CLIENT_SOURCES})
set_target_properties(client.out PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME client.out)
target_link_libraries(client.out PRIVATE 
    OpenSSL::SSL 
    Drogon::Drogon
    plog::plog)