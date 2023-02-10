set(PROJECT_NAME "CafeConLeche")

cmake_minimum_required(VERSION 3.22.1)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD_REQUIRED 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} src/Main.cpp src/Application.cpp src/InputLoad.cpp src/Resolver.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})