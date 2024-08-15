#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "user.h"

class EditUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserDialog(QWidget *parent = nullptr);
    EditUserDialog(const User& user, QWidget *parent);

    // 方法用于获取用户输入的新用户名、密码和角色信息
    QString getUsername() const;
    QString getPassword() const;
    QString getRole() const;

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QComboBox *roleComboBox;

    void setupUI();
};

#endif // EDITUSERDIALOG_H
