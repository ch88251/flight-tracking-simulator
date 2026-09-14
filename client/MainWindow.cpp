#include "MainWindow.h"

#include "FlightClient.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace {
const QUrl kServerUrl(QStringLiteral("ws://127.0.0.1:4500"));
const QStringList kColumnNames = {
  QStringLiteral("Callsign"), QStringLiteral("Origin"), QStringLiteral("Destination"),
  QStringLiteral("Status"), QStringLiteral("Latitude"), QStringLiteral("Longitude"),
  QStringLiteral("Altitude (ft)"), QStringLiteral("Heading"), QStringLiteral("Speed (kt)")
};
}

MainWindow::MainWindow(FlightClient *client, QWidget *parent)
  : QMainWindow(parent)
  , m_client(client)
{
  auto *centralWidget = new QWidget(this);
  auto *layout = new QVBoxLayout(centralWidget);
  auto *toolbar = new QHBoxLayout;
  auto *reconnectButton = new QPushButton(QStringLiteral("Reconnect"), centralWidget);

  m_statusLabel = new QLabel(QStringLiteral("Status: Starting..."), centralWidget);
  m_flightsTable = new QTableWidget(centralWidget);
  m_flightsTable->setColumnCount(kColumnNames.size());
  m_flightsTable->setHorizontalHeaderLabels(kColumnNames);
  m_flightsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_flightsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_flightsTable->setAlternatingRowColors(true);
  m_flightsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_flightsTable->verticalHeader()->setVisible(false);

  toolbar->addWidget(m_statusLabel, 1);
  toolbar->addWidget(reconnectButton);
  layout->addLayout(toolbar);
  layout->addWidget(m_flightsTable);
  setCentralWidget(centralWidget);
  setWindowTitle(QStringLiteral("Flight Tracking Simulator"));
  resize(1100, 480);

  connect(reconnectButton, &QPushButton::clicked, this, &MainWindow::reconnect);
  connect(m_client, &FlightClient::statusMessage, this, &MainWindow::updateStatus);
  connect(m_client, &FlightClient::flightsUpdated, this, &MainWindow::updateFlights);
}

void MainWindow::updateStatus(const QString &message)
{
  m_statusLabel->setText(QStringLiteral("Status: %1").arg(message));
}

void MainWindow::updateFlights(const QJsonArray &flights)
{
  m_flightsTable->setRowCount(flights.size());

  for (int row = 0; row < flights.size(); ++row) {
    const QJsonObject flight = flights.at(row).toObject();
    const QStringList values = {
      flight.value(QStringLiteral("callsign")).toString(),
      flight.value(QStringLiteral("origin")).toString(),
      flight.value(QStringLiteral("destination")).toString(),
      flight.value(QStringLiteral("status")).toString(),
      QString::number(flight.value(QStringLiteral("latitude")).toDouble(), 'f', 4),
      QString::number(flight.value(QStringLiteral("longitude")).toDouble(), 'f', 4),
      QString::number(flight.value(QStringLiteral("altitudeFt")).toDouble(), 'f', 0),
      QString::number(flight.value(QStringLiteral("headingDeg")).toDouble(), 'f', 1),
      QString::number(flight.value(QStringLiteral("groundSpeedKts")).toDouble(), 'f', 1)
    };

    for (int column = 0; column < values.size(); ++column) {
      m_flightsTable->setItem(row, column, new QTableWidgetItem(values.at(column)));
    }
  }
}

void MainWindow::reconnect()
{
  m_client->connectToServer(kServerUrl);
}