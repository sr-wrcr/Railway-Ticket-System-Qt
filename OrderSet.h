#pragma once

#include "Orders.h"
#include <vector>
#include <functional>

class OrderSet {
public:
    OrderSet(void);
    virtual ~OrderSet(void);

    void AddOrder(const Orders& order);
    void RemoveOrder(int orderId);
    void UpdateOrder(const Orders& order);

    Orders GetOrder(int orderId) const;
    void ReadFromFile(const QString& fileName);
    void WriteToFile(const QString& fileName) const;

    std::vector<Orders> GetAllOrders() const;
    void SetOrders(const std::vector<Orders>& newOrders);
    std::vector<Orders> GetOrdersByUserId(int userId) const;

    bool HasOrderInTimeRange(int userId, const QDate& travelDate, 
        const QTime& startTime, const QTime& endTime, 
        const ScheduleSet& scheduleSet, const Orders* currentOrder) const;

    std::function<void()> onOrderChanged;

private:
    std::vector<Orders> orders;
};