#pragma once

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QVector>

#include "Flight.h"

class FlightTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum Column {
		CallsignColumn = 0,
		OriginColumn,
		DestinationColumn,
		StatusColumn,
		LatitudeColumn,
		LongitudeColumn,
		AltitudeColumn,
		HeadingColumn,
		SpeedColumn,
		ColumnCount
	};

	enum Role {
		IdRole = Qt::UserRole + 1,
		CallsignRole,
		OriginRole,
		DestinationRole,
		StatusRole,
		LatitudeRole,
		LongitudeRole,
		AltitudeFtRole,
		HeadingDegRole,
		GroundSpeedKtsRole
	};
	Q_ENUM(Role)

	explicit FlightTableModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

public slots:
	void setFlights(const QJsonArray &flights);

private:
	QVariant flightRoleData(const Flight &flight, int role) const;

	QVector<Flight> m_flights;
};
