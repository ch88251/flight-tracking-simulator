#pragma once

#include <QJsonArray>
#include <QMainWindow>

class FlightClient;
class QLabel;
class QTableWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(FlightClient *client, QWidget *parent = nullptr);

private slots:
  void updateStatus(const QString &message);
  void updateFlights(const QJsonArray &flights);
  void reconnect();

private:
  FlightClient *m_client = nullptr;
  QLabel *m_statusLabel = nullptr;
  QTableWidget *m_flightsTable = nullptr;
};