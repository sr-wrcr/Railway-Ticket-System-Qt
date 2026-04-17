#include "SeatSet.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <set>

SeatSet::SeatSet(void) {}

SeatSet::~SeatSet(void) {}

void SeatSet::AddSeat(const Seats& seat) {
    for (const auto& s : seats) {
        if (s.GetSeatId() == seat.GetSeatId()) {
            throw std::invalid_argument("具有相同ID的座位已存在。");
        }
    }
    seats.push_back(seat);
}

void SeatSet::RemoveSeat(int seatId) {
    auto it = std::remove_if(seats.begin(), seats.end(), [seatId](const Seats& seat) {
        return seat.GetSeatId() == seatId;
        });
    if (it != seats.end()) {
        seats.erase(it, seats.end());
    }
    else {
        throw std::invalid_argument("指定ID的座位不存在。");
    }
}

void SeatSet::UpdateSeat(const Seats& seat) {
    for (auto& s : seats) {
        if (s.GetSeatId() == seat.GetSeatId()) {
            s = seat;
            return;
        }
    }
    throw std::invalid_argument("指定ID的座位不存在。");
}

Seats SeatSet::GetSeat(int seatId) const {
    for (const auto& s : seats) {
        if (s.GetSeatId() == seatId) {
            return s;
        }
    }
    throw std::invalid_argument("指定ID的座位不存在。");
}

void SeatSet::ReadFromFile(const QString& fileName, const OrderSet& orderSet) {
    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件：" + fileName.toStdString());
    }

    seats.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Seats seat;
        ss >> seat;
        seats.push_back(seat);
    }
    file.close();

    dailySeatAvailability.clear();
    QDate today = QDate::currentDate();
    for (int i = 0; i < 30; ++i) {
        QDate futureDate = today.addDays(i);
        ResetDailySeatAvailability(futureDate, orderSet);
    }

    SyncDailySeatAvailability(orderSet);
}

void SeatSet::WriteToFile(const QString& fileName) const {
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件：" + fileName.toStdString());
    }

    for (const auto& seat : seats) {
        file << seat << "\n";
    }
    file.close();
}

std::vector<Seats> SeatSet::GetAllSeats() const {
    return seats;
}

void SeatSet::SetSeats(const std::vector<Seats>& newSeats) {
    seats = newSeats;
}

QString SeatSet::GetSeatTypeById(int seatId) const {
    for (const auto& seat : seats) {
        if (seat.GetSeatId() == seatId) {
            return seat.GetSeatType();
        }
    }
    throw std::invalid_argument("指定ID的座位不存在。");
}

QString SeatSet::GetSeatNoById(int seatId) const {
    for (const auto& seat : seats) {
        if (seat.GetSeatId() == seatId) {
            return seat.GetSeatNo();
        }
    }
    throw std::invalid_argument("指定ID的座位不存在。");
}

int SeatSet::GetPriceById(int seatId) const {
    for (const auto& seat : seats) {
        if (seat.GetSeatId() == seatId) {
            return seat.GetPrice();
        }
    }
    throw std::invalid_argument("指定ID的座位不存在。");
}

int SeatSet::GetSeatPrice(QString seatType) const {
    for (const auto& seat : seats) {
        if (seat.GetSeatType() == seatType) {
            return seat.GetPrice();
        }
    }
    throw std::invalid_argument("指定类型的座位不存在。");
}

int SeatSet::GetSeatAvailability(QString seatType, const OrderSet& orderSet) const {
    int availableCount = 0;
    for (const auto& seat : seats) {
        if (seat.GetSeatType() == seatType && IsSeatAvailable(seat.GetSeatId(), orderSet)) {
            ++availableCount;
        }
    }
    return availableCount;
}

int SeatSet::GetSeatIdByType(const QString& seatType, int trainId, const OrderSet& orderSet) const {
    for (const auto& seat : seats) {
        if (seat.GetSeatType() == seatType && seat.GetTrainId() == trainId) {
            if (IsSeatAvailable(seat.GetSeatId(), orderSet)) {
                return seat.GetSeatId();
            }
        }
    }
    throw std::invalid_argument("指定类型的座位在给定列车中不可用。");
}

int SeatSet::GetDynamicPrice(int seatId, int startStationId, int endStationId, const ScheduleSet& scheduleSet) const {
    int basePrice = GetPriceById(seatId);

    auto schedules = scheduleSet.GetSchedulesByTrainId(GetSeat(seatId).GetTrainId());
    int totalStations = static_cast<int>(schedules.size());

    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < schedules.size(); ++i) {
        if (schedules[i].GetStationId() == startStationId) {
            startIndex = i;
        }
        if (schedules[i].GetStationId() == endStationId) {
            endIndex = i;
        }
    }

    if (startIndex == -1 || endIndex == -1 || startIndex >= endIndex) {
        throw std::invalid_argument("起始站或终点站无效。");
    }

    int traveledStations = endIndex - startIndex;
    return static_cast<int>(basePrice * (static_cast<double>(traveledStations) / (totalStations - 1)));
}

std::vector<QString> SeatSet::GetAllSeatTypes() const {
    std::set<QString> uniqueSeatTypes;
    for (const auto& seat : seats) {
        uniqueSeatTypes.insert(seat.GetSeatType());
    }
    return std::vector<QString>(uniqueSeatTypes.begin(), uniqueSeatTypes.end());
}

void SeatSet::ResetDailySeatAvailability(const QDate& date, const OrderSet& orderSet) {
    if (dailySeatAvailability.find(date) == dailySeatAvailability.end()) {
        dailySeatAvailability[date] = {};
        for (const auto& seat : seats) {
            dailySeatAvailability[date][seat.GetSeatId()] = 1; // 初始化为可用
        }
    }

    // 同步订单数据
    for (const auto& order : orderSet.GetAllOrders()) {
        if (order.GetTravelDate() == date && order.GetStatus() != "canceled") {
            dailySeatAvailability[date][order.GetSeatId()] = 0; // 标记为已使用
        }
    }
}

int SeatSet::GetSeatAvailabilityByDate(const QDate& date, int trainId, const QString& seatType, const OrderSet& orderSet) const {
    if (dailySeatAvailability.find(date) == dailySeatAvailability.end()) {
        const_cast<SeatSet*>(this)->ResetDailySeatAvailability(date, orderSet);
    }

    int availableCount = 0;
    for (const auto& seat : seats) {
        if (seat.GetTrainId() == trainId && seat.GetSeatType() == seatType) {
            if (dailySeatAvailability.at(date).at(seat.GetSeatId()) > 0) {
                ++availableCount;
            }
        }
    }
    return availableCount;
}

void SeatSet::BookSeat(const QDate& date, int seatId) {
    if (dailySeatAvailability.find(date) == dailySeatAvailability.end()) {
        throw std::runtime_error("指定日期没有座位可用数据。");
    }

    if (dailySeatAvailability[date][seatId] > 0) {
        --dailySeatAvailability[date][seatId];
    }
    else {
        throw std::runtime_error("指定日期和座位ID没有可用座位。");
    }
}

void SeatSet::SyncDailySeatAvailability(const OrderSet& orderSet) {
    for (const auto& order : orderSet.GetAllOrders()) {
        QDate orderDate = order.GetTravelDate();
        int seatId = order.GetSeatId();

        if (dailySeatAvailability.find(orderDate) != dailySeatAvailability.end() &&
            dailySeatAvailability[orderDate].find(seatId) != dailySeatAvailability[orderDate].end()) {

            if (order.GetStatus() == "canceled") {
                dailySeatAvailability[orderDate][seatId] = 1; // 标记为可用
            } else {
                dailySeatAvailability[orderDate][seatId] = 0; // 标记为已使用
            }
        }
    }
}

void SeatSet::RegisterOrderSet(OrderSet& orderSet) {
    orderSet.onOrderChanged = [this, &orderSet]() {
        this->SyncDailySeatAvailability(orderSet);
        };
}

bool SeatSet::IsSeatAvailable(int seatId, const OrderSet& orderSet) const {
    for (const auto& order : orderSet.GetAllOrders()) {
        if (order.GetSeatId() == seatId && order.GetStatus() != "canceled") {
            return false;
        }
    }
    return true;
}

int SeatSet::GetSeatIdByDateAndType(const QDate& date, int trainId, const QString& seatType) const {
    if (dailySeatAvailability.find(date) == dailySeatAvailability.end()) {
        throw std::invalid_argument("指定日期没有座位可用数据。");
    }

    for (const auto& seat : seats) {
        if (seat.GetTrainId() == trainId && seat.GetSeatType() == seatType) {
            if (dailySeatAvailability.at(date).at(seat.GetSeatId()) > 0) {
                return seat.GetSeatId();
            }
        }
    }
    throw std::runtime_error("指定日期和座位类型没有可用座位。");
}