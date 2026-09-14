#include "FlightSimulator.h"

#include <QRandomGenerator>
#include <QTextStream>
#include <QtMath>

namespace {
constexpr double kEarthNmPerDegreeLat = 60.0;
constexpr double kClimbRateFtPerSec = 15.0;
constexpr double kDescentRateFtPerSec = 12.0;
constexpr double kMaxHeadingJitterDeg = 1.5;
constexpr double kMaxSpeedJitterKts = 3.0;
constexpr double kMinSpeedKts = 150.0;
constexpr double kMaxSpeedKts = 550.0;

double jitter(double magnitude)
{
  return (QRandomGenerator::global()->generateDouble() * 2.0 - 1.0) * magnitude;
}
}

FlightSimulator::FlightSimulator(QObject *parent)
  : QObject(parent)
{
  seedFlights();
  connect(&m_timer, &QTimer::timeout, this, &FlightSimulator::tick);
}

void FlightSimulator::start(int intervalMs)
{
  m_timer.start(intervalMs);
}

const QVector<Flight> &FlightSimulator::flights() const
{
  return m_flights;
}

void FlightSimulator::seedFlights()
{
  struct Seed {
    const char *id;
    const char *callsign;
    const char *origin;
    const char *destination;
    double lat;
    double lon;
    double altitudeFt;
    double headingDeg;
    double speedKts;
    FlightStatus status;
    double cruiseAltitudeFt;
  };

  const Seed seeds[] = {
    {"f1", "UAL123", "KSFO", "KJFK", 37.6213, -122.3790, 5000.0, 75.0, 280.0, FlightStatus::Climbing, 36000.0},
    {"f2", "DAL456", "KORD", "KATL", 41.9742, -87.9073, 34000.0, 150.0, 460.0, FlightStatus::Cruising, 34000.0},
    {"f3", "SWA789", "KDEN", "KLAS", 39.8561, -104.6737, 38000.0, 240.0, 430.0, FlightStatus::Cruising, 38000.0},
    {"f4", "AAL234", "KDFW", "KMIA", 32.8998, -97.0403, 12000.0, 105.0, 320.0, FlightStatus::Descending, 12000.0},
    {"f5", "JBU567", "KBOS", "KSEA", 42.3656, -71.0096, 1500.0, 290.0, 210.0, FlightStatus::TakingOff, 33000.0},
  };

  for (const Seed &seed : seeds) {
    Flight flight(QString::fromLatin1(seed.id), QString::fromLatin1(seed.callsign),
                  QString::fromLatin1(seed.origin), QString::fromLatin1(seed.destination));
    flight.setLatitude(seed.lat);
    flight.setLongitude(seed.lon);
    flight.setAltitudeFt(seed.altitudeFt);
    flight.setHeadingDeg(seed.headingDeg);
    flight.setGroundSpeedKts(seed.speedKts);
    flight.setStatus(seed.status);
    m_flights.append(flight);
    m_cruiseAltitudesFt.append(seed.cruiseAltitudeFt);
  }
}

void FlightSimulator::updateFlight(int index)
{
  Flight &flight = m_flights[index];
  const double cruiseAltitudeFt = m_cruiseAltitudesFt.at(index);

  if (flight.status() == FlightStatus::Landed) {
    return;
  }

  switch (flight.status()) {
    case FlightStatus::TakingOff:
      flight.setAltitudeFt(flight.altitudeFt() + kClimbRateFtPerSec);
      if (flight.altitudeFt() >= 3000.0) {
        flight.setStatus(FlightStatus::Climbing);
      }
      break;
    case FlightStatus::Climbing:
      flight.setAltitudeFt(flight.altitudeFt() + kClimbRateFtPerSec);
      if (flight.altitudeFt() >= cruiseAltitudeFt) {
        flight.setAltitudeFt(cruiseAltitudeFt);
        flight.setStatus(FlightStatus::Cruising);
      }
      break;
    case FlightStatus::Descending:
      flight.setAltitudeFt(flight.altitudeFt() - kDescentRateFtPerSec);
      if (flight.altitudeFt() <= 0.0) {
        flight.setAltitudeFt(0.0);
        flight.setGroundSpeedKts(0.0);
        flight.setStatus(FlightStatus::Landed);
        return;
      }
      break;
    default:
      break;
  }

  double heading = flight.headingDeg() + jitter(kMaxHeadingJitterDeg);
  heading = std::fmod(heading + 360.0, 360.0);
  flight.setHeadingDeg(heading);

  double speed = flight.groundSpeedKts() + jitter(kMaxSpeedJitterKts);
  speed = qBound(kMinSpeedKts, speed, kMaxSpeedKts);
  flight.setGroundSpeedKts(speed);

  const double distanceNm = speed / 3600.0;
  const double headingRad = qDegreesToRadians(heading);
  const double latRad = qDegreesToRadians(flight.latitude());

  flight.setLatitude(flight.latitude() + (distanceNm / kEarthNmPerDegreeLat) * std::cos(headingRad));
  flight.setLongitude(flight.longitude()
                       + (distanceNm / (kEarthNmPerDegreeLat * std::cos(latRad))) * std::sin(headingRad));
}

void FlightSimulator::tick()
{
  ++m_tickCount;
  for (int i = 0; i < m_flights.size(); ++i) {
    updateFlight(i);
  }
  emit flightsUpdated();
  printFlights();
}

void FlightSimulator::printFlights() const
{
  QTextStream out(stdout);
  out << "\n=== Tick " << m_tickCount << " ===\n";
  out << qSetFieldWidth(9) << "Flight" << qSetFieldWidth(13) << "Status" << qSetFieldWidth(12) << "Lat"
      << qSetFieldWidth(12) << "Lon" << qSetFieldWidth(10) << "Alt(ft)" << qSetFieldWidth(9) << "Hdg"
      << qSetFieldWidth(9) << "Spd(kt)" << qSetFieldWidth(0) << "\n";

  for (const Flight &flight : m_flights) {
    out << qSetFieldWidth(9) << flight.callsign() << qSetFieldWidth(13) << flightStatusToString(flight.status())
        << qSetFieldWidth(12) << QString::number(flight.latitude(), 'f', 4) << qSetFieldWidth(12)
        << QString::number(flight.longitude(), 'f', 4) << qSetFieldWidth(10)
        << QString::number(flight.altitudeFt(), 'f', 0) << qSetFieldWidth(9)
        << QString::number(flight.headingDeg(), 'f', 1) << qSetFieldWidth(9)
        << QString::number(flight.groundSpeedKts(), 'f', 1) << qSetFieldWidth(0) << "\n";
  }
  out.flush();
}
