#include "Flight.h"


Flight::Flight(QString id, QString callsign, QString origin, QString destination)
  : m_id(std::move(id))
  , m_callsign(std::move(callsign))
  , m_origin(std::move(origin))
  , m_destination(std::move(destination)) {}

QString Flight::id() const { return m_id; }
void Flight::setId(const QString &id) { m_id = id }
