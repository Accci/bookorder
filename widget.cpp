#include "widget.h"
//#include "ui_widget.h"
#include "databasemanager.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{

    m_dadbManager = new DatabaseManager;

    if(m_dadbManager->open()){
        QMessageBox::information(this,"成功", "成功打开数据库");
    }else{
        QMessageBox::critical(this,"失败", "无法打开数据库");
        return;
    }
    // 调用创建表格函数
//    if(!m_dadbManager->createTable("User", this)) return;
//    if(!m_dadbManager->createTable("Textbook", this)) return;
//    if(!m_dadbManager->createTable("Course", this)) return;
//    if(!m_dadbManager->createTable("Subscription", this))return;

}

Widget::~Widget()
{
    m_dadbManager->close();
}
