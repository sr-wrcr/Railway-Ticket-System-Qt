#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include "ui_UserMainWindow.h"
#include "UserSet.h"
#include "TrainSet.h"
#include "SeatSet.h"
#include "StationSet.h"
#include "ScheduleSet.h"

class UserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserMainWindow(
        const User& user,
        TrainSet* trainSet,
        SeatSet* seatSet,
        StationSet* stationSet,
        ScheduleSet* scheduleSet,
        UserSet* userSet,
        OrderSet* orderSet,
        QWidget* parent = nullptr
    );

    ~UserMainWindow();

private slots:
    void onSearchButtonClicked();
    void onOrderButtonClicked();
    void onInformationButtonClicked();
    void onBuyButtonClicked();

private:
    Ui::UserMainWindowClass ui;
    User currentUser; // 存储当前用户信息

    TrainSet* trainSet;
    SeatSet* seatSet;
    StationSet* stationSet;
    ScheduleSet* scheduleSet;
    UserSet* userSet;
    OrderSet* orderSet;
};
