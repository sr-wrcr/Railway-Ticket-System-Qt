#include "Schedule.h"

#include <sstream>


Schedule::Schedule()
    : schedule_id(0), train_id(0), station_id(0), stop_order(0), arrival_time(QTime()), depart_time(QTime()) {
}

Schedule::Schedule(int scheduleId, int trainId, int stationId, int stopOrder, QTime arrivalTime, QTime departTime)
    : schedule_id(scheduleId), train_id(trainId), station_id(stationId), stop_order(stopOrder), arrival_time(arrivalTime), depart_time(departTime) {
}


Schedule::~Schedule() {
}

int Schedule::GetScheduleId() const {
	return schedule_id;
}

int Schedule::GetTrainId() const {
	return train_id;
}

int Schedule::GetStationId() const {
	return station_id;
}

int Schedule::GetStopOrder() const {
	return stop_order;
}

QTime Schedule::GetArrivalTime() const {
	return arrival_time;
}

QTime Schedule::GetDepartTime() const {
	return depart_time;
}

void Schedule::SetScheduleId(int scheduleId) {
	schedule_id = scheduleId;
}

void Schedule::SetTrainId(int trainId) {
	train_id = trainId;
}

void Schedule::SetStationId(int stationId) {
	station_id = stationId;
}

void Schedule::SetStopOrder(int stopOrder) {
	stop_order = stopOrder;
}

void Schedule::SetArrivalTime(QTime arrivalTime) {
	arrival_time = arrivalTime;
}

void Schedule::SetDepartTime(QTime departTime) {
	depart_time = departTime;
}
std::istream& operator>>(std::istream& in, Schedule& schedule) {
    std::string line;
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, trainIdStr, stationIdStr, stopOrderStr, arrivalTimeStr, departTimeStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, trainIdStr, ',');
        std::getline(ss, stationIdStr, ',');
        std::getline(ss, stopOrderStr, ',');
        std::getline(ss, arrivalTimeStr, ',');
        std::getline(ss, departTimeStr, ',');

        if (idStr.empty() || trainIdStr.empty() || stationIdStr.empty() || stopOrderStr.empty()) {
            throw std::invalid_argument("时刻表数据包含空字段。");
        }

        schedule.schedule_id = std::stoi(idStr);
        schedule.train_id = std::stoi(trainIdStr);
        schedule.station_id = std::stoi(stationIdStr);
        schedule.stop_order = std::stoi(stopOrderStr);

        if (!arrivalTimeStr.empty()) {
            schedule.arrival_time = QTime::fromString(QString::fromStdString(arrivalTimeStr), "HH:mm");
            if (!schedule.arrival_time.isValid()) {
                throw std::invalid_argument("时刻表数据包含无效的抵达时间。");
            }
        }
        else {
            schedule.arrival_time = QTime();
        }

        if (!departTimeStr.empty()) {
            schedule.depart_time = QTime::fromString(QString::fromStdString(departTimeStr), "HH:mm");
            if (!schedule.depart_time.isValid()) {
                throw std::invalid_argument("时刻表数据包含无效的出发时间。");
            }
        }
        else {
            schedule.depart_time = QTime();
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Schedule& schedule) {
    out << schedule.schedule_id << ","
        << schedule.train_id << ","
        << schedule.station_id << ","
        << schedule.stop_order << ","
        << schedule.arrival_time.toString("HH:mm").toStdString() << ","
        << schedule.depart_time.toString("HH:mm").toStdString();
    return out;
}


