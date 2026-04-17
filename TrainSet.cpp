#include "TrainSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

TrainSet::TrainSet(void) {}

TrainSet::~TrainSet(void) {}

void TrainSet::AddTrain(const Trains& train) {
    for (const auto& t : trains) {
        if (t.GetTrainId() == train.GetTrainId()) {
            throw std::invalid_argument("相同ID列车已存在。");
        }
    }
    trains.push_back(train);
}

void TrainSet::RemoveTrain(int trainId) {
    auto it = std::remove_if(trains.begin(), trains.end(), [trainId](const Trains& train) {
        return train.GetTrainId() == trainId;
        });
    if (it != trains.end()) {
        trains.erase(it, trains.end());
    }
    else {
        throw std::invalid_argument("指定ID的列车不存在。.");
    }
}

void TrainSet::UpdateTrain(const Trains& train) {
    for (auto& t : trains) {
        if (t.GetTrainId() == train.GetTrainId()) {
            t = train;
            return;
        }
    }
    throw std::invalid_argument("指定ID的列车不存在。");
}

Trains TrainSet::GetTrain(int trainId) const {
    for (const auto& t : trains) {
        if (t.GetTrainId() == trainId) {
            return t;
        }
    }
    throw std::invalid_argument("指定ID的列车不存在。");
}

void TrainSet::ReadFromFile(const QString& fileName) {
    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("文件打开失败: " + fileName.toStdString());
    }

    trains.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Trains train;
        ss >> train;
        trains.push_back(train);
    }
    file.close();
}

void TrainSet::WriteToFile(const QString& fileName) const {
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName.toStdString());
    }

    for (const auto& train : trains) {
        file << train << "\n";
    }
    file.close();
}

std::vector<Trains> TrainSet::GetAllTrains() const {
    return trains;
}

QString TrainSet::GetTrainNameById(int trainId) const {
    for (const auto& train : trains) {
        if (train.GetTrainId() == trainId) {
            return train.GetTrainName();
        }
    }
    return "Unknown Train";
}

int TrainSet::GetTrainIdByName(const QString& trainName) const {
    for (const auto& train : trains) {
        if (train.GetTrainName() == trainName) {
            return train.GetTrainId();
        }
    }
    return -1; // Not found
}

void TrainSet::SetTrains(const std::vector<Trains>& newTrains) {
    trains = newTrains;
}