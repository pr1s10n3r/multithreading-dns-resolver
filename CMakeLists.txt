cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME "multithreading-dns-resolver")
set(CMAKE_CXX_STANDARD_REQUIRED 17)

project(${PROJECT_NAME} CXX)
find_package(Poco REQUIRED)

add_executable(${PROJECT_NAME} src/Main.cpp src/Application.cpp src/InputLoad.cpp src/Resolver.cpp)
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::Net Poco::Util)