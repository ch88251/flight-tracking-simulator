#pragma once
#include <QString>
#include <QJsonObject>
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
    void setGroundSpeedKts(double groundSpeedKts);

    FlightStatus status() const;
    void setStatus(const FlightStatus &status);

    QJsonObject toJson() const;
    static Flight fromJson(const QJsonObject &json);

private:
    QString m_id;
    QString m_callsign;
    QString m_origin;
    QString m_destination;
    double m_latitude = 0.0;
    double m_longitude = 0.0;
    double m_altitudeFt = 0.0;
    double m_headingDeg = 0.0;
    double m_groundSpeedKts = 0.0;
    FlightStatus m_status = FlightStatus::Scheduled;
};
