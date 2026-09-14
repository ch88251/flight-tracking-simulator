#include <QCoreApplication>
#include <QDebug>

#include "FlightServer.h"
#include "FlightSimulator.h"

namespace {
constexpr quint16 kDefaultPort = 4500;
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  FlightSimulator simulator;
  FlightServer server(&simulator);
  QObject::connect(&server, &FlightServer::statusMessage, &app,
                   [](const QString &message) { qInfo().noquote() << message; });

  if (!server.start(kDefaultPort)) {
    return 1;
  }

  simulator.start(1000);

  qDebug() << "Flight simulator running with" << simulator.flights().size()
           << "aircraft. Press Ctrl+C to stop.";

  const int exitCode = app.exec();
  server.stop();
  return exitCode;
}
