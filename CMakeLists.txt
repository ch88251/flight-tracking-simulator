cmake_minimum_required(VERSION 3.16)
project(flight-tracking-simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_library(flight-common STATIC
    common/Flight.h
    common/Flight.cpp
    common/FlightStatus.h
)
target_include_directories(flight-common PUBLIC common)
target_link_libraries(flight-common PUBLIC Qt6::Core Qt6::Network)

add_subdirectory(server)
add_subdirectory(client)

option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
