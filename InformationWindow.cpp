#include "InformationWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>
#include <QToolButton>
#include <ResourcePath.h>

InformationWindow::InformationWindow(int userId, UserSet* userSet, QWidget* parent)
    : QDialog(parent), userId(userId), userSet(userSet)
{
    setWindowTitle("个人信息");
    resize(400, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();
    userNameEdit = new QLineEdit(this);
    displayNameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    idCardEdit = new QLineEdit(this);
    currentPasswordEdit = new QLineEdit(this);
    newPasswordEdit = new QLineEdit(this);
    confirmNewPasswordEdit = new QLineEdit(this);

    currentPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmNewPasswordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("用户名:", userNameEdit);
    formLayout->addRow("显示名称:", displayNameEdit);
    formLayout->addRow("电话:", phoneEdit);
    formLayout->addRow("身份证:", idCardEdit);

    // 添加小眼睛按钮到当前密码输入框
    QToolButton* currentPasswordEyeButton = new QToolButton(this);
    currentPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
    currentPasswordEyeButton->setCursor(Qt::PointingHandCursor);
    currentPasswordEyeButton->setFixedWidth(40);
    currentPasswordEyeButton->setFixedHeight(25);
    currentPasswordEyeButton->setIconSize(QSize(25, 40));
    currentPasswordEyeButton->setStyleSheet("QToolButton { padding: 0px; }");

    QHBoxLayout* currentPasswordLayout = new QHBoxLayout();
    currentPasswordLayout->addWidget(currentPasswordEdit);
    currentPasswordLayout->addWidget(currentPasswordEyeButton);
    formLayout->addRow("当前密码:", currentPasswordLayout);

    // 添加小眼睛按钮到新密码输入框
    QToolButton* newPasswordEyeButton = new QToolButton(this);
    newPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
    newPasswordEyeButton->setCursor(Qt::PointingHandCursor);
    newPasswordEyeButton->setFixedWidth(40);
    newPasswordEyeButton->setFixedHeight(25);
    newPasswordEyeButton->setIconSize(QSize(25, 40));
    newPasswordEyeButton->setStyleSheet("QToolButton { padding: 0px; }");

    QHBoxLayout* newPasswordLayout = new QHBoxLayout();
    newPasswordLayout->addWidget(newPasswordEdit);
    newPasswordLayout->addWidget(newPasswordEyeButton);
    formLayout->addRow("新密码:", newPasswordLayout);

    // 添加小眼睛按钮到确认新密码输入框
    QToolButton* confirmNewPasswordEyeButton = new QToolButton(this);
    confirmNewPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
    confirmNewPasswordEyeButton->setCursor(Qt::PointingHandCursor);
    confirmNewPasswordEyeButton->setFixedWidth(40);
    confirmNewPasswordEyeButton->setFixedHeight(25);
    confirmNewPasswordEyeButton->setIconSize(QSize(25, 40));
    confirmNewPasswordEyeButton->setStyleSheet("QToolButton { padding: 0px; }");

    QHBoxLayout* confirmNewPasswordLayout = new QHBoxLayout();
    confirmNewPasswordLayout->addWidget(confirmNewPasswordEdit);
    confirmNewPasswordLayout->addWidget(confirmNewPasswordEyeButton);
    formLayout->addRow("确认新密码:", confirmNewPasswordLayout);

    saveButton = new QPushButton("保存", this);
    connect(saveButton, &QPushButton::clicked, this, &InformationWindow::onSaveButtonClicked);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    try {
        User user = userSet->GetUserById(userId);
        userNameEdit->setText(user.GetUserName());
        displayNameEdit->setText(user.GetDisplayName());
        phoneEdit->setText(user.GetPhone());
        idCardEdit->setText(user.GetIdCard());
    }
    catch (const std::invalid_argument& e) {
        QMessageBox::warning(this, "加载失败", "无法加载用户信息：" + QString::fromStdString(e.what()));
    }

    connect(currentPasswordEyeButton, &QToolButton::clicked, this, [this, currentPasswordEyeButton]() {
        if (currentPasswordEdit->echoMode() == QLineEdit::Password) {
            currentPasswordEdit->setEchoMode(QLineEdit::Normal); 
            currentPasswordEyeButton->setIcon(QIcon(ICON_EYE_OPEN));
        }
        else {
            currentPasswordEdit->setEchoMode(QLineEdit::Password);
            currentPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
        }
        });

    connect(newPasswordEyeButton, &QToolButton::clicked, this, [this, newPasswordEyeButton]() {
        if (newPasswordEdit->echoMode() == QLineEdit::Password) {
            newPasswordEdit->setEchoMode(QLineEdit::Normal); 
            newPasswordEyeButton->setIcon(QIcon(ICON_EYE_OPEN));
        }
        else {
            newPasswordEdit->setEchoMode(QLineEdit::Password); 
            newPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
        }
        });

    connect(confirmNewPasswordEyeButton, &QToolButton::clicked, this, [this, confirmNewPasswordEyeButton]() {
        if (confirmNewPasswordEdit->echoMode() == QLineEdit::Password) {
            confirmNewPasswordEdit->setEchoMode(QLineEdit::Normal); 
            confirmNewPasswordEyeButton->setIcon(QIcon(ICON_EYE_OPEN));
        }
        else {
            confirmNewPasswordEdit->setEchoMode(QLineEdit::Password); 
            confirmNewPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
        }
        });
}

InformationWindow::~InformationWindow()
{
}

void InformationWindow::onSaveButtonClicked()
{
    QString userName = userNameEdit->text().trimmed();
    QString displayName = displayNameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString idCard = idCardEdit->text().trimmed();
    QString currentPassword = currentPasswordEdit->text();
    QString newPassword = newPasswordEdit->text();
    QString confirmNewPassword = confirmNewPasswordEdit->text();

    if (userName.isEmpty() || displayName.isEmpty() || phone.isEmpty() || idCard.isEmpty()) {
        QMessageBox::warning(this, "保存失败", "所有字段均为必填项。");
        return;
    }

    QRegularExpression phoneRegex("^\\d{11}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "保存失败", "电话号码必须为11位数字。");
        return;
    }

    QRegularExpression idCardRegex("^\\d{6}(19|20)\\d{2}(0[1-9]|1[0-2])(0[1-9]|[12]\\d|3[01])\\d{3}[0-9Xx]$");
    if (!idCardRegex.match(idCard).hasMatch()) {
        QMessageBox::warning(this, "保存失败", "身份证号码必须为18位，且包含有效的出生日期（7-14位）。");
        return;
    }

    try {
        User user = userSet->GetUserById(userId);

        if (user.GetPassword() != currentPassword) {
            QMessageBox::warning(this, "保存失败", "当前密码不正确。");
            return;
        }

        user.SetUserName(userName);
        user.SetDisplayName(displayName);
        user.SetPhone(phone);
        user.SetIdCard(idCard);

        if (!newPassword.isEmpty()) {
            if (newPassword != confirmNewPassword) {
                QMessageBox::warning(this, "保存失败", "两次输入的新密码不一致。");
                return;
            }
            QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
            if (!passwordRegex.match(newPassword).hasMatch()) {
                QMessageBox::warning(this, "保存失败", "新密码必须包含字母和数字，且至少6位。");
                return;
            }
            user.SetPassword(newPassword);
        }

        userSet->UpdateUser(user);

        QMessageBox::information(this, "保存成功", "个人信息已成功保存！");
        accept();
    }
    catch (const std::invalid_argument& e) {
        QMessageBox::warning(this, "保存失败", "更新用户信息失败：" + QString::fromStdString(e.what()));
    }
}