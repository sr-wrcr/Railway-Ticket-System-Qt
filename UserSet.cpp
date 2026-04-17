#include "UserSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

UserSet::UserSet(void) {}

UserSet::~UserSet(void) {}

void UserSet::AddUser(const User& user) {
    for (const auto& u : users) {
        if (u.GetUserId() == user.GetUserId()) {
            throw std::invalid_argument("相同ID用户已存在。");
        }
    }
    users.push_back(user);
}

void UserSet::RemoveUser(int userId) {
    auto it = std::remove_if(users.begin(), users.end(), [userId](const User& user) {
        return user.GetUserId() == userId;
        });
    if (it != users.end()) {
        users.erase(it, users.end());
    }
    else {
        throw std::invalid_argument("指定ID的用户不存在。");
    }
}

void UserSet::UpdateUser(const User& user) {
    for (auto& u : users) {
        if (u.GetUserId() == user.GetUserId()) {
            u = user;
            return;
        }
    }
    throw std::invalid_argument("指定ID的用户不存在。");
}

User UserSet::GetUser(int userId) const {
    for (const auto& u : users) {
        if (u.GetUserId() == userId) {
            return u;
        }
    }
    throw std::invalid_argument("指定ID的用户不存在。");
}

void UserSet::ReadFromFile(const QString& fileName) {
    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("文件打开失败: " + fileName.toStdString());
    }

    users.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        User user;
        ss >> user;
        users.push_back(user);
    }
    file.close();
}

void UserSet::WriteToFile(const QString& fileName) const {
    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        throw std::runtime_error("文件打开失败: " + fileName.toStdString());
    }

    for (const auto& user : users) {
        file << user << "\n";
    }
    file.close();
}

std::vector<User> UserSet::GetAllUsers() const {
    return users;
}

QString UserSet::GetUserNameById(int userId) const {
    for (const auto& user : users) {
        if (user.GetUserId() == userId) {
            return user.GetUserName();
        }
    }
    return "Unknown User";
}

QString UserSet::GetPhoneById(int userId) const {
    for (const auto& user : users) {
        if (user.GetUserId() == userId) {
            return user.GetPhone();
        }
    }
    return "Unknown Phone";
}

QString UserSet::GetIdCardById(int userId) const {
    for (const auto& user : users) {
        if (user.GetUserId() == userId) {
            return user.GetIdCard();
        }
    }
    return "Unknown ID Card";
}

User UserSet::GetUserById(int userId) const {
    for (const auto& user : users) {
        if (user.GetUserId() == userId) {
            return user;
        }
    }
    throw std::invalid_argument("指定ID的用户不存在。");
}

void UserSet::SetUsers(const std::vector<User>& newUsers) {
    users = newUsers;
}

