#include <gtest/gtest.h>

#include "Flight.h"

TEST(FlightTest, ConstructorSetsIdentifyingFields)
{
    Flight flight(QStringLiteral("f1"), QStringLiteral("UAL123"),
                  QStringLiteral("KSFO"), QStringLiteral("KJFK"));

    EXPECT_EQ(flight.id(), QStringLiteral("f1"));
    EXPECT_EQ(flight.callsign(), QStringLiteral("UAL123"));
    EXPECT_EQ(flight.origin(), QStringLiteral("KSFO"));
    EXPECT_EQ(flight.destination(), QStringLiteral("KJFK"));
}

TEST(FlightTest, DefaultConstructedFlightHasScheduledStatusAndZeroedTelemetry)
{
    Flight flight;

    EXPECT_EQ(flight.status(), FlightStatus::Scheduled);
    EXPECT_DOUBLE_EQ(flight.latitude(), 0.0);
    EXPECT_DOUBLE_EQ(flight.longitude(), 0.0);
    EXPECT_DOUBLE_EQ(flight.altitudeFt(), 0.0);
    EXPECT_DOUBLE_EQ(flight.headingDeg(), 0.0);
    EXPECT_DOUBLE_EQ(flight.groundSpeedKts(), 0.0);
}

TEST(FlightTest, SettersUpdateTelemetryFields)
{
    Flight flight;

    flight.setLatitude(37.6213);
    flight.setLongitude(-122.3790);
    flight.setAltitudeFt(35000.0);
    flight.setHeadingDeg(270.0);
    flight.setGroundSpeedKts(450.0);
    flight.setStatus(FlightStatus::Cruising);

    EXPECT_DOUBLE_EQ(flight.latitude(), 37.6213);
    EXPECT_DOUBLE_EQ(flight.longitude(), -122.3790);
    EXPECT_DOUBLE_EQ(flight.altitudeFt(), 35000.0);
    EXPECT_DOUBLE_EQ(flight.headingDeg(), 270.0);
    EXPECT_DOUBLE_EQ(flight.groundSpeedKts(), 450.0);
    EXPECT_EQ(flight.status(), FlightStatus::Cruising);
}

TEST(FlightTest, ToJsonSerializesAllFields)
{
    Flight flight(QStringLiteral("f1"), QStringLiteral("UAL123"),
                  QStringLiteral("KSFO"), QStringLiteral("KJFK"));
    flight.setLatitude(37.6213);
    flight.setLongitude(-122.3790);
    flight.setAltitudeFt(35000.0);
    flight.setHeadingDeg(270.0);
    flight.setGroundSpeedKts(450.0);
    flight.setStatus(FlightStatus::Cruising);

    const QJsonObject json = flight.toJson();

    EXPECT_EQ(json.value("id").toString(), QStringLiteral("f1"));
    EXPECT_EQ(json.value("callsign").toString(), QStringLiteral("UAL123"));
    EXPECT_EQ(json.value("origin").toString(), QStringLiteral("KSFO"));
    EXPECT_EQ(json.value("destination").toString(), QStringLiteral("KJFK"));
    EXPECT_DOUBLE_EQ(json.value("latitude").toDouble(), 37.6213);
    EXPECT_DOUBLE_EQ(json.value("longitude").toDouble(), -122.3790);
    EXPECT_DOUBLE_EQ(json.value("altitudeFt").toDouble(), 35000.0);
    EXPECT_DOUBLE_EQ(json.value("headingDeg").toDouble(), 270.0);
    EXPECT_DOUBLE_EQ(json.value("groundSpeedKts").toDouble(), 450.0);
    EXPECT_EQ(json.value("status").toString(), QStringLiteral("Cruising"));
}

TEST(FlightTest, FromJsonRoundTripsThroughToJson)
{
    Flight original(QStringLiteral("f1"), QStringLiteral("UAL123"),
                     QStringLiteral("KSFO"), QStringLiteral("KJFK"));
    original.setLatitude(37.6213);
    original.setLongitude(-122.3790);
    original.setAltitudeFt(35000.0);
    original.setHeadingDeg(270.0);
    original.setGroundSpeedKts(450.0);
    original.setStatus(FlightStatus::Cruising);

    const Flight restored = Flight::fromJson(original.toJson());

    EXPECT_EQ(restored.id(), original.id());
    EXPECT_EQ(restored.callsign(), original.callsign());
    EXPECT_EQ(restored.origin(), original.origin());
    EXPECT_EQ(restored.destination(), original.destination());
    EXPECT_DOUBLE_EQ(restored.latitude(), original.latitude());
    EXPECT_DOUBLE_EQ(restored.longitude(), original.longitude());
    EXPECT_DOUBLE_EQ(restored.altitudeFt(), original.altitudeFt());
    EXPECT_DOUBLE_EQ(restored.headingDeg(), original.headingDeg());
    EXPECT_DOUBLE_EQ(restored.groundSpeedKts(), original.groundSpeedKts());
    EXPECT_EQ(restored.status(), original.status());
}

TEST(FlightTest, FromJsonDefaultsMissingFieldsToScheduledAndZero)
{
    const Flight restored = Flight::fromJson(QJsonObject{});

    EXPECT_EQ(restored.status(), FlightStatus::Scheduled);
    EXPECT_DOUBLE_EQ(restored.latitude(), 0.0);
    EXPECT_DOUBLE_EQ(restored.longitude(), 0.0);
    EXPECT_DOUBLE_EQ(restored.altitudeFt(), 0.0);
    EXPECT_DOUBLE_EQ(restored.headingDeg(), 0.0);
    EXPECT_DOUBLE_EQ(restored.groundSpeedKts(), 0.0);
    EXPECT_TRUE(restored.id().isEmpty());
    EXPECT_TRUE(restored.callsign().isEmpty());
    EXPECT_TRUE(restored.origin().isEmpty());
    EXPECT_TRUE(restored.destination().isEmpty());
}
