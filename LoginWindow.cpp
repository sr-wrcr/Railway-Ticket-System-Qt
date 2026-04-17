#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QFont>
#include "ui_LoginWindow.h"
#include <qformlayout.h>
#include "UserSet.h"
#include <QMessageBox>
#include "RegisterWindow.h"
#include <QToolButton>
#include <ResourcePath.h>

LoginWindow::LoginWindow(UserSet* userSet, QWidget* parent)
    : QDialog(parent), userSet(userSet)
{
    setWindowTitle("高铁订票系统 - 登录");
    resize(400, 300);  

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QLabel* titleLabel = new QLabel("高铁订票系统", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setContentsMargins(20, 20, 20, 20);  // 设置边距

    QLineEdit* userEdit = new QLineEdit(centralWidget);
    userEdit->setObjectName("userEdit"); // 设置 objectName
    formLayout->addRow("用户名:", userEdit);

    QLineEdit* pwdEdit = new QLineEdit(centralWidget);
    pwdEdit->setObjectName("pwdEdit"); 
    pwdEdit->setEchoMode(QLineEdit::Password);

    // 添加小眼睛按钮
    QToolButton* eyeButton = new QToolButton(centralWidget);
    eyeButton->setObjectName("eyeButton");
    eyeButton->setIcon(QIcon(ICON_EYE_CLOSE)); // 默认闭合图标
    eyeButton->setCursor(Qt::PointingHandCursor);
	eyeButton->setFixedWidth(40);
    eyeButton->setFixedHeight(25); // 设置按钮高度
    eyeButton->setIconSize(QSize(25, 40)); // 设置图标大小为 30x30 像素
    eyeButton->setStyleSheet("QToolButton { padding: 0px; }");

    // 将小眼睛按钮放在密码输入框的右侧
    QHBoxLayout* pwdLayout = new QHBoxLayout();
    pwdLayout->addWidget(pwdEdit);
    pwdLayout->addWidget(eyeButton);
    formLayout->addRow("密码:", pwdLayout);

    QComboBox* roleCombo = new QComboBox(centralWidget);
    roleCombo->setObjectName("roleCombo"); 
    roleCombo->addItem("用户");
    roleCombo->addItem("管理员");
    formLayout->addRow("身份:", roleCombo);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* loginBtn = new QPushButton("登录", centralWidget);
    QPushButton* registerBtn = new QPushButton("注册", centralWidget);
    btnLayout->addWidget(loginBtn);
    btnLayout->addWidget(registerBtn);
    btnLayout->setSpacing(20);  

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);


    QWidget* btnContainer = new QWidget(this);
    btnContainer->setLayout(btnLayout);
    mainLayout->addWidget(btnContainer, 0, Qt::AlignCenter);   

    mainLayout->addStretch();

    setLayout(mainLayout);


    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::onRegister);

    connect(eyeButton, &QToolButton::clicked, this, [pwdEdit, eyeButton]() {
        if (pwdEdit->echoMode() == QLineEdit::Password) {
            pwdEdit->setEchoMode(QLineEdit::Normal); // 显示密码
            eyeButton->setIcon(QIcon(ICON_EYE_OPEN)); // 切换为睁开图标
        }
        else {
            pwdEdit->setEchoMode(QLineEdit::Password); // 隐藏密码
            eyeButton->setIcon(QIcon(ICON_EYE_CLOSE)); // 切换为闭合图标
        }
        });
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::onLogin()
{
    QLineEdit* userEdit = findChild<QLineEdit*>("userEdit");
    QLineEdit* pwdEdit = findChild<QLineEdit*>("pwdEdit");
    QComboBox* roleCombo = findChild<QComboBox*>("roleCombo");

    if (!userEdit || !pwdEdit || !roleCombo) {
        loginFailed("内部错误：无法找到输入组件。");
        return;
    }

    QString username = userEdit->text().trimmed();
    QString password = pwdEdit->text();
    QString role = roleCombo->currentText();

    if (username.isEmpty() || password.isEmpty()) {
        loginFailed("用户名或密码不能为空。");
        return;
    }
    
    // 使用索引查找用户
    const auto& users = userSet->GetAllUsers();
    int matchedIndex = -1;

    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].GetDisplayName() == username) {
            matchedIndex = static_cast<int>(i);
            break;
        }
    }

    if (matchedIndex == -1) {
        loginFailed("用户名不存在。");
        return;
    }

    const User& matchedUser = users[matchedIndex];

    // 检查密码是否正确
    if (matchedUser.GetPassword() != password) {
        loginFailed("密码错误。");
        return;
    }

    // 检查角色是否匹配
    if ((role == "用户" && matchedUser.GetRole() == "customer") ||
        (role == "管理员" && matchedUser.GetRole() == "admin")) {
        loginSuccessful(matchedUser.GetUserId());
        accept();
    }
    else {
        loginFailed("身份不匹配。");
    }

}

void LoginWindow::onRegister()
{
    RegisterWindow registerWindow(userSet, this);
    if (registerWindow.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "注册成功", "请使用新注册的账号登录。");
    }
}

void LoginWindow::loginSuccessful(int userId)
{

    try {
        User user = userSet->GetUserById(userId);
        QMessageBox::information(this, "登录成功", "欢迎，" + user.GetUserName() + "！");
        this->userId = user.GetUserId();
        this->userName = user.GetUserName();
        this->displayName = user.GetDisplayName();
        this->role = user.GetRole();
    }
    catch (const std::invalid_argument& e) {
        QMessageBox::warning(this, "登录失败", "用户信息查询失败：" + QString::fromStdString(e.what()));
    }
}

void LoginWindow::loginFailed(const QString& errorMessage)
{
    QMessageBox::warning(this, "登录失败", errorMessage);
}

int LoginWindow::getUserId() const
{
    return userId;
}
QString LoginWindow::getUserName() const
{
    return userName;
}
QString LoginWindow::getDisplayName() const
{
    return displayName;
}
QString LoginWindow::getUserRole() const
{
    return role;
}