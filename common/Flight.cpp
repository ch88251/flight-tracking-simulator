#include "Flight.h"


Flight::Flight(QString id, QString callsign, QString origin, QString destination)
  : m_id(std::move(id))
  , m_callsign(std::move(callsign))
  , m_origin(std::move(origin))
  , m_destination(std::move(destination)) {}

QString Flight::id() const { return m_id; }
void Flight::setId(const QString &id) { m_id = id; }

QString Flight::callsign() const { return m_callsign; }
void Flight::setCallsign(const QString &callsign) { m_callsign = callsign; }

QString Flight::origin() const { return m_origin; }
void Flight::setOrigin(const QString &origin) { m_origin = origin; }

QString Flight::destination() const { return m_destination; }
void Flight::setDestination(const QString &destination) { m_destination = destination; }

double Flight::latitude() const { return m_latitude; }
void Flight::setLatitude(double latitude) { m_latitude = latitude; }

double Flight::longitude() const { return m_longitude; }
void Flight::setLongitude(double longitude) { m_longitude = longitude; }

double Flight::altitudeFt() const { return m_altitudeFt; }
void Flight::setAltitudeFt(double altitudeFt) { m_altitudeFt = altitudeFt; }

double Flight::headingDeg() const { return m_headingDeg; }
void Flight::setHeadingDeg(double headingDeg) { m_headingDeg = headingDeg; }

double Flight::groundSpeedKts() const { return m_groundSpeedKts; }
void Flight::setGroundSpeedKts(double groundSpeedKts) { m_groundSpeedKts = groundSpeedKts; }

FlightStatus Flight::status() const { return m_status; }
void Flight::setStatus(const FlightStatus &status) { m_status = status; }

QJsonObject Flight::toJson() const
{
  QJsonObject json;
  json["id"] = m_id;
  json["callsign"] = m_callsign;
  json["origin"] = m_origin;
  json["destination"] = m_destination;
  json["latitude"] = m_latitude;
  json["longitude"] = m_longitude;
  json["altitudeFt"] = m_altitudeFt;
  json["headingDeg"] = m_headingDeg;
  json["groundSpeedKts"] = m_groundSpeedKts;
  json["status"] = flightStatusToString(m_status);
  return json;
}

Flight Flight::fromJson(const QJsonObject &json)
{
  Flight flight;
  flight.setId(json.value("id").toString());
  flight.setCallsign(json.value("callsign").toString());
  flight.setOrigin(json.value("origin").toString());
  flight.setDestination(json.value("destination").toString());
  flight.setLatitude(json.value("latitude").toDouble());
  flight.setLongitude(json.value("longitude").toDouble());
  flight.setAltitudeFt(json.value("altitudeFt").toDouble());
  flight.setHeadingDeg(json.value("headingDeg").toDouble());
  flight.setGroundSpeedKts(json.value("groundSpeedKts").toDouble());
  flight.setStatus(flightStatusFromString(json.value("status").toString()));
  return flight;
}
