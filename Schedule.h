#pragma once
#include <iostream>
#include <fstream>
#include <QTime>

class Schedule
{
private:
	int schedule_id;
	int train_id;
	int station_id;
	int stop_order;
	QTime arrival_time;
	QTime depart_time;

public:
	Schedule();
	Schedule(int scheduleId, int trainId, int stationId, int stopOrder, QTime arrivalTime, QTime departTime);
	virtual ~Schedule();
	
	int GetScheduleId() const;
	int GetTrainId() const;
	int GetStationId() const;
	int GetStopOrder() const;
	QTime GetArrivalTime() const;
	QTime GetDepartTime() const;

	void SetScheduleId(int scheduleId);
	void SetTrainId(int trainId);
	void SetStationId(int stationId);
	void SetStopOrder(int stopOrder);
	void SetArrivalTime(QTime arrivalTime);
	void SetDepartTime(QTime departTime);

	friend std::istream& operator>>(std::istream& in, Schedule& schedule);
	friend std::ostream& operator<<(std::ostream& out, const Schedule& schedule);
};

