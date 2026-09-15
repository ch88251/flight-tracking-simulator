#include "FlightTableModel.h"

namespace {
const QStringList kColumnNames = {
	QStringLiteral("Callsign"), QStringLiteral("Origin"), QStringLiteral("Destination"),
	QStringLiteral("Status"), QStringLiteral("Latitude"), QStringLiteral("Longitude"),
	QStringLiteral("Altitude (ft)"), QStringLiteral("Heading"), QStringLiteral("Speed (kt)")
};
}

FlightTableModel::FlightTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

int FlightTableModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return 0;
	}
	return m_flights.size();
}

int FlightTableModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return 0;
	}
	return ColumnCount;
}

QVariant FlightTableModel::flightRoleData(const Flight &flight, int role) const
{
	switch (role) {
	case IdRole: return flight.id();
	case CallsignRole: return flight.callsign();
	case OriginRole: return flight.origin();
	case DestinationRole: return flight.destination();
	case StatusRole: return flightStatusToString(flight.status());
	case LatitudeRole: return flight.latitude();
	case LongitudeRole: return flight.longitude();
	case AltitudeFtRole: return flight.altitudeFt();
	case HeadingDegRole: return flight.headingDeg();
	case GroundSpeedKtsRole: return flight.groundSpeedKts();
	default: return QVariant();
	}
}

QVariant FlightTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() < 0 || index.row() >= m_flights.size()) {
		return QVariant();
	}

	const Flight &flight = m_flights.at(index.row());

	if (role >= IdRole) {
		return flightRoleData(flight, role);
	}

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case CallsignColumn: return flight.callsign();
		case OriginColumn: return flight.origin();
		case DestinationColumn: return flight.destination();
		case StatusColumn: return flightStatusToString(flight.status());
		case LatitudeColumn: return QString::number(flight.latitude(), 'f', 4);
		case LongitudeColumn: return QString::number(flight.longitude(), 'f', 4);
		case AltitudeColumn: return QString::number(flight.altitudeFt(), 'f', 0);
		case HeadingColumn: return QString::number(flight.headingDeg(), 'f', 1);
		case SpeedColumn: return QString::number(flight.groundSpeedKts(), 'f', 1);
		default: return QVariant();
		}
	}

	return QVariant();
}

QVariant FlightTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
		return QAbstractTableModel::headerData(section, orientation, role);
	}

	if (section < 0 || section >= kColumnNames.size()) {
		return QVariant();
	}

	return kColumnNames.at(section);
}

QHash<int, QByteArray> FlightTableModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[IdRole] = "flightId";
	roles[CallsignRole] = "callsign";
	roles[OriginRole] = "origin";
	roles[DestinationRole] = "destination";
	roles[StatusRole] = "status";
	roles[LatitudeRole] = "latitude";
	roles[LongitudeRole] = "longitude";
	roles[AltitudeFtRole] = "altitudeFt";
	roles[HeadingDegRole] = "headingDeg";
	roles[GroundSpeedKtsRole] = "groundSpeedKts";
	return roles;
}

void FlightTableModel::setFlights(const QJsonArray &flights)
{
	QVector<Flight> updated;
	updated.reserve(flights.size());
	for (const QJsonValue &value : flights) {
		updated.append(Flight::fromJson(value.toObject()));
	}

	beginResetModel();
	m_flights = std::move(updated);
	endResetModel();
}
