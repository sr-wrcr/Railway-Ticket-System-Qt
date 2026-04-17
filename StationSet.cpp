#include "StationSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

StationSet::StationSet(void) {}

StationSet::~StationSet(void) {}

void StationSet::AddStation(const Stations& station) {
    for (const auto& s : stations) {
        if (s.GetStationId() == station.GetStationId()) {
            throw std::invalid_argument("Station with the same ID already exists.");
        }
    }
    stations.push_back(station);
}

void StationSet::RemoveStation(int stationId) {
    auto it = std::remove_if(stations.begin(), stations.end(), [stationId](const Stations& station) {
        return station.GetStationId() == stationId;
        });
    if (it != stations.end()) {
        stations.erase(it, stations.end());
    }
    else {
        throw std::invalid_argument("指定ID的站点不存在。");
    }
}

void StationSet::UpdateStation(const Stations& station) {
    for (auto& s : stations) {
        if (s.GetStationId() == station.GetStationId()) {
            s = station;
            return;
        }
    }
    throw std::invalid_argument("指定ID的站点不存在。.");
}

Stations StationSet::GetStation(int stationId) const {
    for (const auto& s : stations) {
        if (s.GetStationId() == stationId) {
            return s;
        }
    }
    throw std::invalid_argument("指定ID的站点不存在。");
}

void StationSet::ReadFromFile(const QString& fileName) {
    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("文件打开失败: " + fileName.toStdString());
    }

    stations.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Stations station;
        ss >> station;
        stations.push_back(station);
    }
    file.close();
}

void StationSet::WriteToFile(const QString& fileName) const {
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("文件打开失败: " + fileName.toStdString());
    }

    for (const auto& station : stations) {
        file << station << "\n";
    }
    file.close();
}

std::vector<Stations> StationSet::GetAllStations() const {
    return stations;
}

void StationSet::SetStations(const std::vector<Stations>& newStations) {
    stations = newStations;
}

QString StationSet::GetStationNameById(int stationId) const {
    for (const auto& station : stations) {
        if (station.GetStationId() == stationId) {
            return station.GetStationName();
        }
    }
    return "Unknown Station";
}

int StationSet::GetStationIdByName(const QString& stationName) const {
    for (const auto& station : stations) {
        if (station.GetStationName() == stationName) {
            return station.GetStationId();
        }
    }
    return -1; // 返回 -1 表示未找到
}
