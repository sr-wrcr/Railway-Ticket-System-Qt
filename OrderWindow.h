#pragma once

#include <QDialog>
#include "ui_OrderWindow.h"
#include "OrderSet.h"
#include "TrainSet.h"
#include "UserSet.h"
#include "ScheduleSet.h"
#include "StationSet.h"
#include "SeatSet.h"

class OrderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OrderWindow(
        int userId,
        TrainSet* trainSet,
        UserSet* userSet,
        ScheduleSet* scheduleSet,
        StationSet* stationSet,
        SeatSet* seatSet,
        OrderSet* orderSet,
        QWidget* parent = nullptr
    );

    ~OrderWindow();

private slots:
    void onPreviousButtonClicked();
    void onNextButtonClicked();
    void onReturnButtonClicked();
	void onRefundButtonClicked();
	void onChangeButtonClicked();

private:
    void updateOrderDisplay();

    Ui::OrderWindowClass ui;
    int userId;
    int currentOrderIndex;
    std::vector<Orders> userOrders;

    TrainSet* trainSet;
    UserSet* userSet;
    ScheduleSet* scheduleSet;
    StationSet* stationSet;
    SeatSet* seatSet;
	OrderSet* orderSet;

};