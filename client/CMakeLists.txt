find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

add_executable(flight-client
  main.cpp
)

target_link_libraries(flight-client PRIVATE flight-common Qt6::Core Qt6::Network Qt6::Widgets)
