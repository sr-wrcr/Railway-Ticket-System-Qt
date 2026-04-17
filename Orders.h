#pragma once

#include <QDate>
#include <QString>
#include <QDateTime>
#include <iostream>

#include "TrainSet.h"
#include "UserSet.h"
#include "ScheduleSet.h"


using namespace std;

class Orders
{
private:
	int order_id;
	int user_id;
	int train_id;
	int seat_id;
	int start_station_id;
	int end_station_id;
	QDate travel_date;
	QDateTime order_time;
	QString status;
	int price;

public:
	Orders();
	Orders(int id, int userId, int trainId, int seatId, int startStationId, int endStationId, 
		QDate date, QDateTime time, const QString& stat);
	virtual ~Orders();

	int GetOrderId() const;
	int GetUserId() const;
	int GetTrainId() const;
	int GetSeatId() const;
	int GetStartStationId() const;
	int GetEndStationId() const;
	QDate GetTravelDate() const;
	QDateTime GetOrderTime() const;
	QString GetStatus() const;
	int GetPrice() const;

	void SetPrice(int price);
	void SetOrderId(int id);
	void SetUserId(int userId);
	void SetTrainId(int trainId);
	void SetSeatId(int seatId);
	void SetStartStationId(int startStationId);
	void SetEndStationId(int endStationId);
	void SetTravelDate(QDate date);
	void SetOrderTime(QDateTime datetime);
	void SetStatus(const QString& stat);

	QTime GetStartTime(const ScheduleSet& scheduleSet) const; // 获取起始站的出发时间
	QTime GetEndTime(const ScheduleSet& scheduleSet) const;   // 获取终点站的到达时间

	friend istream& operator>>(istream& in, Orders& order);
	friend ostream& operator<<(ostream& out, const Orders& order);
};

