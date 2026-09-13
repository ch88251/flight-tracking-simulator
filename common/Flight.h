#pragma once
#include <QString>
#include "FlightStatus.h"

class Flight {
public:
    Flight() = default;
    Flight(QString id, QString callsign, QString origin, QString destination);

    QString id() const;
    void setId(const QString &id);

    QString callsign() const;
    void setCallsign(const QString &callsign);

    QString origin() const;
    void setOrigin(const QString &origin);

    QString destination() const;
    void setDestination(const QString &destination);

    double latitude() const;
    void setLatitude(double latitude);

    double longitude() const;
    void setLongitude(double longitude);

    double altitudeFt() const;
    void setAltitudeFt(double altitudeFt);

    double headingDeg() const;
    void setHeadingDeg(double headingDeg);

    double groundSpeedKts() const;
    void setGroundSpeedKts(double groundSpeedKt);

    FlightStatus status() const;
    void setStatus(const FlightStatus &status);
};