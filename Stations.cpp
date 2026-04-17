#include "Stations.h"
#include <sstream>


Stations::Stations()
    : station_id(0), station_name("") {
}

Stations::Stations(int id, const QString& name)
	: station_id(id), station_name(name) {
}

Stations::~Stations() {
	// Destructor implementation (if needed)
}

int Stations::GetStationId() const {
	return station_id;
}

QString Stations::GetStationName() const {
	return station_name;
}

void Stations::SetStationId(int id) {
	station_id = id;
}

void Stations::SetStationName(const QString& name) {
	station_name = name;
}

std::istream& operator>>(std::istream& in, Stations& station) {
    std::string line;
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, name;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');

        if (idStr.empty() || name.empty()) {
            throw std::invalid_argument("站点数据包含空字段。");
        }

        station.station_id = std::stoi(idStr);
        station.station_name = QString::fromStdString(name);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Stations& station) {
    out << station.station_id << ","
        << station.station_name.toStdString();
    return out;
}

