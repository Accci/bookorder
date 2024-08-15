#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include "databasemanager.h"


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    DatabaseManager *m_dadbManager;
};

#endif // WIDGET_H
