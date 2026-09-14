#include <gtest/gtest.h>

#include "FlightSimulator.h"

namespace {

constexpr int kMaxTicksToObserveTransition = 5000;

int findFlightIndex(const FlightSimulator &sim, const QString &callsign)
{
    const QVector<Flight> &flights = sim.flights();
    for (int i = 0; i < flights.size(); ++i) {
        if (flights.at(i).callsign() == callsign) {
            return i;
        }
    }
    return -1;
}

} // namespace

TEST(FlightSimulatorTest, SeedsFiveFlightsWithExpectedCallsigns)
{
    FlightSimulator sim;

    ASSERT_EQ(sim.flights().size(), 5);
    EXPECT_NE(findFlightIndex(sim, QStringLiteral("UAL123")), -1);
    EXPECT_NE(findFlightIndex(sim, QStringLiteral("DAL456")), -1);
    EXPECT_NE(findFlightIndex(sim, QStringLiteral("SWA789")), -1);
    EXPECT_NE(findFlightIndex(sim, QStringLiteral("AAL234")), -1);
    EXPECT_NE(findFlightIndex(sim, QStringLiteral("JBU567")), -1);
}

TEST(FlightSimulatorTest, SeededFlightsStartWithExpectedStatuses)
{
    FlightSimulator sim;

    EXPECT_EQ(sim.flights().at(findFlightIndex(sim, QStringLiteral("UAL123"))).status(), FlightStatus::Climbing);
    EXPECT_EQ(sim.flights().at(findFlightIndex(sim, QStringLiteral("DAL456"))).status(), FlightStatus::Cruising);
    EXPECT_EQ(sim.flights().at(findFlightIndex(sim, QStringLiteral("SWA789"))).status(), FlightStatus::Cruising);
    EXPECT_EQ(sim.flights().at(findFlightIndex(sim, QStringLiteral("AAL234"))).status(), FlightStatus::Descending);
    EXPECT_EQ(sim.flights().at(findFlightIndex(sim, QStringLiteral("JBU567"))).status(), FlightStatus::TakingOff);
}

TEST(FlightSimulatorTest, TakingOffClimbsUntilThreeThousandFeetThenSwitchesToClimbing)
{
    FlightSimulator sim;
    const int index = findFlightIndex(sim, QStringLiteral("JBU567"));
    ASSERT_NE(index, -1);
    ASSERT_EQ(sim.flights().at(index).status(), FlightStatus::TakingOff);

    double previousAltitude = sim.flights().at(index).altitudeFt();
    int ticks = 0;
    while (sim.flights().at(index).status() == FlightStatus::TakingOff) {
        ASSERT_LT(ticks++, kMaxTicksToObserveTransition) << "TakingOff never transitioned to Climbing";
        sim.tick();
        EXPECT_GT(sim.flights().at(index).altitudeFt(), previousAltitude);
        previousAltitude = sim.flights().at(index).altitudeFt();
    }

    EXPECT_EQ(sim.flights().at(index).status(), FlightStatus::Climbing);
    EXPECT_GE(sim.flights().at(index).altitudeFt(), 3000.0);
}

TEST(FlightSimulatorTest, ClimbingLevelsOffExactlyAtCruiseAltitude)
{
    FlightSimulator sim;
    const int index = findFlightIndex(sim, QStringLiteral("UAL123"));
    ASSERT_NE(index, -1);
    ASSERT_EQ(sim.flights().at(index).status(), FlightStatus::Climbing);

    int ticks = 0;
    while (sim.flights().at(index).status() == FlightStatus::Climbing) {
        ASSERT_LT(ticks++, kMaxTicksToObserveTransition) << "Climbing never transitioned to Cruising";
        sim.tick();
    }

    EXPECT_EQ(sim.flights().at(index).status(), FlightStatus::Cruising);
    EXPECT_DOUBLE_EQ(sim.flights().at(index).altitudeFt(), 36000.0);
}

TEST(FlightSimulatorTest, DescendingLandsAtZeroFeetAndZeroesGroundSpeed)
{
    FlightSimulator sim;
    const int index = findFlightIndex(sim, QStringLiteral("AAL234"));
    ASSERT_NE(index, -1);
    ASSERT_EQ(sim.flights().at(index).status(), FlightStatus::Descending);

    int ticks = 0;
    while (sim.flights().at(index).status() == FlightStatus::Descending) {
        ASSERT_LT(ticks++, kMaxTicksToObserveTransition) << "Descending never transitioned to Landed";
        sim.tick();
    }

    EXPECT_EQ(sim.flights().at(index).status(), FlightStatus::Landed);
    EXPECT_DOUBLE_EQ(sim.flights().at(index).altitudeFt(), 0.0);
    EXPECT_DOUBLE_EQ(sim.flights().at(index).groundSpeedKts(), 0.0);
}

TEST(FlightSimulatorTest, LandedFlightIsFrozenOnSubsequentTicks)
{
    FlightSimulator sim;
    const int index = findFlightIndex(sim, QStringLiteral("AAL234"));
    ASSERT_NE(index, -1);

    int ticks = 0;
    while (sim.flights().at(index).status() != FlightStatus::Landed) {
        ASSERT_LT(ticks++, kMaxTicksToObserveTransition) << "Descending never transitioned to Landed";
        sim.tick();
    }

    const Flight landed = sim.flights().at(index);
    sim.tick();
    sim.tick();
    const Flight &stillLanded = sim.flights().at(index);

    EXPECT_DOUBLE_EQ(stillLanded.altitudeFt(), landed.altitudeFt());
    EXPECT_DOUBLE_EQ(stillLanded.headingDeg(), landed.headingDeg());
    EXPECT_DOUBLE_EQ(stillLanded.groundSpeedKts(), landed.groundSpeedKts());
    EXPECT_DOUBLE_EQ(stillLanded.latitude(), landed.latitude());
    EXPECT_DOUBLE_EQ(stillLanded.longitude(), landed.longitude());
}

TEST(FlightSimulatorTest, CruisingFlightHoldsAltitudeWhileHeadingAndSpeedStayWithinBounds)
{
    FlightSimulator sim;
    const int index = findFlightIndex(sim, QStringLiteral("DAL456"));
    ASSERT_NE(index, -1);
    const double cruiseAltitude = sim.flights().at(index).altitudeFt();

    for (int i = 0; i < 20; ++i) {
        sim.tick();
        const Flight &flight = sim.flights().at(index);
        EXPECT_DOUBLE_EQ(flight.altitudeFt(), cruiseAltitude);
        EXPECT_GE(flight.headingDeg(), 0.0);
        EXPECT_LT(flight.headingDeg(), 360.0);
        EXPECT_GE(flight.groundSpeedKts(), 150.0);
        EXPECT_LE(flight.groundSpeedKts(), 550.0);
    }
}
