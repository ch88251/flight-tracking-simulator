find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

add_executable(flight-client
  main.cpp
  FlightClient.h
  FlightClient.cpp
  MainWindow.h
  MainWindow.cpp
)

target_link_libraries(flight-client PRIVATE flight-common Qt6::Core Qt6::Network Qt6::Widgets Qt6::WebSockets)
