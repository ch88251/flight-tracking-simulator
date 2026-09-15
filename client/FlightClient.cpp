#include "FlightClient.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

FlightClient::FlightClient(QObject *parent)
	: QObject(parent)
{
	connect(&m_socket, &QWebSocket::connected, this, &FlightClient::handleConnected);
	connect(&m_socket, &QWebSocket::disconnected, this, &FlightClient::handleDisconnected);
	connect(&m_socket, &QWebSocket::textMessageReceived, this, &FlightClient::handleMessage);
	connect(&m_socket,
		static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
		this,
		&FlightClient::handleSocketError);
}

void FlightClient::connectToServer(const QUrl &url)
{
	if (m_socket.state() != QAbstractSocket::UnconnectedState) {
		m_socket.abort();
	}

	emit statusMessage(QStringLiteral("Connecting to %1").arg(url.toString()));
	m_socket.open(url);
}

void FlightClient::disconnectFromServer()
{
	m_socket.close();
}

bool FlightClient::isConnected() const
{
	return m_socket.state() == QAbstractSocket::ConnectedState;
}

void FlightClient::setSimulationSpeed(double multiplier)
{
	if (!isConnected()) {
		return;
	}

	QJsonObject command;
	command.insert(QStringLiteral("cmd"), QStringLiteral("setSpeed"));
	command.insert(QStringLiteral("multiplier"), multiplier);
	m_socket.sendTextMessage(QString::fromUtf8(QJsonDocument(command).toJson(QJsonDocument::Compact)));
}

void FlightClient::handleConnected()
{
	emit statusMessage(QStringLiteral("Connected to flight server"));
}

void FlightClient::handleDisconnected()
{
	emit statusMessage(QStringLiteral("Disconnected from flight server"));
}

void FlightClient::handleMessage(const QString &message)
{
	const QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
	if (!document.isArray()) {
		emit statusMessage(QStringLiteral("Received invalid flight update"));
		return;
	}

	emit flightsUpdated(document.array());
}

void FlightClient::handleSocketError(QAbstractSocket::SocketError error)
{
	Q_UNUSED(error);
	emit statusMessage(QStringLiteral("WebSocket error: %1").arg(m_socket.errorString()));
}
