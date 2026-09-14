#pragma once

#include <QString>

enum class FlightStatus {
    Scheduled,
    Boarding,
    Departed,
    EnRoute,
    Landed,
    Delayed,
    Cancelled
};

inline QString flightStatusToString(FlightStatus status)
{
  switch(status) {
    case FlightStatus::Scheduled: return QStringLiteral("Scheduled");
    case FlightStatus::Boarding: return QStringLiteral("Boarding");
    case FlightStatus::Departed: return QStringLiteral("Departed");
    case FlightStatus::EnRoute: return QStringLiteral("EnRoute");
    case FlightStatus::Landed: return QStringLiteral("Landed");
    case FlightStatus::Delayed: return QStringLiteral("Delayed");
    case FlightStatus::Cancelled: return QStringLiteral("Cancelled");
  }

  return QStringLiteral("Unknown");
}

inline FlightStatus flightStatusFromString(const QString &value)
{
  if (value == QStringLiteral("Boarding")) return FlightStatus::Boarding;
  if (value == QStringLiteral("Departing")) return FlightStatus::Departing;
  if (value == QStringLiteral("EnRoute")) return FlightStatus::EnRoute;
  if (value == QStringLiteral("Landed")) return FlightStatus::Landed;
  if (value == QStringLiteral("Delayed")) return FlightStatus::Delayed;
  if (value == QStringLiteral("Cancelled")) return FlightStatus::Cancelled;

  return FlightStatus::Scheduled;
}
