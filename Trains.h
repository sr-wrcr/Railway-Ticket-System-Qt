#pragma once
#include <QString>
#include <iostream>

class Trains
{
private:
	int train_id;
	QString train_name;
	QString train_type;
	int start_station_id;
	int end_station_id;

public:
	Trains();
	Trains(int id, const QString& name, const QString& type, int startId, int endId);
	virtual ~Trains();

	int GetTrainId() const;
	QString GetTrainName() const;
	QString GetTrainType() const;
	int GetStartStationId() const;
	int GetEndStationId() const;

	void SetTrainId(int id);
	void SetTrainName(const QString& name);
	void SetTrainType(const QString& type);
	void SetStartStationId(int startId);
	void SetEndStationId(int endId);

	friend std::istream& operator>>(std::istream& in, Trains& train);
	friend std::ostream& operator<<(std::ostream& out, const Trains& train);
};

