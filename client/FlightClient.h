#pragma once

#include <QJsonArray>
#include <QObject>
#include <QUrl>
#include <QWebSocket>

class FlightClient : public QObject
{
	Q_OBJECT

public:
	explicit FlightClient(QObject *parent = nullptr);

	void connectToServer(const QUrl &url);
	void disconnectFromServer();
	bool isConnected() const;

signals:
	void statusMessage(const QString &message);
	void flightsUpdated(const QJsonArray &flights);

private slots:
	void handleConnected();
	void handleDisconnected();
	void handleMessage(const QString &message);
	void handleSocketError(QAbstractSocket::SocketError error);

private:
	QWebSocket m_socket;
};
