#include "AdminMainWindow.h"
#include <QMessageBox>
#include <QTime>
#include <QInputDialog>

AdminMainWindow::AdminMainWindow(
    TrainSet* trainSet,
    SeatSet* seatSet,
    StationSet* stationSet,
    ScheduleSet* scheduleSet,
    UserSet* userSet,
    OrderSet* orderSet,
    QWidget* parent
)
    : QMainWindow(parent),
    trainSet(trainSet),
    seatSet(seatSet),
    stationSet(stationSet),
    scheduleSet(scheduleSet),
    userSet(userSet),
    orderSet(orderSet),
    tableModel(nullptr)
{
    ui.setupUi(this);

    setWindowTitle("高铁订票系统 - 管理员");

	tableModel = nullptr; 

    viewMenu = menuBar()->addMenu("菜单");

    exitAction = new QAction("退出", this); 

    stationAction = new QAction("站点管理", this);
    trainAction = new QAction("列车管理", this);
    userAction = new QAction("用户管理", this);
    scheduleAction = new QAction("时刻表管理", this);
    orderAction = new QAction("订单管理", this);
    seatAction = new QAction("座位管理", this);

    viewMenu->addAction(stationAction);
    viewMenu->addAction(trainAction);
    viewMenu->addAction(userAction);
    viewMenu->addAction(scheduleAction);
    viewMenu->addAction(orderAction);
    viewMenu->addAction(seatAction); 

    menuBar()->addAction(exitAction);

    connect(stationAction, &QAction::triggered, this, &AdminMainWindow::onStationButtonClicked);
    connect(trainAction, &QAction::triggered, this, &AdminMainWindow::onTrainButtonClicked);
    connect(userAction, &QAction::triggered, this, &AdminMainWindow::onUserButtonClicked);
    connect(scheduleAction, &QAction::triggered, this, &AdminMainWindow::onScheduleButtonClicked);
    connect(orderAction, &QAction::triggered, this, &AdminMainWindow::onOrderButtonClicked);
    connect(seatAction, &QAction::triggered, this, &AdminMainWindow::onSeatButtonClicked); 

    connect(exitAction, &QAction::triggered, this, &AdminMainWindow::onExitButtonClicked); // 连接退出按钮到槽函数

    contextMenu = new QMenu(this);
    deleteAction = new QAction("删除", this);

    // 仅在非订单管理时添加“修改”和“添加”操作
    if (currentTableType != TableType::Order) {
        addAction = new QAction("添加", this);
        editAction = new QAction("修改", this);
        contextMenu->addAction(addAction);
        contextMenu->addAction(editAction);
    }
    contextMenu->addAction(deleteAction);

    ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.tableView, &QTableView::customContextMenuRequested, this, &AdminMainWindow::onCustomContextMenuRequested);
    connect(addAction, &QAction::triggered, this, &AdminMainWindow::onAddButtonClicked);
    connect(editAction, &QAction::triggered, this, &AdminMainWindow::onEditButtonClicked);
    connect(deleteAction, &QAction::triggered, this, &AdminMainWindow::onDeleteButtonClicked);
}

AdminMainWindow::~AdminMainWindow()
{
}

void AdminMainWindow::onCustomContextMenuRequested(const QPoint& pos)
{
    QModelIndex index = ui.tableView->indexAt(pos);

    if (currentTableType == TableType::Order) {
        int column = index.column();

        if (addAction) addAction->setEnabled(false);
        if (column == 8) {
            if (editAction) {
                editAction->setText("切换");
                editAction->setEnabled(true);
            }
        }
        else {
            if (editAction) {
                editAction->setText("修改");
                editAction->setEnabled(false);
            }
        }
        if (deleteAction) deleteAction->setEnabled(false);

    }
    else if (currentTableType == TableType::User) {
        int row = index.row();
        int column = index.column();

        // 禁用操作
        if (deleteAction) deleteAction->setEnabled(false);
        if (addAction) addAction->setEnabled(false);

        // 第一行（管理员信息行）允许编辑所有列
        if (row == 0) {
            if (editAction) {
                editAction->setText("修改");
                editAction->setEnabled(true);
            }
        }
        // 其他行仅允许编辑密码列（列号为3）
        else {
            if (column == 3) {
                if (editAction) {
                    editAction->setText("重置");
                    editAction->setEnabled(true);
                }
            }
            else {
                if (editAction) {
                    editAction->setText("修改");
                    editAction->setEnabled(false);
                }
            }
        }
    }
    else if (currentTableType == TableType::Seat) {
        int column = index.column();

        if (addAction) addAction->setEnabled(false);
        if (column == 4) {
            if (editAction) {
                editAction->setText("修改");
                editAction->setEnabled(true);
            }
        }
        else {
            if (editAction) {
                editAction->setText("修改");
                editAction->setEnabled(false);
            }
		}
        if (deleteAction) deleteAction->setEnabled(false);
    }
    else {
        if (addAction) addAction->setEnabled(true);
        if (editAction) {
            editAction->setText("修改");
            editAction->setEnabled(index.isValid());
        }
        if (deleteAction) deleteAction->setEnabled(false);

    }

    contextMenu->exec(ui.tableView->viewport()->mapToGlobal(pos));
}

void AdminMainWindow::onEditButtonClicked()
{

    QModelIndex index = ui.tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请选择要修改的项！");
        return;
    }
    if (currentTableType == TableType::Seat) {
        int column = index.column();

        // 如果是价格列（假设价格列为第4列）
        if (column == 4) {
            // 获取所有座位类型
            std::vector<QString> seatTypesVector = seatSet->GetAllSeatTypes();

            QStringList seatTypes;
            for (const auto& seatType : seatTypesVector) {
                seatTypes.append(seatType);
            }

            // 弹出选择框，选择座位类型
            bool ok;
            QString selectedSeatType = QInputDialog::getItem(
                this, "选择座位类型", "请选择要修改的座位类型：", seatTypes, 0, false, &ok);

            if (!ok || selectedSeatType.isEmpty()) {
                QMessageBox::information(this, "操作取消", "未选择座位类型，操作已取消。");
                return;
            }

            // 弹出输入框，输入新的价格
            int newPrice = QInputDialog::getInt(
                this, "输入新价格", "请输入新的价格：", 0, 0, 10000, 1, &ok);

            if (!ok) {
                QMessageBox::information(this, "操作取消", "未输入价格，操作已取消。");
                return;
            }

            // 更新所有相同类型座位的价格
            auto seats = seatSet->GetAllSeats();
            for (auto& seat : seats) {
                if (seat.GetSeatType() == selectedSeatType) {
                    seat.SetPrice(newPrice);
                }
            }
            seatSet->SetSeats(seats);

            // 刷新表格显示
            onSeatButtonClicked();

            QMessageBox::information(this, "修改成功", QString("所有 %1 类型座位的价格已修改为 %2 元。")
                .arg(selectedSeatType)
                .arg(newPrice));
        }
    }
    if (currentTableType == TableType::Order) {
        int column = index.column();

        // 如果是状态列（假设状态列为第8列），执行状态切换逻辑
        if (column == 8 && editAction->text() == "切换") {
            auto orders = orderSet->GetAllOrders();
            int row = index.row();

            if (row < orders.size()) {
                Orders& order = orders[row];
                QString currentStatus = order.GetStatus();

                // 切换状态
                if (currentStatus == "booked") {
                    order.SetStatus("canceled");
                } else if (currentStatus == "canceled") {
                    order.SetStatus("booked");
                }

                // 更新订单数据
                orderSet->SetOrders(orders);

                // 刷新表格显示
                onOrderButtonClicked();

                QMessageBox::information(this, "状态切换", "订单状态已成功切换！");
            }
            return;
        }

        QMessageBox::warning(this, "操作无效", "订单管理中仅支持切换状态列。");
        return;
    }

    ui.tableView->edit(index);
}


void AdminMainWindow::onStationButtonClicked()
{
    currentTableType = TableType::Station;

    if (tableModel) {
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
    }
    tableModel = new QStandardItemModel(this);
    tableModel->setHorizontalHeaderLabels({ "站点ID", "站名" });

    for (const auto& station : stationSet->GetAllStations()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(station.GetStationId()));
        row << new QStandardItem(station.GetStationName());
        tableModel->appendRow(row);
    }

    ui.tableView->setModel(tableModel);
    ui.tableView->reset();
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑

    connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);

}

void AdminMainWindow::onTrainButtonClicked()
{
    currentTableType = TableType::Train;

    if (tableModel) {
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
    }
    tableModel = new QStandardItemModel(this);
    tableModel->setHorizontalHeaderLabels({ "列车ID", "列车名字", "列车类型", "起始站", "终点站" });

    for (const auto& train : trainSet->GetAllTrains()) {
        QList<QStandardItem*> row;

        QString startStationName = stationSet->GetStationNameById(train.GetStartStationId());
        QString endStationName = stationSet->GetStationNameById(train.GetEndStationId());

        row << new QStandardItem(QString::number(train.GetTrainId()));
        row << new QStandardItem(train.GetTrainName());
        row << new QStandardItem(train.GetTrainType());
        row << new QStandardItem(startStationName);
        row << new QStandardItem(endStationName);

        tableModel->appendRow(row);
    }

    ui.tableView->setModel(tableModel);
    ui.tableView->reset();
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑

    connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);

}

void AdminMainWindow::onUserButtonClicked()
{

	currentTableType = TableType::User;

    if (tableModel) {
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
    }
    tableModel = new QStandardItemModel(this);
    tableModel->setHorizontalHeaderLabels({ "用户ID", "姓名", "用户名", "密码", "手机号码", "身份", "身份证号码" });

    for (const auto& user : userSet->GetAllUsers()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(user.GetUserId()));
        row << new QStandardItem(user.GetUserName());
        row << new QStandardItem(user.GetDisplayName());
        row << new QStandardItem(user.GetPassword());
        row << new QStandardItem(user.GetPhone());
        row << new QStandardItem(user.GetRole());
        row << new QStandardItem(user.GetIdCard());
        tableModel->appendRow(row);
    }

    ui.tableView->setModel(tableModel);
    ui.tableView->reset();
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑

    connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);

}

void AdminMainWindow::onScheduleButtonClicked()
{
	currentTableType = TableType::Schedule;

    if (tableModel) {
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
    }
    tableModel = new QStandardItemModel(this);
    tableModel->setHorizontalHeaderLabels({ "时刻ID", "火车名", "站名", "经停序号", "抵达时间", "出发时间" });

    for (const auto& schedule : scheduleSet->GetAllSchedules()) {
        QList<QStandardItem*> row;

        QString trainName = trainSet->GetTrainNameById(schedule.GetTrainId());
        QString stationName = stationSet->GetStationNameById(schedule.GetStationId());

        row << new QStandardItem(QString::number(schedule.GetScheduleId()));
        row << new QStandardItem(trainName);
        row << new QStandardItem(stationName);
        row << new QStandardItem(QString::number(schedule.GetStopOrder()));
        row << new QStandardItem(schedule.GetArrivalTime().toString("HH:mm"));
        row << new QStandardItem(schedule.GetDepartTime().toString("HH:mm"));

        tableModel->appendRow(row);
    }

    ui.tableView->setModel(tableModel);
    ui.tableView->reset();
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑

    connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
}

void AdminMainWindow::onOrderButtonClicked()
{
	currentTableType = TableType::Order;

    if (tableModel) {
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
    }
    tableModel = new QStandardItemModel(this);
    tableModel->setHorizontalHeaderLabels({ "订单ID", "用户姓名", "列车名", "座位序号", "起始站", "终点站", "出发时间", "预定时间", "订单状态", "价格"});

    for (const auto& order : orderSet->GetAllOrders()) {
        QList<QStandardItem*> row;

        QString userName = userSet->GetUserNameById(order.GetUserId());
        QString trainName = trainSet->GetTrainNameById(order.GetTrainId());
        QString startStationName = stationSet->GetStationNameById(order.GetStartStationId());
        QString endStationName = stationSet->GetStationNameById(order.GetEndStationId());
		QString seatNo = seatSet->GetSeat(order.GetSeatId()).GetSeatNo();
        row << new QStandardItem(QString::number(order.GetOrderId()));
        row << new QStandardItem(userName);
        row << new QStandardItem(trainName);
        row << new QStandardItem(seatNo);
        row << new QStandardItem(startStationName);
        row << new QStandardItem(endStationName);
        row << new QStandardItem(order.GetTravelDate().toString("yyyy-MM-dd"));
        row << new QStandardItem(order.GetOrderTime().toString("yyyy-MM-dd HH:mm:ss"));
        row << new QStandardItem(order.GetStatus());
        row << new QStandardItem(QString("%1 元").arg(order.GetPrice())); 

        tableModel->appendRow(row);
    }

    ui.tableView->setModel(tableModel);
    ui.tableView->reset();
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminMainWindow::onAddButtonClicked()
{
    if (currentTableType == TableType::Order) {
        QMessageBox::warning(this, "操作无效", "订单管理中不支持添加操作。");
        return;
    }

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView->model());
    if (!model) return;

    QList<QStandardItem*> newRow;
    switch (currentTableType) {
    case TableType::Station: {
        int newId = static_cast<int>(stationSet->GetAllStations().size()) + 1; 
        stationSet->AddStation(Stations(newId, "")); 
        newRow << new QStandardItem(QString::number(newId));
        newRow << new QStandardItem(""); 
        break;
    }
    case TableType::Train: {
        int newId = static_cast<int>(trainSet->GetAllTrains().size()) + 1;
        trainSet->AddTrain(Trains(newId, "", "", 0, 0)); 
        newRow << new QStandardItem(QString::number(newId));
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        break;
    }
    case TableType::Schedule: {
        int newId = static_cast<int>(scheduleSet->GetAllSchedules().size()) + 1;
        scheduleSet->AddSchedule(Schedule(newId, 0, 0, 0, QTime(), QTime())); 
        newRow << new QStandardItem(QString::number(newId));
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        newRow << new QStandardItem(""); 
        break;
    }
    case TableType::Order: {
        int newId = static_cast<int>(orderSet->GetAllOrders().size()) + 1;
        orderSet->AddOrder(Orders(newId, 0, 0, 0, 0, 0, QDate(), QDateTime(), ""));
        newRow << new QStandardItem(QString::number(newId));
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        newRow << new QStandardItem("");
        break;
    }
    default:
        return;
    }

    model->appendRow(newRow);
}

void AdminMainWindow::onDeleteButtonClicked()
{

    if (currentTableType == TableType::Order) {
        QMessageBox::warning(this, "操作无效", "订单管理中不支持删除操作。");
        return;
    }

    if (currentTableType == TableType::User) {
        QMessageBox::warning(this, "操作无效", "用户管理中不支持删除操作。");
        return;
    }

    QItemSelectionModel* selectionModel = ui.tableView->selectionModel();
    if (!selectionModel->hasSelection()) return;

    QModelIndex index = selectionModel->currentIndex();
    int row = index.row();

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.tableView->model());
    if (!model) return;

    model->removeRow(row);

    switch (currentTableType) {
    case TableType::Station: {
        auto stations = stationSet->GetAllStations();
        if (row < stations.size()) {
            stations.erase(stations.begin() + row);
            stationSet->SetStations(stations); // 更新内存集合
        }
        break;
    }
    case TableType::Train: {
        auto trains = trainSet->GetAllTrains();
        if (row < trains.size()) {
            trains.erase(trains.begin() + row);
            trainSet->SetTrains(trains); // 更新内存集合
        }
        break;
    }
    case TableType::Schedule: {
        auto schedules = scheduleSet->GetAllSchedules();
        if (row < schedules.size()) {
            schedules.erase(schedules.begin() + row);
            scheduleSet->SetSchedules(schedules); // 更新内存集合
        }
        break;
    }
    default:
        break;
    }
}

void AdminMainWindow::onExitButtonClicked()
{
    this->close();
}

void AdminMainWindow::onTableItemChanged(QStandardItem* item)
{
    if (!item) return;

    int row = item->row();
    int column = item->column();

    if (column < 0 || column >= tableModel->columnCount()) {
        return; // 防止访问无效列
    }

    QString newValue = item->text();

    // 检查是否为空字段
    if (newValue.trimmed().isEmpty()) {
        QMessageBox::warning(this, "无效输入", "字段不能为空！");

        // 恢复为修改前的值
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
        QString oldValue;

        switch (currentTableType) {
        case TableType::Station: {
            auto stations = stationSet->GetAllStations();
            if (row < stations.size()) {
                if (column == 0) {
                    oldValue = QString::number(stations[row].GetStationId());
                } else if (column == 1) {
                    oldValue = stations[row].GetStationName();
                }
            }
            break;
        }
        case TableType::Train: {
            auto trains = trainSet->GetAllTrains();
            if (row < trains.size()) {
                if (column == 0) {
                    oldValue = QString::number(trains[row].GetTrainId());
                } else if (column == 1) {
                    oldValue = trains[row].GetTrainName();
                } else if (column == 2) {
                    oldValue = trains[row].GetTrainType();
                } else if (column == 3) {
                    oldValue = QString::number(trains[row].GetStartStationId());
                } else if (column == 4) {
                    oldValue = QString::number(trains[row].GetEndStationId());
                }
            }
            break;
        }
        case TableType::User: {
            auto users = userSet->GetAllUsers();
            if (row < users.size()) {
                const User& user = users[row];
                switch (column) {
                case 0: oldValue = QString::number(user.GetUserId()); break;
                case 1: oldValue = user.GetUserName(); break;
                case 2: oldValue = user.GetDisplayName(); break;
                case 3: oldValue = user.GetPassword(); break;
                case 4: oldValue = user.GetPhone(); break;
                case 5: oldValue = user.GetRole(); break;
                case 6: oldValue = user.GetIdCard(); break;
                default: break;
                }
            }
            break;
        }
        case TableType::Schedule: {
            auto schedules = scheduleSet->GetAllSchedules();
            if (row < schedules.size()) {
                if (column == 0) {
                    oldValue = QString::number(schedules[row].GetScheduleId());
                } else if (column == 1) {
                    oldValue = QString::number(schedules[row].GetTrainId());
                } else if (column == 2) {
                    oldValue = QString::number(schedules[row].GetStationId());
                } else if (column == 3) {
                    oldValue = QString::number(schedules[row].GetStopOrder());
                } else if (column == 4) {
                    oldValue = schedules[row].GetArrivalTime().toString("HH:mm");
                } else if (column == 5) {
                    oldValue = schedules[row].GetDepartTime().toString("HH:mm");
                }
            }
            break;
        }
        case TableType::Order: {
            auto orders = orderSet->GetAllOrders();
            if (row < orders.size()) {
                if (column == 0) {
                    oldValue = QString::number(orders[row].GetOrderId());
                } else if (column == 1) {
                    oldValue = QString::number(orders[row].GetUserId());
                } else if (column == 2) {
                    oldValue = QString::number(orders[row].GetTrainId());
                } else if (column == 3) {
                    oldValue = QString::number(orders[row].GetSeatId());
                } else if (column == 4) {
                    oldValue = QString::number(orders[row].GetStartStationId());
                } else if (column == 5) {
                    oldValue = QString::number(orders[row].GetEndStationId());
                } else if (column == 6) {
                    oldValue = orders[row].GetTravelDate().toString("yyyy-MM-dd");
                } else if (column == 7) {
                    oldValue = orders[row].GetOrderTime().toString("yyyy-MM-dd HH:mm:ss");
                } else if (column == 8) {
                    oldValue = orders[row].GetStatus();
                }
            }
            break;
        }
        default:
            break;
        }

        item->setText(oldValue); // 恢复为修改前的值
        connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
        return;
    }

    disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);

    // 更新数据逻辑
    switch (currentTableType) {
    case TableType::Station: {
        auto stations = stationSet->GetAllStations();
        if (row < stations.size()) {
            if (column == 0) {
                stations[row].SetStationId(newValue.toInt());
            } else if (column == 1) {
                stations[row].SetStationName(newValue);
            }
            stationSet->SetStations(stations); // 更新内存集合
        }
        break;
    }
    case TableType::Train: {
        auto trains = trainSet->GetAllTrains();
        if (row < trains.size()) {
            if (column == 0) {
                trains[row].SetTrainId(newValue.toInt());
            }
            else if (column == 1) {
                trains[row].SetTrainName(newValue);
            }
            else if (column == 2) {
                trains[row].SetTrainType(newValue);
            }
            else if (column == 3) {
                trains[row].SetStartStationId(newValue.toInt());
            }
            else if (column == 4) {
                trains[row].SetEndStationId(newValue.toInt());
            }
            trainSet->SetTrains(trains); // 更新内存集合
        }
        break;
    }
    case TableType::User: {
        auto users = userSet->GetAllUsers();
        if (row < users.size()) {
            User& user = users[row];

            if (user.GetRole() == "admin") {
                switch (column) {
                case 0: user.SetUserId(newValue.toInt()); break;
                case 1: user.SetUserName(newValue); break;
                case 2: user.SetDisplayName(newValue); break;
                case 3: user.SetPassword(newValue); break;
                case 4: user.SetPhone(newValue); break;
                case 5: user.SetRole(newValue); break;
                case 6: user.SetIdCard(newValue); break;
                default: break;
                }
            }
            else {
                if (column == 3) { // 密码列
                    // 弹出确认对话框
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "确认重置密码",
                        "是否将密码重置为 123abc？",
                        QMessageBox::Yes | QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        user.SetPassword("123abc");
                        QMessageBox::information(this, "密码重置", "密码已重置为 123abc。");
                    }
                    else {
                        item->setText(user.GetPassword()); // 恢复原密码
                    }
                }
                else {
                    QMessageBox::warning(this, "权限不足", "管理员仅可重置用户密码为123abc。");
                    item->setText(""); // 恢复为空
                }

            }

            userSet->SetUsers(users); // 更新用户数据
			//刷新表格显示
            onUserButtonClicked();

        }
        break;
    }
    case TableType::Schedule: {
        auto schedules = scheduleSet->GetAllSchedules();
        if (row < schedules.size()) {
            if (column == 0) {
                schedules[row].SetScheduleId(newValue.toInt());
            }
            else if (column == 1) {
                schedules[row].SetTrainId(newValue.toInt());
            }
            else if (column == 2) {
                schedules[row].SetStationId(newValue.toInt());
            }
            else if (column == 3) {
                schedules[row].SetStopOrder(newValue.toInt());
            }
            else if (column == 4) {
                QTime time = QTime::fromString(newValue, "HH:mm");
                if (time.isValid()) {
                    schedules[row].SetArrivalTime(time);
                }
                else {
                    QMessageBox::warning(this, "错误输入", "以HH:mm格式输入时间");
                }
            }
            else if (column == 5) {
                QTime time = QTime::fromString(newValue, "HH:mm");
                if (time.isValid()) {
                    schedules[row].SetDepartTime(time);
                }
                else {
                    QMessageBox::warning(this, "错误输入", "以HH:mm格式输入时间");
                }
            }
            scheduleSet->SetSchedules(schedules); // 更新内存集合
        }
        break;
    }
    case TableType::Order: {
        auto orders = orderSet->GetAllOrders();
        if (row < orders.size()) {
            if (column == 0) {
                orders[row].SetOrderId(newValue.toInt());
            }
            else if (column == 1) {
                orders[row].SetUserId(newValue.toInt());
            }
            else if (column == 2) {
                orders[row].SetTrainId(newValue.toInt());
            }
            else if (column == 3) {
                orders[row].SetSeatId(newValue.toInt());
            }
            else if (column == 4) {
                orders[row].SetStartStationId(newValue.toInt());
            }
            else if (column == 5) {
                orders[row].SetEndStationId(newValue.toInt());
            }
            else if (column == 6) {
                QDate date = QDate::fromString(newValue, "yyyy-MM-dd");
                if (date.isValid()) {
                    orders[row].SetTravelDate(date);
                }
                else {
                    QMessageBox::warning(this, "错误输入", "以yyyy-MM-dd格式输入日期");
                }
            }
            else if (column == 7) {
                QDateTime dateTime = QDateTime::fromString(newValue, "yyyy-MM-dd HH:mm:ss");
                if (dateTime.isValid()) {
                    orders[row].SetOrderTime(dateTime);
                }
                else {
                    QMessageBox::warning(this, "错误输入", "以yyyy-MM-dd HH:mm:ss格式输入日期时间");
                }
            }
            else if (column == 8) {
                orders[row].SetStatus(newValue);
            }
            orderSet->SetOrders(orders); // 更新内存集合
        }
        break;
    }
    default:
        break;
    }

    connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);

}

void AdminMainWindow::onSeatButtonClicked()
{
    currentTableType = TableType::Seat; // 设置当前表格类型为 Seat

    if (tableModel) {
        disconnect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
    }
    tableModel = new QStandardItemModel(this);
    tableModel->setHorizontalHeaderLabels({ "座位ID", "列车名", "座位类型", "座位序号", "定价" });

    for (const auto& seat : seatSet->GetAllSeats()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(seat.GetSeatId()));
        QString trainName = trainSet->GetTrainNameById(seat.GetTrainId());
        row << new QStandardItem(trainName); 
        row << new QStandardItem(seat.GetSeatType());
        row << new QStandardItem(seat.GetSeatNo());
        row << new QStandardItem(QString::number(seatSet->GetPriceById(seat.GetSeatId())));
        tableModel->appendRow(row);
    }

    ui.tableView->setModel(tableModel);
    ui.tableView->reset();
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑

    connect(tableModel, &QStandardItemModel::itemChanged, this, &AdminMainWindow::onTableItemChanged);
}