#pragma once

#include "User.h"
#include <vector>

class UserSet {
public:
    UserSet(void);
    virtual ~UserSet(void);

    void AddUser(const User& user);
    void RemoveUser(int userId);
    void UpdateUser(const User& user);

    User GetUser(int userId) const;

    void ReadFromFile(const QString& fileName);
    void WriteToFile(const QString& fileName) const;

	std::vector<User> GetAllUsers() const;

    QString GetUserNameById(int userId) const;
	QString GetPhoneById(int userId) const;
	QString GetIdCardById(int userId) const;
    User GetUserById(int userId) const;

	void SetUsers(const std::vector<User>& newUsers);

private:
    std::vector<User> users; 

};