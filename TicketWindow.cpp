#include "TicketWindow.h"
#include <QDateTime>

TicketWindow::TicketWindow(
    int currentUserId,
    Orders* currentOrder,
    bool isChangeMode,
    TrainSet* trainSet,
    SeatSet* seatSet,
    StationSet* stationSet,
    ScheduleSet* scheduleSet,
    OrderSet* orderSet,
    QWidget* parent
)
    : QDialog(parent),
    currentUserId(currentUserId),
    currentOrder(currentOrder),
    isChangeMode(isChangeMode),
    trainSet(trainSet),
    seatSet(seatSet),
    stationSet(stationSet),
    scheduleSet(scheduleSet),
    orderSet(orderSet)
{
    isOrderChanged = false;

    ui.setupUi(this);

    // 初始化车次选择框
    for (const auto& train : trainSet->GetAllTrains()) {
        ui.trainnameBox->addItem(train.GetTrainName());
    }

    // 初始化座位类型选择框
    for (const auto& seatType : seatSet->GetAllSeatTypes()) {
        ui.seatBox->addItem(seatType);
    }

    // 设置默认出发时间为当前时间的第二天 00:00
    ui.dateEdit->setCalendarPopup(true);
    ui.dateEdit->setMinimumDate(QDate::currentDate().addDays(1));
    ui.dateEdit->setMaximumDate(QDate::currentDate().addDays(15));
    

    // 手动触发默认选中车次的逻辑
    if (ui.trainnameBox->count() > 0) {
        onTrainSelectionChanged(ui.trainnameBox->currentText());
    }

    // 根据模式设置窗口标题和默认值
    if (isChangeMode) {
        setWindowTitle("改签窗口");

        // 设置默认值
        ui.trainnameBox->setCurrentText(trainSet->GetTrainNameById(currentOrder->GetTrainId()));
        ui.dateEdit->setDate(currentOrder->GetTravelDate());
        ui.seatBox->setCurrentText(seatSet->GetSeatTypeById(currentOrder->GetSeatId()));

        // 手动触发车次站点更新逻辑
        populateTrainStations(ui.trainnameBox->currentText());

        // 设置起点站和终点站
        ui.startstationBox->setCurrentText(stationSet->GetStationNameById(currentOrder->GetStartStationId()));
        updateEndStations(ui.startstationBox->currentText());
        ui.endstationBox->setCurrentText(stationSet->GetStationNameById(currentOrder->GetEndStationId()));
    }
    else {
        setWindowTitle("购票窗口");
    }

    // 手动触发余票更新逻辑
    updateSeatAvailability();

    // 连接信号槽
    connect(ui.trainnameBox, &QComboBox::currentTextChanged, this, &TicketWindow::onTrainSelectionChanged);
    connect(ui.buyButton, &QPushButton::clicked, this, &TicketWindow::onConfirmButtonClicked);
    connect(ui.cancelButton, &QPushButton::clicked, this, &TicketWindow::reject);

    connect(ui.startstationBox, &QComboBox::currentTextChanged, this, &TicketWindow::updateTicketPrice);
    connect(ui.endstationBox, &QComboBox::currentTextChanged, this, &TicketWindow::updateTicketPrice);
    connect(ui.seatBox, &QComboBox::currentTextChanged, this, &TicketWindow::updateTicketPrice);

    connect(ui.dateEdit, &QDateEdit::dateChanged, this, &TicketWindow::onDateChanged);
    connect(ui.trainnameBox, &QComboBox::currentTextChanged, this, &TicketWindow::updateSeatAvailability);
    connect(ui.seatBox, &QComboBox::currentTextChanged, this, &TicketWindow::updateSeatAvailability);

    connect(ui.startstationBox, &QComboBox::currentTextChanged, this, &TicketWindow::updateEndStations);

    connect(this, &TicketWindow::seatAvailabilityChanged, this, &TicketWindow::updateSeatAvailability);
}

TicketWindow::~TicketWindow()
{
    if (isChangeMode && isOrderChanged && currentOrder != nullptr) {
        currentOrder->SetStatus("canceled");
        orderSet->UpdateOrder(*currentOrder);
    }
}

void TicketWindow::onDateChanged(const QDate& date)
{
    // 检查日期是否在允许范围内
    if (date < QDate::currentDate().addDays(1) || date > QDate::currentDate().addDays(15)) {
        QMessageBox::warning(this, "日期无效", "请选择第二天到15天内的日期！");
        ui.dateEdit->setDate(QDate::currentDate().addDays(1)); // 重置为第二天
        return;
    }

    // 更新余票信息
    updateSeatAvailability();

    seatSet->SyncDailySeatAvailability(*orderSet);
}

void TicketWindow::updateTicketPrice()
{
    QString seatType = ui.seatBox->currentText();
    QString startStation = ui.startstationBox->currentText();
    QString endStation = ui.endstationBox->currentText();
    QString trainName = ui.trainnameBox->currentText();

    if (seatType.isEmpty() || startStation.isEmpty() || endStation.isEmpty() || trainName.isEmpty()) {
        ui.priceLabel->setText("未知");
        return;
    }

    try {
        int trainId = trainSet->GetTrainIdByName(trainName);
        int seatId = seatSet->GetSeatIdByType(seatType, trainId, *orderSet);
        int startStationId = stationSet->GetStationIdByName(startStation);
        int endStationId = stationSet->GetStationIdByName(endStation);

        int price = seatSet->GetDynamicPrice(seatId, startStationId, endStationId, *scheduleSet);
        ui.priceLabel->setText(QString("%1 元").arg(price));
    }
    catch (const std::exception& e) {
        ui.priceLabel->setText("无效的选择");
    }
}

void TicketWindow::updateSeatAvailability()
{
    QDate travelDate = ui.dateEdit->date();
    QString trainName = ui.trainnameBox->currentText();
    QString seatType = ui.seatBox->currentText();

    if (trainName.isEmpty() || seatType.isEmpty()) {
        ui.seatnumberlabel_t->setText("未知");
        return;
    }

    try {
        int trainId = trainSet->GetTrainIdByName(trainName);
        int availableSeats = seatSet->GetSeatAvailabilityByDate(travelDate, trainId, seatType, *orderSet);

        ui.seatnumberlabel_t->setText(QString("%1").arg(availableSeats));
    }
    catch (const std::exception& e) {
        ui.seatnumberlabel_t->setText("无效选择");
    }
}

void TicketWindow::onTrainSelectionChanged(const QString& trainName)
{
    populateTrainStations(trainName);
    updateTicketPrice();
}

void TicketWindow::populateTrainStations(const QString& trainName)
{
    int trainId = trainSet->GetTrainIdByName(trainName);
    auto schedules = scheduleSet->GetSchedulesByTrainId(trainId);

    ui.startstationBox->clear();
    ui.endstationBox->clear();

    stationList.clear();
    for (const auto& schedule : schedules) {
        QString stationName = stationSet->GetStationNameById(schedule.GetStationId());
        stationList.push_back(stationName);
        ui.startstationBox->addItem(stationName);
    }

    if (!stationList.empty()) {
        ui.startstationBox->setCurrentText(stationList.front());
        updateEndStations(stationList.front());
    }
}

void TicketWindow::updateEndStations(const QString& startStation)
{
    ui.endstationBox->clear();

    bool startFound = false;
    for (const auto& stationName : stationList) {
        if (startFound) {
            ui.endstationBox->addItem(stationName);
        }
        if (stationName == startStation) {
            startFound = true;
        }
    }

    if (ui.endstationBox->count() == 0) {
        ui.endstationBox->addItem("无效终点站");
    }
}

bool TicketWindow::isValidTrainAndStations(int trainId, int startStationId, int endStationId)
{
    if (startStationId == endStationId) {
        return false;
    }

    auto schedules = scheduleSet->GetSchedulesByTrainId(trainId);
    bool startFound = false, endFound = false;

    for (const auto& schedule : schedules) {
        if (schedule.GetStationId() == startStationId) {
            startFound = true;
        }
        if (startFound && schedule.GetStationId() == endStationId) {
            endFound = true;
            break;
        }
    }

    return startFound && endFound;
}

int TicketWindow::generateOrderId()
{
    int maxId = 0;
    for (const auto& order : orderSet->GetAllOrders()) {
        if (order.GetOrderId() > maxId) {
            maxId = order.GetOrderId();
        }
    }
    return maxId + 1;
}

void TicketWindow::onConfirmButtonClicked()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate travelDate = ui.dateEdit->date();

    QDate maxAllowedDate = currentDateTime.date().addDays(15);
    if (travelDate > maxAllowedDate) {
        QMessageBox::warning(this, "错误", "出发日期不能超过当前时间的 15 天后，请重新选择。");
        return;
    }

    QString trainName = ui.trainnameBox->currentText();
    QString seatType = ui.seatBox->currentText();
    QString startStation = ui.startstationBox->currentText();
    QString endStation = ui.endstationBox->currentText();

    if (trainName.isEmpty() || seatType.isEmpty() || startStation.isEmpty() || endStation.isEmpty()) {
        QMessageBox::warning(this, "错误", "请完整填写所有购票信息。");
        return;
    }

    try {
        int trainId = trainSet->GetTrainIdByName(trainName);
        int startStationId = stationSet->GetStationIdByName(startStation);
        int endStationId = stationSet->GetStationIdByName(endStation);

        if (!isValidTrainAndStations(trainId, startStationId, endStationId)) {
            QMessageBox::warning(this, "错误", "选择的车次或站点无效。");
            return;
        }

        int seatId = seatSet->GetSeatIdByDateAndType(travelDate, trainId, seatType);

        QTime trainStartTime;
        QTime trainEndTime;

        auto schedules = scheduleSet->GetSchedulesByTrainId(trainId);
        bool startFound = false, endFound = false;

        for (const auto& schedule : schedules) {
            if (schedule.GetStationId() == startStationId) {
                trainStartTime = schedule.GetDepartTime();
                startFound = true;
            }
            if (schedule.GetStationId() == endStationId) {
                trainEndTime = schedule.GetArrivalTime();
                endFound = true;
            }
            if (startFound && endFound) {
                break;
            }
        }

        if (!startFound || !endFound) {
            QMessageBox::warning(this, "错误", "无法获取所选路线的时间，请检查时刻表数据。");
            return;
        }

        QDateTime trainStartDateTime(travelDate, trainStartTime);
        if (currentDateTime >= trainStartDateTime) {
            QMessageBox::warning(this, "错误", "选择的车次已过期，请选择其他车次。");
            return;
        }
        if (orderSet->HasOrderInTimeRange(currentUserId, travelDate, trainStartTime, trainEndTime, *scheduleSet, currentOrder)) {
            QMessageBox::warning(this, "错误", "您已在该时间段内购买了其他车票，请选择其他时间段。");
            return;
        }

        seatSet->BookSeat(travelDate, seatId);

        int price = seatSet->GetDynamicPrice(seatId, startStationId, endStationId, *scheduleSet);

        Orders newOrder(
            generateOrderId(),
            currentUserId,
            trainId,
            seatId,
            startStationId,
            endStationId,
            travelDate,
            currentDateTime,
            "booked"
        );
        newOrder.SetPrice(price);

        orderSet->AddOrder(newOrder);
        isOrderChanged = true;

        QMessageBox::information(this, "购票成功", QString("您的车票已成功预订！\n座位号：%1").arg(seatSet->GetSeatNoById(seatId)));

        emit seatAvailabilityChanged(); // 触发信号

        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "错误", e.what());
    }
}
