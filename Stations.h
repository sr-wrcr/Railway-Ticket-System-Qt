#pragma once

#include <QString>
#include <iostream>

class Stations
{
private:
	int station_id;
	QString station_name;

public:
	Stations();
	Stations(int id, const QString& name);
	virtual ~Stations();

	int GetStationId() const;
	QString GetStationName() const;
	void SetStationId(int id);
	void SetStationName(const QString& name);

	friend std::istream& operator>>(std::istream& in, Stations& station);
	friend std::ostream& operator<<(std::ostream& out, const Stations& station);

};

