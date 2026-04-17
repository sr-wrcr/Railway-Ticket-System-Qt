#include "User.h"
#include <QDate>
#include <sstream>


User::User()
    : user_id(0), user_name(""), display_name(""), password(""), phone(""), role(""), id_card("") {
}

User::User(int id, const QString& name, const QString& displayName, const QString& pass, const QString& phoneNumber, const QString& userRole, const QString& idCard)
	: user_id(id), user_name(name), display_name(displayName), password(pass), phone(phoneNumber), role(userRole), id_card(idCard) {
}

User::~User() {
	// Destructor implementation (if needed)
}

int User::GetUserId() const {
	return user_id;
}

QString User::GetUserName() const {
	return user_name;
}

QString User::GetDisplayName() const {
	return display_name;
}

QString User::GetPassword() const {
	return password;
}

QString User::GetPhone() const {
	return phone;
}

QString User::GetRole() const {
	return role;
}

QString User::GetIdCard() const {
	return id_card;
}

void User::SetUserId(int id) {
	user_id = id;
}

void User::SetUserName(const QString& name) {
	user_name = name;
}

void User::SetDisplayName(const QString& displayName) {
	display_name = displayName;
}

void User::SetPassword(const QString& pass) {
	password = pass;
}

void User::SetPhone(const QString& phoneNumber) {
	phone = phoneNumber;
}

void User::SetRole(const QString& userRole) {
	role = userRole;
}

void User::SetIdCard(const QString& idCard) {
	id_card = idCard;
}

std::istream& operator>>(std::istream& in, User& user) {
    std::string line;
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, name, displayName, pass, phoneNumber, userRole, idCard;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, displayName, ',');
        std::getline(ss, pass, ',');
        std::getline(ss, phoneNumber, ',');
        std::getline(ss, userRole, ',');
        std::getline(ss, idCard, ',');

        if (idStr.empty() || name.empty() || displayName.empty() || pass.empty() || phoneNumber.empty() || userRole.empty() || idCard.empty()) {
            throw std::invalid_argument("用户数据包含空字段。");
        }

        user.user_id = std::stoi(idStr);
        user.user_name = QString::fromStdString(name);
        user.display_name = QString::fromStdString(displayName);
        user.password = QString::fromStdString(pass);
        user.phone = QString::fromStdString(phoneNumber);
        user.role = QString::fromStdString(userRole);
        user.id_card = QString::fromStdString(idCard);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const User& user) {
    out << user.user_id << ","
        << user.user_name.toStdString() << ","
        << user.display_name.toStdString() << ","
        << user.password.toStdString() << ","
        << user.phone.toStdString() << ","
        << user.role.toStdString() << ","
        << user.id_card.toStdString();
    return out;
}

void User::isValid() const {
    if (user_id <= 0) {
        throw std::invalid_argument("用户ID必须有效。");
    }

    if (user_name.isEmpty()) {
        throw std::invalid_argument("姓名不能为空。");
    }
    if (display_name.isEmpty()) {
        throw std::invalid_argument("用户名不能为空。");
    }

    if (phone.length() != 11) {
        throw std::invalid_argument("手机号码必须11位数字。");
    }

    if (role != "admin" && role != "customer") {
        throw std::invalid_argument("身份必须是'admin'或'customer'。");
    }

    if (id_card.length() != 18) {
        throw std::invalid_argument("有效身份证必须18位");
    }

    QString dateStr = id_card.mid(6, 8); 
    if (dateStr.length() != 8 || !dateStr.toULongLong()) {
        throw std::invalid_argument("身份证出生年月日字段需有效。");
    }

    int year = dateStr.mid(0, 4).toInt();
    int month = dateStr.mid(4, 2).toInt();
    int day = dateStr.mid(6, 2).toInt();

    if (year < 1900 || year > 2025 || month < 1 || month > 12 || day < 1 || day > 31) {
        throw std::invalid_argument("身份证出生年月日字段必须在19000101到20251231。");
    }

    QDate date(year, month, day);
    if (!date.isValid()) {
        throw std::invalid_argument("身份证出生年月日非法。");
    }
}