include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(flight-tests
    FlightTest.cpp
    FlightStatusTest.cpp
    FlightSimulatorTest.cpp
    ${CMAKE_SOURCE_DIR}/server/FlightSimulator.cpp
)
target_include_directories(flight-tests PRIVATE ${CMAKE_SOURCE_DIR}/server)
target_link_libraries(flight-tests PRIVATE flight-common GTest::gtest_main GTest::gmock)

include(GoogleTest)
gtest_discover_tests(flight-tests)
