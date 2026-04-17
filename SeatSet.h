#pragma once

#include "Seats.h"
#include "OrderSet.h"
#include <vector>
#include <QString>
#include <map>
#include <QDate>

class SeatSet {
public:
    SeatSet(void);
    virtual ~SeatSet(void);

    void AddSeat(const Seats& seat);
    void RemoveSeat(int seatId);
    void UpdateSeat(const Seats& seat);

    Seats GetSeat(int seatId) const;

    void ReadFromFile(const QString& fileName, const OrderSet& orderSet);
    void WriteToFile(const QString& fileName) const;

    std::vector<Seats> GetAllSeats() const;
    void SetSeats(const std::vector<Seats>& newSeats);

    QString GetSeatTypeById(int seatId) const;
    QString GetSeatNoById(int seatId) const;
    int GetPriceById(int seatId) const;

    int GetSeatPrice(QString seatType) const;
    int GetSeatAvailability(QString seatType, const OrderSet& orderSet) const;

    int GetSeatIdByType(const QString& seatType, int trainId, const OrderSet& orderSet) const;

    int GetDynamicPrice(int seatId, int startStationId, int endStationId, const ScheduleSet& scheduleSet) const;

    std::vector<QString> GetAllSeatTypes() const;

    void ResetDailySeatAvailability(const QDate& date, const OrderSet& orderSet);
    int GetSeatAvailabilityByDate(const QDate& date, int trainId, const QString& seatType, const OrderSet& orderSet) const;
    void BookSeat(const QDate& date, int seatId);

    void RegisterOrderSet(OrderSet& orderSet); // 注册订单事件回调
    int GetSeatIdByDateAndType(const QDate& date, int trainId, const QString& seatType) const;

    void SyncDailySeatAvailability(const OrderSet& orderSet);

private:
    std::vector<Seats> seats;
    std::map<QDate, std::map<int, int>> dailySeatAvailability;

    bool IsSeatAvailable(int seatId, const OrderSet& orderSet) const; 
};