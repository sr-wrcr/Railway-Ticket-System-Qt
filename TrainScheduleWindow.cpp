#include "TrainScheduleWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateEdit>
#include <QMessageBox>

TrainScheduleWindow::TrainScheduleWindow(
    TrainSet* trainSet,
    SeatSet* seatSet,
    StationSet* stationSet,
    ScheduleSet* scheduleSet,
    OrderSet* orderSet,
    QWidget* parent
)
    : QDialog(parent),
    trainSet(trainSet),
    seatSet(seatSet),
    stationSet(stationSet),
    scheduleSet(scheduleSet),
    orderSet(orderSet),
    currentTrainIndex(0)
{
    setWindowTitle("车次时刻表");
    resize(500, 450);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 顶部区域：车次名称、座位信息和日期选择
    QHBoxLayout* topLayout = new QHBoxLayout();
    trainNameLabel = new QLabel(this);
    seatInfoLabel = new QLabel(this);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumDate(QDate::currentDate().addDays(1));
    dateEdit->setMaximumDate(QDate::currentDate().addDays(15));
    topLayout->addWidget(trainNameLabel);
    topLayout->addWidget(seatInfoLabel);
    topLayout->addWidget(new QLabel("选择日期：", this));
    topLayout->addWidget(dateEdit);
    mainLayout->addLayout(topLayout);

    // 中间区域：站点信息表格
    stationTable = new QTableWidget(this);
    stationTable->setColumnCount(3);
    stationTable->setHorizontalHeaderLabels({ "站点名称", "到达时间", "出发时间" });
    stationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    stationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stationTable->setSelectionMode(QAbstractItemView::NoSelection);
    mainLayout->addWidget(stationTable);

    // 底部区域：分页按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    prevButton = new QPushButton("上一页", this);
    nextButton = new QPushButton("下一页", this);
    returnButton = new QPushButton("返回", this);
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(returnButton);

    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(prevButton, &QPushButton::clicked, this, &TrainScheduleWindow::onPrevButtonClicked);
    connect(nextButton, &QPushButton::clicked, this, &TrainScheduleWindow::onNextButtonClicked);
    connect(returnButton, &QPushButton::clicked, this, &TrainScheduleWindow::onReturnButtonClicked);
    connect(dateEdit, &QDateEdit::dateChanged, this, &TrainScheduleWindow::onDateChanged);

    // 加载初始数据
    loadTrainData(currentTrainIndex);
}

TrainScheduleWindow::~TrainScheduleWindow()
{
}

void TrainScheduleWindow::loadTrainData(int trainIndex)
{
    // 获取当前车次
    const auto& trains = trainSet->GetAllTrains();
    if (trainIndex < 0 || trainIndex >= trains.size()) {
        return;
    }

    const Trains& train = trains[trainIndex];
    trainNameLabel->setText(QString("车次：%1").arg(train.GetTrainName()));

    // 获取座位信息
    QDate selectedDate = dateEdit->date();
    QString seatInfo = QString("商务座：%1张，¥%2 | 一等座：%3张，¥%4 | 二等座：%5张，¥%6")
        .arg(seatSet->GetSeatAvailabilityByDate(selectedDate, train.GetTrainId(), "商务", *orderSet))
        .arg(seatSet->GetSeatPrice("商务"))
        .arg(seatSet->GetSeatAvailabilityByDate(selectedDate, train.GetTrainId(), "一等", *orderSet))
        .arg(seatSet->GetSeatPrice("一等"))
        .arg(seatSet->GetSeatAvailabilityByDate(selectedDate, train.GetTrainId(), "二等", *orderSet))
        .arg(seatSet->GetSeatPrice("二等"));
    seatInfoLabel->setText(seatInfo);

    // 获取站点信息
    auto schedules = scheduleSet->GetSchedulesByTrainId(train.GetTrainId());
    stationTable->setRowCount(schedules.size());
    for (int i = 0; i < schedules.size(); ++i) {
        stationTable->setItem(i, 0, new QTableWidgetItem(stationSet->GetStationNameById(schedules[i].GetStationId())));
        stationTable->setItem(i, 1, new QTableWidgetItem(schedules[i].GetArrivalTime().toString("HH:mm")));
        stationTable->setItem(i, 2, new QTableWidgetItem(schedules[i].GetDepartTime().toString("HH:mm")));
    }

    // 更新分页按钮状态
    prevButton->setEnabled(trainIndex > 0);
    nextButton->setEnabled(trainIndex < trains.size() - 1);
}

void TrainScheduleWindow::onPrevButtonClicked()
{
    if (currentTrainIndex > 0) {
        --currentTrainIndex;
        loadTrainData(currentTrainIndex);
    }
}

void TrainScheduleWindow::onNextButtonClicked()
{
    if (currentTrainIndex < trainSet->GetAllTrains().size() - 1) {
        ++currentTrainIndex;
        loadTrainData(currentTrainIndex);
    }
}

void TrainScheduleWindow::onDateChanged(const QDate& date)
{
    // 检查日期是否在允许范围内
    if (date <= QDate::currentDate() || date > QDate::currentDate().addDays(15)) {
        QMessageBox::warning(this, "日期无效", "请选择第二天日期到15天内的日期！");
        dateEdit->setDate(QDate::currentDate().addDays(1)); // 重置为当前日期
        return;
    }

    // 当日期更改时，重新加载当前车次数据
    loadTrainData(currentTrainIndex);
}

void TrainScheduleWindow::onReturnButtonClicked()
{
    close();
}
