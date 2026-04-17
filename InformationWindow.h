#pragma once

#include <QDialog>
#include "UserSet.h"

class QLineEdit;
class QPushButton;

class InformationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InformationWindow(int userId, UserSet* userSet, QWidget* parent = nullptr);
    ~InformationWindow();

private slots:
    void onSaveButtonClicked();

private:
    int userId;
    QLineEdit* userNameEdit;
    QLineEdit* displayNameEdit;
    QLineEdit* phoneEdit;
    QLineEdit* idCardEdit;
    QLineEdit* currentPasswordEdit;
    QLineEdit* newPasswordEdit;
	QLineEdit* confirmNewPasswordEdit;
    QPushButton* saveButton;

	UserSet* userSet;
};