#include "RegisterWindow.h"
#include "UserSet.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>
#include <QToolButton> 
#include <ResourcePath.h>

RegisterWindow::RegisterWindow(UserSet* userSet, QWidget* parent)
    : QDialog(parent), userSet(userSet)
{
    setWindowTitle("用户注册");
    resize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();
    userNameEdit = new QLineEdit(this);
    displayNameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    confirmPasswordEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    idCardEdit = new QLineEdit(this);

    passwordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("姓名:", userNameEdit);
    formLayout->addRow("用户名:", displayNameEdit);
    formLayout->addRow("电话:", phoneEdit);
    formLayout->addRow("身份证:", idCardEdit);

    // 添加小眼睛按钮到密码输入框
    QToolButton* passwordEyeButton = new QToolButton(this);
    passwordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
    passwordEyeButton->setCursor(Qt::PointingHandCursor);
    passwordEyeButton->setFixedWidth(40);
    passwordEyeButton->setFixedHeight(25);
    passwordEyeButton->setIconSize(QSize(25, 40));
    passwordEyeButton->setStyleSheet("QToolButton { padding: 0px; }");

    QHBoxLayout* passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(passwordEyeButton);
    formLayout->addRow("密码:", passwordLayout);

    // 添加小眼睛按钮到确认密码输入框
    QToolButton* confirmPasswordEyeButton = new QToolButton(this);
    confirmPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
    confirmPasswordEyeButton->setCursor(Qt::PointingHandCursor);
    confirmPasswordEyeButton->setFixedWidth(40);
    confirmPasswordEyeButton->setFixedHeight(25);
    confirmPasswordEyeButton->setIconSize(QSize(25, 40));
    confirmPasswordEyeButton->setStyleSheet("QToolButton { padding: 0px; }");

    QHBoxLayout* confirmPasswordLayout = new QHBoxLayout();
    confirmPasswordLayout->addWidget(confirmPasswordEdit);
    confirmPasswordLayout->addWidget(confirmPasswordEyeButton);
    formLayout->addRow("确认密码:", confirmPasswordLayout);


    registerButton = new QPushButton("注册", this);
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterButtonClicked);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(registerButton);


    // 连接小眼睛按钮的点击事件
    connect(passwordEyeButton, &QToolButton::clicked, this, [this, passwordEyeButton]() {
        if (passwordEdit->echoMode() == QLineEdit::Password) {
            passwordEdit->setEchoMode(QLineEdit::Normal); // 显示密码
            passwordEyeButton->setIcon(QIcon(ICON_EYE_OPEN));
        }
        else {
            passwordEdit->setEchoMode(QLineEdit::Password); // 隐藏密码
            passwordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
        }
        });

    connect(confirmPasswordEyeButton, &QToolButton::clicked, this, [this, confirmPasswordEyeButton]() {
        if (confirmPasswordEdit->echoMode() == QLineEdit::Password) {
            confirmPasswordEdit->setEchoMode(QLineEdit::Normal); // 显示密码
            confirmPasswordEyeButton->setIcon(QIcon(ICON_EYE_OPEN));
        }
        else {
            confirmPasswordEdit->setEchoMode(QLineEdit::Password); // 隐藏密码
            confirmPasswordEyeButton->setIcon(QIcon(ICON_EYE_CLOSE));
        }
        });
}

RegisterWindow::~RegisterWindow()
{
}

void RegisterWindow::onRegisterButtonClicked()
{
    QString userName = userNameEdit->text().trimmed();
    QString displayName = displayNameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();
    QString phone = phoneEdit->text().trimmed();
    QString idCard = idCardEdit->text().trimmed();

    if (userName.isEmpty() || displayName.isEmpty() || password.isEmpty() || confirmPassword.isEmpty() || phone.isEmpty() || idCard.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "所有字段均为必填项。");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致。");
        return;
    }

    QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{6,}$");
    if (!passwordRegex.match(password).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "密码必须包含字母和数字，且至少6位。");
        return;
    }

    QRegularExpression phoneRegex("^\\d{11}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "电话号码必须为11位数字。");
        return;
    }

    QRegularExpression idCardRegex("^\\d{6}(19|20)\\d{2}(0[1-9]|1[0-2])(0[1-9]|[12]\\d|3[01])\\d{3}[0-9Xx]$");
    if (!idCardRegex.match(idCard).hasMatch()) {
        QMessageBox::warning(this, "注册失败", "身份证号码不合法");
        return;
    }


    for (const auto& user : userSet->GetAllUsers()) {
        if (user.GetDisplayName() == displayName) {
            QMessageBox::warning(this, "注册失败", "用户名已存在，请选择其他用户名。");
            return;
        }
    }

    int newUserId = static_cast<int>(userSet->GetAllUsers().size()) + 1;
    User newUser(newUserId, userName, displayName, password, phone, "customer", idCard);
    
    userSet->AddUser(newUser);
    QMessageBox::information(this, "注册成功", "用户注册成功！");
    accept();
}