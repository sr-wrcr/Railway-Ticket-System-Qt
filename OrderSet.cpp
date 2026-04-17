#include "OrderSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

OrderSet::OrderSet(void) {}

OrderSet::~OrderSet(void) {}

void OrderSet::AddOrder(const Orders& order) {
    for (const auto& o : orders) {
        if (o.GetOrderId() == order.GetOrderId()) {
            throw std::invalid_argument("具有相同ID的订单已存在。");
        }
    }
    orders.push_back(order);

    // 触发订单变更事件
    if (onOrderChanged) {
        onOrderChanged();
    }
}

void OrderSet::RemoveOrder(int orderId) {
    auto it = std::remove_if(orders.begin(), orders.end(), [orderId](const Orders& order) {
        return order.GetOrderId() == orderId;
        });
    if (it != orders.end()) {
        orders.erase(it, orders.end());

        // 触发订单变更事件
        if (onOrderChanged) {
            onOrderChanged();
        }
    }
    else {
        throw std::invalid_argument("指定ID的订单不存在。");
    }
}

void OrderSet::UpdateOrder(const Orders& order) {
    for (auto& o : orders) {
        if (o.GetOrderId() == order.GetOrderId()) {
            o = order;

            // 触发订单变更事件
            if (onOrderChanged) {
                onOrderChanged();
            }
            return;
        }
    }
    throw std::invalid_argument("指定ID的订单不存在。");
}

Orders OrderSet::GetOrder(int orderId) const {
    for (const auto& o : orders) {
        if (o.GetOrderId() == orderId) {
            return o;
        }
    }
    throw std::invalid_argument("指定ID的订单不存在。");
}

void OrderSet::ReadFromFile(const QString& fileName) {
    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件：" + fileName.toStdString());
    }

    orders.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Orders order;
        ss >> order;
        orders.push_back(order);
    }
    file.close();
}

void OrderSet::WriteToFile(const QString& fileName) const {
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件：" + fileName.toStdString());
    }

    for (const auto& order : orders) {
        file << order << "\n";
    }
    file.close();
}

std::vector<Orders> OrderSet::GetAllOrders() const {
    return orders;
}

void OrderSet::SetOrders(const std::vector<Orders>& newOrders) {
    orders = newOrders;

    // 触发订单变更事件
    if (onOrderChanged) {
        onOrderChanged();
    }
}

std::vector<Orders> OrderSet::GetOrdersByUserId(int userId) const {
    std::vector<Orders> userOrders;
    for (const auto& order : orders) {
        if (order.GetUserId() == userId) {
            userOrders.push_back(order);
        }
    }
    return userOrders;
}

bool OrderSet::HasOrderInTimeRange(int userId, const QDate& travelDate, const QTime& startTime, const QTime& endTime, const ScheduleSet& scheduleSet, const Orders* currentOrder) const {
    for (const auto& order : orders) {
        if ((currentOrder != nullptr && order.GetOrderId() == currentOrder->GetOrderId()) || order.GetStatus() == "canceled") {
            continue; // 忽略当前订单和已取消的订单
        }

        if (order.GetUserId() == userId && order.GetTravelDate() == travelDate) {
            QTime orderStartTime = order.GetStartTime(scheduleSet);
            QTime orderEndTime = order.GetEndTime(scheduleSet);

            if (!orderStartTime.isValid() || !orderEndTime.isValid()) {
                throw std::runtime_error("现有订单中存在无效时间。");
            }

            if ((startTime < orderEndTime && endTime > orderStartTime)) {
                return true;
            }
        }
    }
    return false;
}