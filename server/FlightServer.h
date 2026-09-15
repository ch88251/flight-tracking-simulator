#pragma once

#include <QList>
#include <QObject>
#include <QString>
#include <QWebSocketServer>

class FlightSimulator;
class QWebSocket;

class FlightServer : public QObject
{
  Q_OBJECT

public:
  explicit FlightServer(FlightSimulator *simulator, QObject *parent = nullptr);

  bool start(quint16 port);
  void stop();
  bool isListening() const;

signals:
  void statusMessage(const QString &message);

private slots:
  void handleNewConnection();
  void handleDisconnected();
  void handleTextMessage(const QString &message);
  void broadcastFlights();

private:
  QString serializedFlights() const;

  FlightSimulator *m_simulator = nullptr;
  QWebSocketServer m_server;
  QList<QWebSocket *> m_clients;
};