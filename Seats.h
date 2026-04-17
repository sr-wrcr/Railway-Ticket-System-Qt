#pragma once

#include <QString>
#include <iostream>

class Seats
{
private:
	int seat_id;
	int train_id;
	QString seat_no;
	QString seat_type;
	int price;

public:
	Seats();
	Seats(int id, int trainId, const QString& no, const QString& type, int priceValue);
	virtual ~Seats();

	int GetSeatId() const;
	int GetTrainId() const;
	QString GetSeatNo() const;
	QString GetSeatType() const;
	int GetPrice() const;

	void SetSeatId(int id);
	void SetTrainId(int trainId);
	void SetSeatNo(const QString& no);
	void SetSeatType(const QString& type);
	void SetPrice(int pri);

	friend std::istream& operator>>(std::istream& in, Seats& seat);
	friend std::ostream& operator<<(std::ostream& out, const Seats& seat);

};

