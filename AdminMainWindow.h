#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "ui_AdminMainWindow.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include "StationSet.h"
#include "TrainSet.h"
#include "UserSet.h"
#include "ScheduleSet.h"
#include "OrderSet.h"
#include "SeatSet.h"

enum class TableType {
    Station,
    Train,
    User,
    Schedule,
    Order,
	Seat,
    None
};


class AdminMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminMainWindow(
        TrainSet* trainSet,
        SeatSet* seatSet,
        StationSet* stationSet,
        ScheduleSet* scheduleSet,
        UserSet* userSet,
        OrderSet* orderSet,
        QWidget* parent = nullptr
    );
    ~AdminMainWindow();

private slots:
    void onStationButtonClicked();
    void onTrainButtonClicked();
    void onUserButtonClicked();
    void onScheduleButtonClicked();
    void onOrderButtonClicked();
    void onSeatButtonClicked();
    void onAddButtonClicked();
    void onTableItemChanged(QStandardItem* item);
    void onDeleteButtonClicked();
    void onExitButtonClicked();
	void onCustomContextMenuRequested(const QPoint& pos);
	void onEditButtonClicked();

private:
    Ui::AdminMainWindowClass ui;
    TableType currentTableType = TableType::None; // 当前表格类型
    QStandardItemModel* tableModel; // 用于保存表格模型

    QMenu* viewMenu;
    QAction* stationAction;
    QAction* trainAction;
    QAction* userAction;
    QAction* scheduleAction;
    QAction* orderAction;
    QAction* exitAction;
	QAction* seatAction;

    StationSet* stationSet;
    TrainSet* trainSet;
    UserSet* userSet;
    ScheduleSet* scheduleSet;
    OrderSet* orderSet;
	SeatSet* seatSet;

    QMenu* contextMenu; 
    QAction* addAction;
    QAction* editAction;
    QAction* deleteAction;
};
