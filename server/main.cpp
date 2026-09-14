#include <QCoreApplication>
#include <QDebug>

#include "FlightSimulator.h"

namespace {
constexpr quint16 kDefaultPort = 4500;
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  FlightSimulator simulator;
  simulator.start(1000);

  qDebug() << "Flight simulator running with" << 5 << "aircraft. Press Ctrl+C to stop.";

  return app.exec();
}
