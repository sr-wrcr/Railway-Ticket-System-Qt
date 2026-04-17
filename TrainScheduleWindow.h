#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QDateEdit>
#include "TrainSet.h"
#include "SeatSet.h"
#include "StationSet.h"
#include "ScheduleSet.h"
#include "OrderSet.h"

class TrainScheduleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrainScheduleWindow(
        TrainSet* trainSet,
        SeatSet* seatSet,
        StationSet* stationSet,
        ScheduleSet* scheduleSet,
        OrderSet* orderSet,
        QWidget* parent = nullptr
    );
    ~TrainScheduleWindow();

private:
    TrainSet* trainSet;
    SeatSet* seatSet;
    StationSet* stationSet;
    ScheduleSet* scheduleSet;
    OrderSet* orderSet;

    int currentTrainIndex;

    QLabel* trainNameLabel;       
    QLabel* seatInfoLabel;        
    QDateEdit* dateEdit;          
    QTableWidget* stationTable;   
    QPushButton* prevButton;      
    QPushButton* nextButton;      
	QPushButton* returnButton;    

    void loadTrainData(int trainIndex); 

private slots:
    void onPrevButtonClicked();  
    void onNextButtonClicked(); 
    void onDateChanged(const QDate& date);
	void onReturnButtonClicked(); 
};