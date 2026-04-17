#include "Seats.h"

#include <sstream>


Seats::Seats()
    : seat_id(0), train_id(0), seat_no(""), seat_type(""), price(0) {
}


Seats::Seats(int id, int trainId, const QString& no, const QString& type, int priceValue)
	: seat_id(id), train_id(trainId), seat_no(no), seat_type(type), price(priceValue) {
}

Seats::~Seats() {
	// Destructor implementation (if needed)
}

int Seats::GetSeatId() const {
	return seat_id;
}

int Seats::GetTrainId() const {
	return train_id;
}

QString Seats::GetSeatNo() const {
	return seat_no;
}

QString Seats::GetSeatType() const {
	return seat_type;
}

int Seats::GetPrice() const {
	return price;
}

void Seats::SetSeatId(int id) {
	seat_id = id;
}

void Seats::SetTrainId(int trainId) {
	train_id = trainId;
}

void Seats::SetSeatNo(const QString& no) {
	seat_no = no;
}

void Seats::SetSeatType(const QString& type) {
	seat_type = type;
}

void Seats::SetPrice(int pri) {
    price = pri;
}

std::istream& operator>>(std::istream& in, Seats& seat) {
    std::string line;
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, trainIdStr, seatNo, seatType, priceStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, trainIdStr, ',');
        std::getline(ss, seatNo, ',');
        std::getline(ss, seatType, ',');
        std::getline(ss, priceStr, ',');

        if (idStr.empty() || trainIdStr.empty() || seatNo.empty() || seatType.empty() || priceStr.empty()) {
            throw std::invalid_argument("座位数据包含空字段。");
        }

        seat.seat_id = std::stoi(idStr);
        seat.train_id = std::stoi(trainIdStr);
        seat.seat_no = QString::fromStdString(seatNo);
        seat.seat_type = QString::fromStdString(seatType);
        seat.price = std::stoi(priceStr);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Seats& seat) {
    out << seat.seat_id << ","
        << seat.train_id << ","
        << seat.seat_no.toStdString() << ","
        << seat.seat_type.toStdString() << ","
        << seat.price;
    return out;
}