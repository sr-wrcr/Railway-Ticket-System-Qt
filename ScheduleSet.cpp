#include "ScheduleSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

ScheduleSet::ScheduleSet(void) {}

ScheduleSet::~ScheduleSet(void) {}

void ScheduleSet::AddSchedule(const Schedule& schedule) {
    for (const auto& s : schedules) {
        if (s.GetScheduleId() == schedule.GetScheduleId()) {
            throw std::invalid_argument("时刻表中已经存在相同ID。");
        }
    }
    schedules.push_back(schedule);
}

void ScheduleSet::RemoveSchedule(int scheduleId) {
    auto it = std::remove_if(schedules.begin(), schedules.end(), [scheduleId](const Schedule& schedule) {
        return schedule.GetScheduleId() == scheduleId;
        });
    if (it != schedules.end()) {
        schedules.erase(it, schedules.end());
    }
    else {
        throw std::invalid_argument("指定ID的时刻表不存在。");
    }
}

void ScheduleSet::UpdateSchedule(const Schedule& schedule) {
    for (auto& s : schedules) {
        if (s.GetScheduleId() == schedule.GetScheduleId()) {
            s = schedule;
            return;
        }
    }
    throw std::invalid_argument("指定 ID 的时刻表不存在。");
}

Schedule ScheduleSet::GetSchedule(int scheduleId) const {
    for (const auto& s : schedules) {
        if (s.GetScheduleId() == scheduleId) {
            return s;
        }
    }
    throw std::invalid_argument("指定 ID 的时刻表不存在。");
}

void ScheduleSet::ReadFromFile(const QString& fileName) {
    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("打开文件失败: " + fileName.toStdString());
    }

    schedules.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Schedule schedule;
        ss >> schedule;
        schedules.push_back(schedule);
    }
    file.close();
}

void ScheduleSet::WriteToFile(const QString& fileName) const {
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("文件打开失败: " + fileName.toStdString());
    }

    for (const auto& schedule : schedules) {
        file << schedule << "\n";
    }
    file.close();
}

std::vector<Schedule> ScheduleSet::GetAllSchedules() const {
    return schedules;
}

void ScheduleSet::SetSchedules(const std::vector<Schedule>& newSchedules) {
    schedules = newSchedules;
}

std::vector<Schedule> ScheduleSet::GetSchedulesByTrainId(int trainId) const {
    std::vector<Schedule> result;
    for (const auto& schedule : schedules) {
        if (schedule.GetTrainId() == trainId) {
            result.push_back(schedule);
        }
    }
    return result;
}