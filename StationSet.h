#pragma once

#include "Stations.h"
#include <vector>

class StationSet {
public:
    StationSet(void);
    virtual ~StationSet(void);

    void AddStation(const Stations& station);
    void RemoveStation(int stationId);
    void UpdateStation(const Stations& station);

    Stations GetStation(int stationId) const;

    void ReadFromFile(const QString& fileName);
    void WriteToFile(const QString& fileName) const;

    std::vector<Stations> GetAllStations() const;

    QString GetStationNameById(int stationId) const;
	int GetStationIdByName(const QString& stationName) const;
    void SetStations(const std::vector<Stations>& newStations);

private:
    std::vector<Stations> stations;
};

