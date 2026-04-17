#include "OrderWindow.h"
#include <QMessageBox>
#include "TicketWindow.h"
#include <QTimer>

OrderWindow::OrderWindow(
    int userId,
    TrainSet* trainSet,
    UserSet* userSet,
    ScheduleSet* scheduleSet,
    StationSet* stationSet,
    SeatSet* seatSet,
	OrderSet* orderSet,
    QWidget* parent
)
    : QDialog(parent),
    userId(userId),
    currentOrderIndex(0),
    trainSet(trainSet),
    userSet(userSet),
    scheduleSet(scheduleSet),
    stationSet(stationSet),
    seatSet(seatSet),
	orderSet(orderSet)
{
    ui.setupUi(this);

    ui.bookTimeEdit->setReadOnly(true);
    ui.startTimeEdit->setReadOnly(true);
    ui.endTimeEdit->setReadOnly(true);

    userOrders = orderSet->GetOrdersByUserId(userId);

    std::sort(userOrders.begin(), userOrders.end(), [this](const Orders& a, const Orders& b) {
        QDateTime startTimeA = QDateTime(a.GetTravelDate(), a.GetStartTime(*this->scheduleSet));
        QDateTime startTimeB = QDateTime(b.GetTravelDate(), b.GetStartTime(*this->scheduleSet));
        return startTimeA < startTimeB;
        });

    updateOrderDisplay();

    connect(ui.prevPageButton, &QPushButton::clicked, this, &OrderWindow::onPreviousButtonClicked);
    connect(ui.nextPageButton, &QPushButton::clicked, this, &OrderWindow::onNextButtonClicked);
    connect(ui.returnButton, &QPushButton::clicked, this, &OrderWindow::onReturnButtonClicked);
    connect(ui.refundButton, &QPushButton::clicked, this, &OrderWindow::onRefundButtonClicked);
	connect(ui.changeButton, &QPushButton::clicked, this, &OrderWindow::onChangeButtonClicked);
}

OrderWindow::~OrderWindow()
{
}

void OrderWindow::updateOrderDisplay()
{
    if (userOrders.empty() || currentOrderIndex < 0 || currentOrderIndex >= userOrders.size()) {
        return;
    }

    const Orders& order = userOrders[currentOrderIndex];
    
    ui.trainlabel_t->setText(trainSet->GetTrainNameById(order.GetTrainId()));
    ui.bookTimeEdit->setDateTime(order.GetOrderTime());
	ui.statuslabel_t->setText(order.GetStatus());
    ui.startstationlabel_t->setText(stationSet->GetStationNameById(order.GetStartStationId()));
	ui.endstationlabel_t->setText(stationSet->GetStationNameById(order.GetEndStationId()));
    ui.startTimeEdit->setDateTime(QDateTime(order.GetTravelDate(), order.GetStartTime(*scheduleSet)));
    ui.endTimeEdit->setDateTime(QDateTime(order.GetTravelDate(), order.GetEndTime(*scheduleSet)));
    ui.seatlabel_t->setText(seatSet->GetSeatNoById(order.GetSeatId()));
    ui.seatlabel_t_2->setText(seatSet->GetSeatTypeById(order.GetSeatId()));
    ui.seatlabel_t_3->setText(QString("票价: %1 元").arg(order.GetPrice()));
    ui.namelabel->setText(userSet->GetUserNameById(order.GetUserId()));
    ui.phonelabel->setText(userSet->GetPhoneById(order.GetUserId()));
    ui.IDcardlabel->setText(userSet->GetIdCardById(order.GetUserId()));

    QString orderStatus = userOrders[currentOrderIndex].GetStatus();

    // 根据订单状态设置按钮状态
    if (orderStatus == "canceled") {
        ui.refundButton->setEnabled(false);
        ui.changeButton->setEnabled(false);
    }
    else {
        ui.refundButton->setEnabled(true);
        ui.changeButton->setEnabled(true);
    }

    ui.prevPageButton->setEnabled(currentOrderIndex > 0);
    ui.nextPageButton->setEnabled(currentOrderIndex < userOrders.size() - 1);
}

void OrderWindow::onPreviousButtonClicked()
{
    if (currentOrderIndex > 0) {
        --currentOrderIndex;
        updateOrderDisplay();
    }
}

void OrderWindow::onNextButtonClicked()
{
    if (currentOrderIndex < userOrders.size() - 1) {
        ++currentOrderIndex;
        updateOrderDisplay();
    }
}

void OrderWindow::onReturnButtonClicked()
{
    close();
}

void OrderWindow::onChangeButtonClicked() {
    if (userOrders.empty() || currentOrderIndex < 0 || currentOrderIndex >= userOrders.size()) {
        QMessageBox::warning(this, "错误", "当前没有可改签的订单。");
        return;
    }

    Orders& orderToChange = userOrders[currentOrderIndex];

    TicketWindow ticketWindow(
        this->userId,
        &orderToChange,
        true,
        trainSet,
        seatSet,
        stationSet,
        scheduleSet,
        orderSet, 
        this
    );
    if (ticketWindow.exec() == QDialog::Accepted) {
        // 延迟刷新订单数据
        QTimer::singleShot(0, this, [this]() {
            userOrders = orderSet->GetOrdersByUserId(userId);
            updateOrderDisplay();
            });
    }
}

void OrderWindow::onRefundButtonClicked() {
    if (userOrders.empty() || currentOrderIndex < 0 || currentOrderIndex >= userOrders.size()) {
        QMessageBox::warning(this, "错误", "当前没有可退款的订单。");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认退款",
        "您确定要退款吗？",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::No) {
        return;
    }

    Orders& orderToRefund = userOrders[currentOrderIndex];

    // 更新订单状态为 "canceled"
    orderToRefund.SetStatus("canceled");

    // 更新共享的 OrderSet 对象，仅更新内存集合
    orderSet->UpdateOrder(orderToRefund);

    QMessageBox::information(this, "退款成功", "订单已成功退款。");

    // 更新订单显示
    updateOrderDisplay();

    // 触发事件驱动机制
    if (orderSet->onOrderChanged) {
        orderSet->onOrderChanged();
    }
}
