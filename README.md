# Flight Tracking Simulator

A C++ / Qt6 flight tracking simulator. A server-side simulator seeds a small
fleet of aircraft and advances their position, altitude, heading, and speed
on a timer, modeling each flight's lifecycle from takeoff through climb,
cruise, descent, and landing. A shared `flight-common` library defines the
`Flight` data model (with JSON serialization) used across the server and
client.

## Project layout

```
common/    Shared data model (Flight, FlightStatus) — built as the flight-common static library
server/    flight-server executable: runs FlightSimulator and prints flight state on each tick
client/    flight-client executable: Qt Widgets GUI (in progress)
tests/     GoogleTest unit tests for the common model and the simulator
```

## Requirements

- CMake 3.16+
- A C++17 compiler
- Qt6 (`Core`, `Network`, `WebSockets`, and `Widgets` for the client)

## Building

From the repository root:

```bash
cmake -B build -S .
cmake --build build
```

If CMake can't find Qt6 automatically, point it at your Qt installation:

```bash
cmake -B build -S . -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
```

This builds the `flight-common` library, the `flight-server` and
`flight-client` executables, and (by default) the test suite.

## Running

Run the server simulator, which seeds five aircraft and prints their state
to stdout once per second:

```bash
./build/server/flight-server
```

Press `Ctrl+C` to stop it.

The client (`./build/client/flight-client`) connects to the server at
`ws://127.0.0.1:4500` with `QWebSocket` and reports connection status and
received flight updates. The server accepts clients with `QWebSocketServer`
and broadcasts the serialized flight list after each simulator tick.

## Running the tests

Tests are built with [GoogleTest](https://github.com/google/googletest)
(fetched automatically via CMake's `FetchContent`) and are on by default via
the `BUILD_TESTING` option. Run them with CTest:

```bash
ctest --test-dir build
```

Or run the test binary directly for more verbose output:

```bash
./build/tests/flight-tests
```

To build without tests, configure with `-DBUILD_TESTING=OFF`.
