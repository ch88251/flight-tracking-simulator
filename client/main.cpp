#include <QApplication>

#include "FlightClient.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  FlightClient client;
  MainWindow window(&client);
  window.show();

  client.connectToServer(QUrl(QStringLiteral("ws://127.0.0.1:4500")));

  return app.exec();
}
