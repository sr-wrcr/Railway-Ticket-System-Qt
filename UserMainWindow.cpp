#include "UserMainWindow.h"
#include "LoginWindow.h"
#include <User.h>
#include "OrderWindow.h"
#include "InformationWindow.h"
#include "TicketWindow.h"
#include "TrainScheduleWindow.h"


UserMainWindow::UserMainWindow(
    const User& user,
    TrainSet* trainSet,
    SeatSet* seatSet,
    StationSet* stationSet,
    ScheduleSet* scheduleSet,
    UserSet* userSet,
    OrderSet* orderSet,
    QWidget* parent
)
    : QMainWindow(parent),
    currentUser(user),
    trainSet(trainSet),
    seatSet(seatSet),
    stationSet(stationSet),
    scheduleSet(scheduleSet),
    userSet(userSet),
    orderSet(orderSet)
{
    ui.setupUi(this);

    setWindowTitle("高铁订票系统 - 用户");
    
    connect(ui.searchButton, &QPushButton::clicked, this, &UserMainWindow::onSearchButtonClicked);
    connect(ui.orderButton, &QPushButton::clicked, this, &UserMainWindow::onOrderButtonClicked);
    connect(ui.informationButton, &QPushButton::clicked, this, &UserMainWindow::onInformationButtonClicked);
    connect(ui.buyButton, &QPushButton::clicked, this, &UserMainWindow::onBuyButtonClicked);

}

UserMainWindow::~UserMainWindow()
{

}

void UserMainWindow::onSearchButtonClicked()
{
    // 创建 TrainScheduleWindow 窗口并传递必要的数据集
    TrainScheduleWindow trainScheduleWindow(
        trainSet,    // 车次数据集
        seatSet,     // 座位数据集
        stationSet,  // 站点数据集
        scheduleSet, // 时刻表数据集
        orderSet,    // 订单数据集
        this         // 父窗口
    );

    // 显示窗口为模态对话框
    trainScheduleWindow.exec();
}

void UserMainWindow::onOrderButtonClicked()
{
    // 获取用户订单
    std::vector<Orders> userOrders = orderSet->GetOrdersByUserId(this->currentUser.GetUserId());

    // 检查是否有订单
    if (userOrders.empty()) {
        QMessageBox::information(this, "无订单", "您当前没有任何订单。");
        return; // 不创建 OrderWindow
    }

    OrderWindow orderWindow(
        this->currentUser.GetUserId(),
        trainSet,  
        userSet,   
        scheduleSet, 
        stationSet, 
        seatSet,   
		orderSet,
        this
    );
    orderWindow.exec();
}

void UserMainWindow::onInformationButtonClicked()
{
    InformationWindow infoWindow(this->currentUser.GetUserId(), userSet, this);
    infoWindow.exec();
}

void UserMainWindow::onBuyButtonClicked()
{
    TicketWindow ticketWindow(
        this->currentUser.GetUserId(),
        nullptr,
        false,
        trainSet,  
        seatSet,   
        stationSet,
        scheduleSet,
		orderSet,
        this
    );
    if (ticketWindow.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "购票成功", "您的车票已成功预订！");
    }
}
