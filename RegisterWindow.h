#pragma once

#include <QDialog>
#include "UserSet.h"

class QLineEdit;
class QPushButton;

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(UserSet* userSet, QWidget* parent = nullptr);
    ~RegisterWindow();

private slots:
    void onRegisterButtonClicked();

private:

    UserSet* userSet;
    QLineEdit* userNameEdit;
    QLineEdit* displayNameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* confirmPasswordEdit;
    QLineEdit* phoneEdit;
    QLineEdit* idCardEdit;
    QPushButton* registerButton;
};