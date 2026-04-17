#pragma once

#include "ui_TicketWindow.h"
#include "OrderSet.h"
#include "SeatSet.h"
#include "TrainSet.h"
#include "StationSet.h"
#include "ScheduleSet.h"
#include "Orders.h"
#include <QDialog>
#include <QDateTime>
#include <QMessageBox>


class TicketWindow : public QDialog {
    Q_OBJECT

public:
    explicit TicketWindow(
        int currentUserId,
        Orders* currentOrder = nullptr,
        bool isChangeMode = false,
        TrainSet* trainSet = nullptr,
        SeatSet* seatSet = nullptr,
        StationSet* stationSet = nullptr,
        ScheduleSet* scheduleSet = nullptr,
        OrderSet* orderSet = nullptr,
        QWidget* parent = nullptr
    );
    ~TicketWindow();

signals:
    void seatAvailabilityChanged();

private:
    Ui::TicketWindowClass ui;
    Orders* currentOrder; 
    bool isChangeMode; // 是否为改签模式
    int currentUserId; 
    TrainSet* trainSet; 
    SeatSet* seatSet; 
    StationSet* stationSet; 
    ScheduleSet* scheduleSet; 
    OrderSet* orderSet; 
    std::vector<QString> stationList;
    bool isOrderChanged; // 标记是否生成了新订单

    void populateTrainStations(const QString& trainName);
    bool isValidTrainAndStations(int trainId, int startStationId, int endStationId); 
    int generateOrderId(); 
    void updateTicketPrice();
    void updateSeatAvailability();
    void updateEndStations(const QString& startStation);

private slots:
    void onConfirmButtonClicked(); 
    void onTrainSelectionChanged(const QString& trainName); 
	void onDateChanged(const QDate& date);
};