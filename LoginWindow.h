#pragma once

#include <QDialog>
#include "ui_LoginWindow.h"
#include <UserSet.h>

class LoginWindow : public QDialog
{
	Q_OBJECT

private:
	int userId;
	QString userName;
	QString displayName;
	QString role;

	UserSet* userSet;
public:
	int getUserId() const;
	QString getUserName() const;
	QString getDisplayName() const;
	QString getUserRole() const;
	explicit LoginWindow(UserSet* userSet, QWidget* parent = nullptr);
	~LoginWindow();

	void onLogin();
	void onRegister();

	void loginSuccessful(int userId);
	void loginFailed(const QString& errorMessage);

private:
	Ui::LoginWindowClass ui;

};


