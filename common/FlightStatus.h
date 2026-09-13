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