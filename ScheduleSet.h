#pragma once

#include "Schedule.h"
#include <vector>

class ScheduleSet {
public:
    ScheduleSet(void);
    virtual ~ScheduleSet(void);

    void AddSchedule(const Schedule& schedule);
    void RemoveSchedule(int scheduleId);
    void UpdateSchedule(const Schedule& schedule);

    Schedule GetSchedule(int scheduleId) const;

    void ReadFromFile(const QString& fileName);
    void WriteToFile(const QString& fileName) const;

	std::vector<Schedule> GetAllSchedules() const;
	void SetSchedules(const std::vector<Schedule>& newSchedules);

    std::vector<Schedule> GetSchedulesByTrainId(int trainId) const;

private:
    std::vector<Schedule> schedules;
};