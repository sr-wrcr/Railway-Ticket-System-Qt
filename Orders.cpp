#include "Orders.h"
#include <sstream>
#include <string>


Orders::Orders()
    : order_id(0), user_id(0), train_id(0), seat_id(0),
    start_station_id(0), end_station_id(0), travel_date(QDate()), order_time(QDateTime()), status("") {
}

Orders::Orders(int id, int userId, int trainId, int seatId, int startStationId, int endStationId, QDate date, QDateTime time,	
			   const QString& stat)
	: order_id(id), user_id(userId), train_id(trainId), seat_id(seatId),
	start_station_id(startStationId), end_station_id(endStationId), travel_date(date), order_time(time), status(stat) {
}

Orders::~Orders() {
}

int Orders::GetOrderId() const {
	return order_id;
}

int Orders::GetUserId() const {
	return user_id;
}

int Orders::GetTrainId() const {
	return train_id;
}

int Orders::GetSeatId() const {
	return seat_id;
}

int Orders::GetStartStationId() const {
	return start_station_id;
}

int Orders::GetEndStationId() const {
	return end_station_id;
}

QDate Orders::GetTravelDate() const {
	return travel_date;
}

QDateTime Orders::GetOrderTime() const {
 	return order_time;
}

QString Orders::GetStatus() const {
	return status;
}

int Orders::GetPrice() const {
    return price;
}

void Orders::SetOrderId(int id) {
	order_id = id;
}

void Orders::SetUserId(int userId) {
	user_id = userId;
}

void Orders::SetTrainId(int trainId) {
	train_id = trainId;
}

void Orders::SetSeatId(int seatId) {
	seat_id = seatId;
}

void Orders::SetStartStationId(int startStationId) {
	start_station_id = startStationId;
}

void Orders::SetEndStationId(int endStationId) {
	end_station_id = endStationId;
}

void Orders::SetTravelDate(QDate date) {
	travel_date = date;
}

 void Orders::SetOrderTime(QDateTime time) {
 	order_time = time;
 }

void Orders::SetStatus(const QString& stat) {
	status = stat;
}

void Orders::SetPrice(int pric) {
    price = pric;
}

std::istream& operator>>(std::istream& in, Orders& order) {
    std::string line;
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, userIdStr, trainIdStr, seatIdStr, startStationIdStr, endStationIdStr, travelDateStr, orderTimeStr, status, price;

        std::getline(ss, idStr, ',');
        std::getline(ss, userIdStr, ',');
        std::getline(ss, trainIdStr, ',');
        std::getline(ss, seatIdStr, ',');
        std::getline(ss, startStationIdStr, ',');
        std::getline(ss, endStationIdStr, ',');
        std::getline(ss, travelDateStr, ',');
        std::getline(ss, orderTimeStr, ',');
        std::getline(ss, status, ',');
        std::getline(ss, price, ',');

        if (idStr.empty() || userIdStr.empty() || trainIdStr.empty() || seatIdStr.empty() || startStationIdStr.empty() || endStationIdStr.empty() || travelDateStr.empty() || orderTimeStr.empty() || status.empty() || price.empty() ) {
            throw std::invalid_argument("订单数据包含空字段。");
        }

        order.order_id = std::stoi(idStr);
        order.user_id = std::stoi(userIdStr);
        order.train_id = std::stoi(trainIdStr);
        order.seat_id = std::stoi(seatIdStr);
        order.start_station_id = std::stoi(startStationIdStr);
        order.end_station_id = std::stoi(endStationIdStr);
        order.travel_date = QDate::fromString(QString::fromStdString(travelDateStr), "yyyy-MM-dd");
        order.order_time = QDateTime::fromString(QString::fromStdString(orderTimeStr), "yyyy-MM-dd HH:mm");
        order.status = QString::fromStdString(status);
        order.price = std::stoi(price); 

        if (!order.travel_date.isValid() || !order.order_time.isValid()) {
            throw std::invalid_argument("订单数据包含无效的日期或时间。");
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Orders& order) {
    out << order.order_id << "," << order.user_id << "," << order.train_id << ","
        << order.seat_id << "," << order.start_station_id << ","
        << order.end_station_id << ","
        << order.travel_date.toString("yyyy-MM-dd").toStdString() << ","
        << order.order_time.toString("yyyy-MM-dd HH:mm").toStdString() << "," // 修改为不包含秒
        << order.status.toStdString() << ","
        << order.price;
    return out;
}


QTime Orders::GetStartTime(const ScheduleSet& scheduleSet) const {
    // 获取与 train_id 和 start_station_id 匹配的 Schedule
    auto schedules = scheduleSet.GetSchedulesByTrainId(train_id);
    for (const auto& schedule : schedules) {
        if (schedule.GetStationId() == start_station_id) {
            return schedule.GetDepartTime(); // 返回起始站的出发时间
        }
    }
    return QTime(); // 如果未找到，返回无效时间
}

QTime Orders::GetEndTime(const ScheduleSet& scheduleSet) const {
    // 获取与 train_id 和 end_station_id 匹配的 Schedule
    auto schedules = scheduleSet.GetSchedulesByTrainId(train_id);
    for (const auto& schedule : schedules) {
        if (schedule.GetStationId() == end_station_id) {
            return schedule.GetArrivalTime(); // 返回终点站的到达时间
        }
    }
    return QTime(); // 如果未找到，返回无效时间
}
