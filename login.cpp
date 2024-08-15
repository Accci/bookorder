#include "login.h"
#include "ui_login.h"
#include "databasemanager.h"
#include <QMessageBox>
#include "adminwidget.h"
#include "userwidget.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle("教材订阅系统");
    ui->titleLabel->setDisabled(true);
    connect(ui->loginButton, &QPushButton::clicked, this, &login::userLog);
    connect(ui->exitButton, &QPushButton::clicked, this, &login::exitApp);
}

void login::userLog()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // 在数据库中验证用户名和密码
    if (DatabaseManager::verifyUser(username, password)) {
           QString role = DatabaseManager::getUserRole(username);
           if (role == "admin") {
               // 跳转到管理员界面
               AdminWidget *adminWidget = new AdminWidget;
               adminWidget->show();
               this->hide();
           } else {
               // 跳转到普通用户界面
               UserWidget *userWidget = new UserWidget;
               User currentUser = DatabaseManager::searchUserByUsername(username);
               userWidget->setCurrentUser(currentUser);
               userWidget->show();
               this->hide();
           }
       } else {
           QMessageBox::warning(this, "登录失败", "用户名或密码错误");
       }
}

void login::exitApp()
{
    QApplication::exit(); // 退出应用程序
}

login::~login()
{
    delete ui;
}


