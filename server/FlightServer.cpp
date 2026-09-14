#include "FlightServer.h"

#include "FlightSimulator.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QHostAddress>
#include <QWebSocket>

#include <utility>

FlightServer::FlightServer(FlightSimulator *simulator, QObject *parent)
  : QObject(parent)
  , m_simulator(simulator)
  , m_server(QStringLiteral("Flight Tracking Simulator"), QWebSocketServer::NonSecureMode, this)
{
  connect(&m_server, &QWebSocketServer::newConnection, this, &FlightServer::handleNewConnection);
  connect(m_simulator, &FlightSimulator::flightsUpdated, this, &FlightServer::broadcastFlights);
}

bool FlightServer::start(quint16 port)
{
  if (!m_server.listen(QHostAddress::Any, port)) {
    emit statusMessage(QStringLiteral("Unable to listen on port %1: %2")
                         .arg(port)
                         .arg(m_server.errorString()));
    return false;
  }

  emit statusMessage(QStringLiteral("WebSocket server listening on port %1").arg(port));
  return true;
}

void FlightServer::stop()
{
  for (QWebSocket *client : std::as_const(m_clients)) {
    client->close();
  }
  m_clients.clear();
  m_server.close();
}

bool FlightServer::isListening() const
{
  return m_server.isListening();
}

void FlightServer::handleNewConnection()
{
  QWebSocket *client = m_server.nextPendingConnection();
  if (client == nullptr) {
    return;
  }

  m_clients.append(client);
  connect(client, &QWebSocket::disconnected, this, &FlightServer::handleDisconnected);
  client->sendTextMessage(serializedFlights());
  emit statusMessage(QStringLiteral("Client connected: %1").arg(client->peerAddress().toString()));
}

void FlightServer::handleDisconnected()
{
  auto *client = qobject_cast<QWebSocket *>(sender());
  if (client == nullptr) {
    return;
  }

  m_clients.removeAll(client);
  emit statusMessage(QStringLiteral("Client disconnected"));
  client->deleteLater();
}

void FlightServer::broadcastFlights()
{
  const QString message = serializedFlights();
  for (QWebSocket *client : std::as_const(m_clients)) {
    if (client->isValid()) {
      client->sendTextMessage(message);
    }
  }
}

QString FlightServer::serializedFlights() const
{
  QJsonArray flights;
  for (const Flight &flight : m_simulator->flights()) {
    flights.append(flight.toJson());
  }
  return QString::fromUtf8(QJsonDocument(flights).toJson(QJsonDocument::Compact));
}