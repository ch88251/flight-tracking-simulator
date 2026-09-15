#include "MainWindow.h"

#include "FlightClient.h"
#include "FlightTableModel.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QTableView>
#include <QTabWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

namespace {
const QUrl kServerUrl(QStringLiteral("ws://127.0.0.1:4500"));
}

MainWindow::MainWindow(FlightClient *client, QWidget *parent)
  : QMainWindow(parent)
  , m_client(client)
  , m_flightModel(new FlightTableModel(this))
{
  auto *centralWidget = new QWidget(this);
  auto *layout = new QVBoxLayout(centralWidget);
  auto *toolbar = new QHBoxLayout;
  auto *reconnectButton = new QPushButton(QStringLiteral("Reconnect"), centralWidget);

  m_statusLabel = new QLabel(QStringLiteral("Status: Starting..."), centralWidget);

  m_flightsTable = new QTableView(centralWidget);
  m_flightsTable->setModel(m_flightModel);
  m_flightsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_flightsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_flightsTable->setAlternatingRowColors(true);
  m_flightsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_flightsTable->verticalHeader()->setVisible(false);

  m_mapView = new QQuickWidget(centralWidget);
  m_mapView->setResizeMode(QQuickWidget::SizeRootObjectToView);
  m_mapView->rootContext()->setContextProperty(QStringLiteral("flightModel"), m_flightModel);
  m_mapView->setSource(QUrl(QStringLiteral("qrc:/qml/FlightMap.qml")));

  auto *tabs = new QTabWidget(centralWidget);
  tabs->addTab(m_flightsTable, QStringLiteral("Table"));
  tabs->addTab(m_mapView, QStringLiteral("Map"));

  toolbar->addWidget(m_statusLabel, 1);
  toolbar->addWidget(reconnectButton);
  layout->addLayout(toolbar);
  layout->addWidget(tabs);
  setCentralWidget(centralWidget);
  setWindowTitle(QStringLiteral("Flight Tracking Simulator"));
  resize(1100, 640);

  connect(reconnectButton, &QPushButton::clicked, this, &MainWindow::reconnect);
  connect(m_client, &FlightClient::statusMessage, this, &MainWindow::updateStatus);
  connect(m_client, &FlightClient::flightsUpdated, m_flightModel, &FlightTableModel::setFlights);
}

void MainWindow::updateStatus(const QString &message)
{
  m_statusLabel->setText(QStringLiteral("Status: %1").arg(message));
}

void MainWindow::reconnect()
{
  m_client->connectToServer(kServerUrl);
}
