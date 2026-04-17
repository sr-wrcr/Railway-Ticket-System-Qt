#pragma once

#include "Trains.h"
#include <vector>

class TrainSet {
public:
    TrainSet(void);
    virtual ~TrainSet(void);

    void AddTrain(const Trains& train);
    void RemoveTrain(int trainId);
    void UpdateTrain(const Trains& train);

    Trains GetTrain(int trainId) const;

    void ReadFromFile(const QString& fileName);
    void WriteToFile(const QString& fileName) const;

	std::vector<Trains> GetAllTrains() const;

    QString GetTrainNameById(int trainId) const;
	int GetTrainIdByName(const QString& trainName) const;
	void SetTrains(const std::vector<Trains>& newTrains);

private:
    std::vector<Trains> trains;
};