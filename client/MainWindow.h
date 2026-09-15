#pragma once

#include <QMainWindow>

class FlightClient;
class FlightTableModel;
class QLabel;
class QTableView;
class QQuickWidget;
class QComboBox;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(FlightClient *client, QWidget *parent = nullptr);

private slots:
  void updateStatus(const QString &message);
  void reconnect();
  void changeSimulationSpeed(int index);

private:
  FlightClient *m_client = nullptr;
  FlightTableModel *m_flightModel = nullptr;
  QLabel *m_statusLabel = nullptr;
  QTableView *m_flightsTable = nullptr;
  QQuickWidget *m_mapView = nullptr;
  QComboBox *m_speedCombo = nullptr;
};
