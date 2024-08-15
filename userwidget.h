#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include "user.h"
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>



class UserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserWidget(QWidget *parent = nullptr);
    ~UserWidget();
    void setupUserPage();
    void setupTextbookOrderPage();
    void setCurrentUser(const User& user){currentUser = user;}
    bool subscribeTextbook();
private:
    void setupUi();
    void clearRightLayout();
    void clearLayout(QLayout *layout);
    QSplitter* splitter;
    QVBoxLayout* rightLayout;
    User currentUser;
    QLineEdit* quantityLineEdit;
    QComboBox* courseComboBox;
    QComboBox* textbookComboBox;
    QTableWidget* subscriptionsTable;
};

#endif // USERWIDGET_H
