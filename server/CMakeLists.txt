find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(flight-server
  main.cpp
  FlightSimulator.h
  FlightSimulator.cpp
)

target_link_libraries(flight-server PRIVATE flight-common Qt6::Core Qt6::Network)
