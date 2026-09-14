#pragma once

#include <QString>

enum class FlightStatus {
    Scheduled,
    Taxiing,
    TakingOff,
    Climbing,
    Cruising,
    Descending,
    Landing,
    Landed
};

inline QString flightStatusToString(FlightStatus status)
{
  switch(status) {
    case FlightStatus::Scheduled: return QStringLiteral("Scheduled");
    case FlightStatus::Taxiing: return QStringLiteral("Taxiing");
    case FlightStatus::TakingOff: return QStringLiteral("TakingOff");
    case FlightStatus::Climbing: return QStringLiteral("Climbing");
    case FlightStatus::Cruising: return QStringLiteral("Cruising");
    case FlightStatus::Descending: return QStringLiteral("Descending");
    case FlightStatus::Landing: return QStringLiteral("Landing");
    case FlightStatus::Landed: return QStringLiteral("Landed");
  }

  return QStringLiteral("Unknown");
}

inline FlightStatus flightStatusFromString(const QString &value)
{
  if (value == QStringLiteral("Taxiing")) return FlightStatus::Taxiing;
  if (value == QStringLiteral("TakingOff")) return FlightStatus::TakingOff;
  if (value == QStringLiteral("Climbing")) return FlightStatus::Climbing;
  if (value == QStringLiteral("Cruising")) return FlightStatus::Cruising;
  if (value == QStringLiteral("Descending")) return FlightStatus::Descending;
  if (value == QStringLiteral("Landing")) return FlightStatus::Landing;
  if (value == QStringLiteral("Landed")) return FlightStatus::Landed;

  return FlightStatus::Scheduled;
}
