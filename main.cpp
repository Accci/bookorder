#include "widget.h"
#include "login.h"
#include <QApplication>
#include <QDebug>
#include "adminwidget.h"
#include "userwidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w1;
    login w;
    w.show();

    return a.exec();
}
