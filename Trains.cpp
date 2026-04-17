#include "Trains.h"
#include <sstream>

Trains::Trains()
    : train_id(0), train_name(""), train_type(""), start_station_id(0), end_station_id(0) {
}

Trains::Trains(int id, const QString& name, const QString& type, int startId, int endId)
	: train_id(id), train_name(name), train_type(type), start_station_id(startId), end_station_id(endId) {
}

Trains::~Trains() {
	// Destructor implementation (if needed)
}

int Trains::GetTrainId() const {
	return train_id;
}

QString Trains::GetTrainName() const {
	return train_name;
}

QString Trains::GetTrainType() const {
	return train_type;
}

int Trains::GetStartStationId() const {
	return start_station_id;
}

int Trains::GetEndStationId() const {
	return end_station_id;
}

void Trains::SetTrainId(int id) {
	train_id = id;
}

void Trains::SetTrainName(const QString& name) {
	train_name = name;
}

void Trains::SetTrainType(const QString& type) {
	train_type = type;
}

void Trains::SetStartStationId(int startId) {
	start_station_id = startId;
}

void Trains::SetEndStationId(int endId) {
	end_station_id = endId;
}

std::istream& operator>>(std::istream& in, Trains& train) {
    std::string line;
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, name, type, startStationIdStr, endStationIdStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, type, ',');
        std::getline(ss, startStationIdStr, ',');
        std::getline(ss, endStationIdStr, ',');

        if (idStr.empty() || name.empty() || type.empty() || startStationIdStr.empty() || endStationIdStr.empty()) {
            throw std::invalid_argument("列车数据包含空字段。");
        }

        train.train_id = std::stoi(idStr);
        train.train_name = QString::fromStdString(name);
        train.train_type = QString::fromStdString(type);
        train.start_station_id = std::stoi(startStationIdStr);
        train.end_station_id = std::stoi(endStationIdStr);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Trains& train) {
    out << train.train_id << ","
        << train.train_name.toStdString() << ","
        << train.train_type.toStdString() << ","
        << train.start_station_id << ","
        << train.end_station_id;
    return out;
}
    