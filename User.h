#pragma once
#include <QString>
#include <iostream>

using namespace std;

class User
{
private:
	int user_id;
	QString user_name;
	QString display_name;
	QString password;
	QString phone;
	QString role;
	QString id_card;

public:
	User();
	User(int id, const QString& name, const QString& displayName, const QString& pass, 
		const QString& phoneNumber, const QString& userRole, const QString& idCard);
	virtual ~User();

	int GetUserId() const;
	QString GetUserName() const;
	QString GetDisplayName() const;
	QString GetPassword() const;
	QString GetPhone() const;
	QString GetRole() const;
	QString GetIdCard() const;

	void SetUserId(int id);
	void SetUserName(const QString& name);
	void SetDisplayName(const QString& displayName);
	void SetPassword(const QString& pass);
	void SetPhone(const QString& phoneNumber);
	void SetRole(const QString& userRole);
	void SetIdCard(const QString& idCard);

	friend std::istream& operator>>(std::istream& in, User& user);
	friend std::ostream& operator<<(std::ostream& out, const User& user);

	void isValid() const;
};

