#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include "Flight.h"

class FlightSimulator : public QObject {
    Q_OBJECT
public:
    explicit FlightSimulator(QObject *parent = nullptr);

    void start(int intervalMs = 1000);

    const QVector<Flight> &flights() const;

public slots:
    void tick();

signals:
    void flightsUpdated();

private:
    void seedFlights();
    void updateFlight(int index);
    void printFlights() const;

    QVector<Flight> m_flights;
    QVector<double> m_cruiseAltitudesFt;
    QTimer m_timer;
    int m_tickCount = 0;
};
