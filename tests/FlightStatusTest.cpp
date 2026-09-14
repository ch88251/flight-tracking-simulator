#include <gtest/gtest.h>

#include "FlightStatus.h"

TEST(FlightStatusTest, ToStringMapsKnownValues)
{
    EXPECT_EQ(flightStatusToString(FlightStatus::Scheduled), "Scheduled");
    EXPECT_EQ(flightStatusToString(FlightStatus::Taxiing), "Taxiing");
    EXPECT_EQ(flightStatusToString(FlightStatus::TakingOff), "TakingOff");
    EXPECT_EQ(flightStatusToString(FlightStatus::Climbing), "Climbing");
    EXPECT_EQ(flightStatusToString(FlightStatus::Cruising), "Cruising");
    EXPECT_EQ(flightStatusToString(FlightStatus::Descending), "Descending");
    EXPECT_EQ(flightStatusToString(FlightStatus::Landing), "Landing");
    EXPECT_EQ(flightStatusToString(FlightStatus::Landed), "Landed");
}

TEST(FlightStatusTest, FromStringMapsKnownValues)
{
    EXPECT_EQ(flightStatusFromString("Taxiing"), FlightStatus::Taxiing);
    EXPECT_EQ(flightStatusFromString("TakingOff"), FlightStatus::TakingOff);
    EXPECT_EQ(flightStatusFromString("Climbing"), FlightStatus::Climbing);
    EXPECT_EQ(flightStatusFromString("Cruising"), FlightStatus::Cruising);
    EXPECT_EQ(flightStatusFromString("Descending"), FlightStatus::Descending);
    EXPECT_EQ(flightStatusFromString("Landing"), FlightStatus::Landing);
    EXPECT_EQ(flightStatusFromString("Landed"), FlightStatus::Landed);
}

TEST(FlightStatusTest, FromStringFallsBackToScheduledForUnknownInput)
{
    EXPECT_EQ(flightStatusFromString("not-a-status"), FlightStatus::Scheduled);
    EXPECT_EQ(flightStatusFromString(""), FlightStatus::Scheduled);
}

TEST(FlightStatusTest, RoundTripsThroughStringForEveryValue)
{
    const FlightStatus statuses[] = {
        FlightStatus::Scheduled,
        FlightStatus::Taxiing,
        FlightStatus::TakingOff,
        FlightStatus::Climbing,
        FlightStatus::Cruising,
        FlightStatus::Descending,
        FlightStatus::Landing,
        FlightStatus::Landed,
    };

    for (const auto status : statuses) {
        EXPECT_EQ(flightStatusFromString(flightStatusToString(status)), status);
    }
}
