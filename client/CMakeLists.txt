find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets Qml Quick QuickWidgets Location Positioning)

add_executable(flight-client
  main.cpp
  FlightClient.h
  FlightClient.cpp
  FlightTableModel.h
  FlightTableModel.cpp
  MainWindow.h
  MainWindow.cpp
  qml.qrc
)

target_link_libraries(flight-client PRIVATE
  flight-common
  Qt6::Core
  Qt6::Network
  Qt6::Widgets
  Qt6::WebSockets
  Qt6::Qml
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Location
  Qt6::Positioning
)
