#include <QApplication>
#include "LoginWindow.h"
#include "AdminMainWindow.h"
#include "UserMainWindow.h"
#include "User.h"
#include "TrainSet.h"
#include "SeatSet.h"
#include "StationSet.h"
#include "ScheduleSet.h"
#include "UserSet.h"
#include "OrderSet.h"
#include "ResourcePath.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    TrainSet trainSet;
    SeatSet seatSet;
    StationSet stationSet;
    ScheduleSet scheduleSet;
    UserSet userSet;
    OrderSet orderSet;


    try {
        trainSet.ReadFromFile(TrainFileName);
        stationSet.ReadFromFile(StationFileName);
        scheduleSet.ReadFromFile(ScheduleFileName);
        userSet.ReadFromFile(UserFileName);
		orderSet.ReadFromFile(OrderFileName);
        seatSet.ReadFromFile(SeatFileName, orderSet);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "加载错误", QString("加载数据时发生错误：%1").arg(e.what()));
        return -1;
    }

    seatSet.RegisterOrderSet(orderSet);

    while (true) {
        LoginWindow loginDialog(&userSet);
        if (loginDialog.exec() == QDialog::Accepted) {
            if (loginDialog.getUserRole() == "admin") {
                AdminMainWindow adminWindow(&trainSet, &seatSet, &stationSet, &scheduleSet, &userSet, &orderSet);
                adminWindow.show();

                if (app.exec() != 0) {
                    break;
                }
            }
            else if (loginDialog.getUserRole() == "customer") {
                User user(loginDialog.getUserId(), loginDialog.getUserName(), loginDialog.getDisplayName(), "", "", "customer", "");
                UserMainWindow userWindow(user, &trainSet, &seatSet, &stationSet, &scheduleSet, &userSet, &orderSet);
                userWindow.show();

                if (app.exec() != 0) {
                    break;
                }
            }
        }
        else {
            break;
        }
    }

    try {
        trainSet.WriteToFile(TrainFileName);
		seatSet.WriteToFile(SeatFileName);
        stationSet.WriteToFile(StationFileName);
        scheduleSet.WriteToFile(ScheduleFileName);
		userSet.WriteToFile(UserFileName);
		orderSet.WriteToFile(OrderFileName);
       
    }
    catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "保存错误", QString("保存数据时发生错误：%1").arg(e.what()));
        return -1;
    }


    return 0;
}