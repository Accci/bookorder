#include "adminwidget.h"
#include "databasemanager.h"
#include "edituserdialog.h"
#include "addtextbookdialog.h"
#include <QSplitter>
#include <QTreeView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QStringListModel>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QHeaderView >

AdminWidget::AdminWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
}

AdminWidget::~AdminWidget()
{
}

void AdminWidget::setupUserManagementPage()
{
    clearRightLayout();

    QVBoxLayout *userManagementLayout = new QVBoxLayout;

    // 创建表格用于显示用户信息
    userTable = new QTableWidget;
    userTable->setColumnCount(3); // 设置表格列数
    QStringList headers = {"ID", "用户名", "密码"}; // 设置表头
    userTable->setHorizontalHeaderLabels(headers);
    userTable->verticalHeader()->setVisible(false);


    // 从数据库中获取用户信息，并填充到 userTable 中
    populateUserTable();

    // 实现分页显示，确保每页最多显示 5 条用户信息
//    QTableWidget* pagedUserTable = paginateUserTable();

    // 创建搜索框和按钮用于搜索用户
    searchLineEdit = new QLineEdit;
    QPushButton *searchUserButton = new QPushButton("搜索用户");

    // 连接搜索按钮的点击信号与槽函数，以响应搜索操作
    connect(searchUserButton, &QPushButton::clicked, this, &AdminWidget::searchUser);

    // 将搜索框和搜索按钮添加到用户管理页面的布局中
    userManagementLayout->addWidget(searchLineEdit);
    userManagementLayout->addWidget(searchUserButton);
    // 创建按钮用于操作用户
    QPushButton *addUserButton = new QPushButton("添加用户");
    QPushButton *deleteUserButton = new QPushButton("删除用户");
    QPushButton *editUserButton = new QPushButton("编辑用户");


    // 将按钮添加到用户管理页面的布局中
    userManagementLayout->addWidget(addUserButton);
    userManagementLayout->addWidget(deleteUserButton);
    userManagementLayout->addWidget(editUserButton);


    // 将表格添加到用户管理页面的布局中
    userManagementLayout->addWidget(userTable);

    // 设置右侧管理功能区域的布局为用户管理页面的布局
    rightLayout->addLayout(userManagementLayout);

    // 连接按钮的点击信号与槽函数，以响应用户的操作
    connect(addUserButton, &QPushButton::clicked, this, &AdminWidget::addUser);
    connect(deleteUserButton, &QPushButton::clicked, this, &AdminWidget::deleteUser);
    connect(editUserButton, &QPushButton::clicked, this, &AdminWidget::editUser);

}

void AdminWidget::setupTextbookManagementPage()
{
    // 清空右侧管理功能区域的内容
    clearRightLayout();

    // 创建教材管理页面的布局
    QVBoxLayout* textbookManagementLayout = new QVBoxLayout;

    // 创建表格用于显示教材信息
    textbookTable = new QTableWidget;
    textbookTable->setColumnCount(4); // 设置表格列数
    QStringList headers = {"ID", "教材名", "作者", "单价"}; // 设置表头
    textbookTable->setHorizontalHeaderLabels(headers);
    textbookTable->verticalHeader()->setVisible(false);

    populateTextTable();

    // 创建按钮用于操作教材
    QPushButton *addTextbookButton = new QPushButton("添加教材");
    QPushButton *deleteTextbookButton = new QPushButton("删除教材");
    QPushButton *editTextbookButton = new QPushButton("编辑教材");
    searchTextbookLineEdit = new QLineEdit;
    QPushButton *searchTextbookButton = new QPushButton("搜索教材");

    // 将按钮添加到教材管理页面的布局中
    textbookManagementLayout->addWidget(addTextbookButton);
    textbookManagementLayout->addWidget(deleteTextbookButton);
    textbookManagementLayout->addWidget(editTextbookButton);
    textbookManagementLayout->addWidget(searchTextbookLineEdit);
    textbookManagementLayout->addWidget(searchTextbookButton);

    // 将表格添加到教材管理页面的布局中
    textbookManagementLayout->addWidget(textbookTable);

    // 设置右侧管理功能区域的布局为教材管理页面的布局
    rightLayout->addLayout(textbookManagementLayout);

    // 连接按钮的点击信号与槽函数，以响应用户的操作
    connect(addTextbookButton, &QPushButton::clicked, this, &AdminWidget::addTextbook);
    connect(deleteTextbookButton, &QPushButton::clicked, this, &AdminWidget::deleteTextbook);
    connect(editTextbookButton, &QPushButton::clicked, this, &AdminWidget::editTextbook);
    connect(searchTextbookButton, &QPushButton::clicked, this, &AdminWidget::searchTextbook);
}

void AdminWidget::setupQueryPage()
{
    // 清空右侧管理功能区域的内容
    clearRightLayout();

    // 创建订阅查询页面的布局
    QVBoxLayout* queryLayout = new QVBoxLayout;

    // 创建下拉框用于选择查询方式
    QComboBox* queryComboBox = new QComboBox;
    queryComboBox->addItem("按人员查询");
    queryComboBox->addItem("按教材查询");
    queryComboBox->addItem("按课程查询");

    // 创建表格用于显示查询结果
    queryResultTable = new QTableWidget;
    queryResultTable->setColumnCount(6); // 设置表格列数
    QStringList headers = {"用户名", "教材名", "课程名", "订阅价格", "订阅数量", "订阅时间"}; // 设置表头
    queryResultTable->setHorizontalHeaderLabels(headers);


    // 将控件添加到订阅查询页面的布局中
    queryline = new QLineEdit();
    queryLayout->addWidget(queryline);
    queryLayout->addWidget(queryComboBox);
    queryLayout->addWidget(queryResultTable);
//    queryLayout->addWidget(printButton);

    // 设置右侧管理功能区域的布局为订阅查询页面的布局
    rightLayout->addLayout(queryLayout);

    // 连接按钮的点击信号与槽函数，以响应用户的操作
    connect(queryComboBox, QOverload<int>::of(&QComboBox::activated), this, &AdminWidget::onQueryComboBoxActivated);
//    connect(printButton, &QPushButton::clicked, this, &AdminWidget::printQueryResult);
//    connect(queryComboBox, QOverload<int>::of(&QComboBox::activated), [=](int index) {
//        // 清空表格内容
//        queryResultTable->clearContents();

//        // 根据选择的查询方式更新表头
//        if (index == 0) { // 按人员查询
//            queryResultTable->setHorizontalHeaderLabels({"用户名", "教材", "订阅价格", "订阅数量", "订阅时间"});
//        } else if (index == 1) { // 按教材查询
//            queryResultTable->setHorizontalHeaderLabels({"教材", "订阅价格", "订阅数量", "订阅时间"});
//        } else if (index == 2) { // 按课程查询
//            queryResultTable->setHorizontalHeaderLabels({"课程", "教材", "订阅价格", "订阅数量", "订阅时间"});
//        }
//    });

}

void AdminWidget::setupStatisticsPage()
{
    // 清空右侧布局
    clearRightLayout();

    // 创建组合框用于选择统计类型
    QComboBox *statisticsComboBox = new QComboBox;
    statisticsComboBox->addItem("按教材统计");
    statisticsComboBox->addItem("按人员统计");
    statisticsComboBox->addItem("按课程统计");

    // 创建用于显示统计结果的表格
    statisticsTable = new QTableWidget;
    statisticsTable->setColumnCount(4); // 设置表格列数为4，用于显示统计结果
    QStringList headers = {" ", " ", " ", " "}; // 设置表头
    statisticsTable->setHorizontalHeaderLabels(headers);


    // 创建按钮用于进行统计
//    QPushButton *calculateButton = new QPushButton("统计");

    // 将组合框、表格和按钮放在一个垂直布局中
    QVBoxLayout *statisticsLayout = new QVBoxLayout;
    statisticsLayout->addWidget(statisticsComboBox);
    statisticsLayout->addWidget(statisticsTable);

    // 设置右侧布局为统计页面布局
    rightLayout->addLayout(statisticsLayout);

    // 连接统计类型选择框的槽函数，根据选择的类型更新表头
    connect(statisticsComboBox, QOverload<int>::of(&QComboBox::activated), [=](int index) {
        // 清空表格内容
        statisticsTable->clearContents();

        // 根据选择的统计类型设置表头
        if (index == 0) { // 按教材统计
            statisticsTable->setHorizontalHeaderLabels({"教材名", "订阅人数","订阅数量", "总金额"});
        } else if (index == 1) { // 按人员统计
            statisticsTable->setHorizontalHeaderLabels({"用户名", "教材名", "课程名", "订阅金额"});
        } else if (index == 2) { // 按课程统计
            statisticsTable->setHorizontalHeaderLabels({"课程名", "教材名", "订阅总数", "总金额"});
        }
    });

    // 连接统计按钮的点击信号与槽函数，以响应统计操作
    connect(statisticsComboBox, QOverload<int>::of(&QComboBox::activated), this, &AdminWidget::calculateStatistics);
}



void AdminWidget::populateUserTable() {
    // 获取用户信息，可以从数据库中获取，这里使用示例数据填充
    QList<User> userList = DatabaseManager::getUserListFromDatabase();

    // 设置表格行数
    userTable->setRowCount(userList.size());

    // 填充表格
    int row = 0;
    for (const User& user : userList) {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(user.id));
        QTableWidgetItem* usernameItem = new QTableWidgetItem(user.username);
        QTableWidgetItem* passwordItem = new QTableWidgetItem(user.password);

        userTable->setItem(row, 0, idItem);
        userTable->setItem(row, 1, usernameItem);
        userTable->setItem(row, 2, passwordItem);

        ++row;
    }
}

void AdminWidget::addUser()
{
    // 创建一个对话框或表单
    QDialog *addUserDialog = new QDialog(this);
    QVBoxLayout *dialogLayout = new QVBoxLayout(addUserDialog);
    addUserDialog->setWindowTitle("添加用户");
    // 创建输入字段和标签
    QLabel *usernameLabel = new QLabel("用户名:");
    QLineEdit *usernameLineEdit = new QLineEdit;
    QLabel *passwordLabel = new QLabel("密码:");
    QLineEdit *passwordLineEdit = new QLineEdit;
    QLabel *roleLabel = new QLabel("角色:");
    QComboBox *roleComboBox = new QComboBox;
    roleComboBox->addItem("普通用户");
    roleComboBox->addItem("管理员");

    // 添加输入字段和标签到布局中
    dialogLayout->addWidget(usernameLabel);
    dialogLayout->addWidget(usernameLineEdit);
    dialogLayout->addWidget(passwordLabel);
    dialogLayout->addWidget(passwordLineEdit);
    dialogLayout->addWidget(roleLabel);
    dialogLayout->addWidget(roleComboBox);

    // 创建确认按钮
    QPushButton *confirmButton = new QPushButton("确认");
    dialogLayout->addWidget(confirmButton);

    // 连接确认按钮的点击信号与槽函数
    connect(confirmButton, &QPushButton::clicked, [=]() {
        // 获取用户输入的信息
        QString username = usernameLineEdit->text();
        QString password = passwordLineEdit->text();
        QString role = roleComboBox->currentText();
        if(DatabaseManager::addUser(username, password, role)){
            QMessageBox::information(this, "添加成功", "注册成功！欢迎加入！");
            populateUserTable();
        }else
            QMessageBox::warning(this, "添加失败", "注册失败！请稍后再试！");

        // 关闭对话框
        addUserDialog->close();
    });

    // 设置对话框的窗口标题
    addUserDialog->setWindowTitle("添加用户");

    // 显示对话框
    addUserDialog->exec();
}

void AdminWidget::deleteUser()
{
    // 获取当前选中的行索引
    int selectedRow = userTable->currentRow();

    if (selectedRow != -1) {
        // 获取用户 ID 所在列的数据
        QTableWidgetItem *itemId = userTable->item(selectedRow, 0);
        if (itemId) {
            // 获取用户 ID
            QString userId = itemId->text();

            // 在这里执行删除用户的操作
            bool success = DatabaseManager::delUser(userId.toInt());

            if (success) {
                populateUserTable();
                // 更新用户界面以反映已删除用户的更改
                QMessageBox::critical(this, "成功", "删除用户成功！");
            } else {
                // 如果删除失败，显示错误消息给用户
                QMessageBox::critical(this, "错误", "删除用户失败！");
            }
        }
    } else {
        // 如果没有选中任何行，显示提示给用户
        QMessageBox::information(this, "提示", "请先选中要删除的用户！");
    }
}

void AdminWidget::editUser()
{
    // 获取选中的用户行
    int selectedRow = userTable->currentRow();
    if (selectedRow < 0) {
        // 如果没有选中用户，显示错误消息或者返回
        QMessageBox::warning(this, "编辑用户", "请先选中要编辑的用户！");
        return;
    }

    // 获取选中行的用户ID（假设ID位于第一列）
    int userId = userTable->item(selectedRow, 0)->text().toInt();

    // 从数据库中获取选中用户的信息
    User user = DatabaseManager::getUserFromDatabase(userId);
    if (user.isValid()) {
        // 创建一个对话框用于编辑用户信息
        EditUserDialog editDialog(user, this);
        editDialog.setWindowTitle("编辑用户信息");
        // 显示对话框，并等待用户操作完成
        if (editDialog.exec() == QDialog::Accepted) {
            // 用户点击了对话框中的确认按钮

            // 获取用户在对话框中输入的新的用户信息
            QString newUsername = editDialog.getUsername();
            QString newPassword = editDialog.getPassword();
            QString newRole = editDialog.getRole();
            qDebug() << newUsername << " " << newPassword << " " << newRole;

            // 更新数据库中的用户信息
            user.setUsername(newUsername);
            user.setPassword(newPassword);
            user.setRole(newRole);
            bool success = DatabaseManager::updateUserInDatabase(user);

            if (success) {
                populateUserTable();
                QMessageBox::information(this, "编辑用户", "用户信息已更新！");
            } else {
                // 更新失败，显示错误消息
                QMessageBox::critical(this, "编辑用户", "更新用户信息失败！");
            }
        }
    } else {
        // 无法从数据库中获取用户信息，显示错误消息
        QMessageBox::critical(this, "编辑用户", "无法获取选中用户的信息！");
    }
}

void AdminWidget::searchUser()
{
    // 获取搜索关键字，这里假设使用 QLineEdit 输入框进行输入
    QString keyword = searchLineEdit->text().trimmed(); // 获取输入的搜索关键字并去除首尾空格

    // 如果搜索关键字为空，则不执行搜索操作
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "搜索用户", "请输入搜索用户名！");
        return;
    }

    // 执行搜索操作，从数据库中检索用户信息
    User searchResult = DatabaseManager::searchUserByUsername(keyword);

    // 如果搜索结果为空，则提示用户未找到匹配项
    if (!searchResult.isValid()) {
        QMessageBox::information(this, "搜索用户", "未找到匹配用户！");
        return;
    }

    // 显示搜索结果，可以将搜索结果显示在另一个表格中，或者在当前表格中覆盖显示
    // 这里仅做演示，假设将搜索结果显示在当前表格中
    // 清空表格中原有的内容
    userTable->clearContents();

    // 设置表格行数为1，因为只有一个搜索结果
    userTable->setRowCount(1);

    // 填充搜索结果到表格中
    QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(searchResult.id));
    QTableWidgetItem* usernameItem = new QTableWidgetItem(searchResult.username);
    QTableWidgetItem* passwordItem = new QTableWidgetItem(searchResult.password);

    userTable->setItem(0, 0, idItem);
    userTable->setItem(0, 1, usernameItem);
    userTable->setItem(0, 2, passwordItem);

}

void AdminWidget::populateTextTable()
{
    // 获取教材信息，可以从数据库中获取，这里使用示例数据填充
    QList<Textbook> textbookList = DatabaseManager::getTextbookListFromDatabase();

    // 设置表格行数
    textbookTable->setRowCount(textbookList.size());

    // 填充表格
    int row = 0;
    for (const Textbook& textbook : textbookList) {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(textbook.id()));
        QTableWidgetItem* nameItem = new QTableWidgetItem(textbook.name());
        QTableWidgetItem* authorItem = new QTableWidgetItem(textbook.author());
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(textbook.price()));

        textbookTable->setItem(row, 0, idItem);
        textbookTable->setItem(row, 1, nameItem);
        textbookTable->setItem(row, 2, authorItem);
        textbookTable->setItem(row, 3, priceItem);

        ++row;
    }
}




void AdminWidget::calculateStatistics(int index)
{
    // 根据选择的统计类型进行相应的统计操作
    // 根据选择的统计类型执行相应的逻辑
    if (index == 0) { // 按教材统计
        // 查询数据库，获取按教材统计的结果
        QList<Textbook> textbooks = DatabaseManager::getAllTextbooks();

        // 清空表格内容
        statisticsTable->clearContents();
        statisticsTable->setRowCount(0); // 确保表格行数为0

        // 填充统计结果到表格中
        for (const Textbook& textbook : textbooks) {
            // 获取与当前教材相关的订阅记录
            QList<Subscription> subscriptions = DatabaseManager::getBookSubscriptions(textbook.id());

            // 统计订阅人数
            QSet<int> uniqueUserIds; // 用于存储不同用户的id
            for (const Subscription& subscription : subscriptions) {
                uniqueUserIds.insert(subscription.userId());
            }
            int subscribersCount = uniqueUserIds.size();

//            // 统计订阅数量（即订阅人数）
//            int subscriptionsCount = subscriptions.size();

            // 计算总金额
            double totalAmount = 0.0;
            for (const Subscription& subscription : subscriptions) {
                totalAmount += subscription.price();
            }

            // 计算订阅数量（四舍五入取整）
            int totalSubscriptionCount = qRound(totalAmount / textbook.price());

            // 添加一行到表格中
            int row = statisticsTable->rowCount();
            statisticsTable->insertRow(row);
            statisticsTable->setItem(row, 0, new QTableWidgetItem(textbook.name())); // 教材名
            statisticsTable->setItem(row, 1, new QTableWidgetItem(QString::number(subscribersCount))); // 订阅人数
            statisticsTable->setItem(row, 2, new QTableWidgetItem(QString::number(totalSubscriptionCount))); // 订阅数量
            statisticsTable->setItem(row, 3, new QTableWidgetItem(QString::number(totalAmount))); // 总金额
        }
    } else if (index == 1) { // 按人员统计
        // 执行按人员统计的逻辑
        // 查询数据库，计算每个用户订阅的教材、课程以及订阅金额等信息
        // 将统计结果填充到表格中
        // 查询数据库，获取每个用户订阅的信息
        QList<User> users = DatabaseManager::getAllUsers();
        statisticsTable->clearContents();
        statisticsTable->setRowCount(0); // 确保表格行数为0
        for (const User& user : users) {
                // 获取用户订阅信息
                QList<Subscription> subscriptions = DatabaseManager::getUserSubscriptions(user.id);
                int row = statisticsTable->rowCount();
                // 添加用户订阅信息到表格中
                for (const Subscription& subscription : subscriptions) {

                    statisticsTable->insertRow(row);
                    User user = DatabaseManager::getUserFromDatabase(subscription.userId());
                    statisticsTable->setItem(row, 0, new QTableWidgetItem(user.username)); // 用户名
                    Textbook book = DatabaseManager::searchTextbookByid(subscription.bookId());
                    statisticsTable->setItem(row, 1, new QTableWidgetItem(book.name())); // 教材名
                    Course course = DatabaseManager::searchCourseByid(subscription.courseId());
                    statisticsTable->setItem(row, 2, new QTableWidgetItem(course.name())); // 课程名
                    statisticsTable->setItem(row, 3, new QTableWidgetItem(QString::number(subscription.price()))); // 订阅金额
                }
                row++;
            }

    } else if (index == 2) { // 按课程统计
        // 清空表格内容
        statisticsTable->clearContents();
        statisticsTable->setRowCount(0);
        // 设置表头
        statisticsTable->setHorizontalHeaderLabels({"课程名", "教材名", "订阅数量", "总金额"});
        Textbook book;
        // 查询数据库，获取每个课程的订阅信息
        QList<Course> courses = DatabaseManager::getAllCourses();
        for (const Course& course : courses) {
            // 获取课程的订阅信息
            QList<Subscription> subscriptions = DatabaseManager::getCourseSubscriptions(course.id());

            // 计算订阅总数和总金额
//            int totalSubscriptions = subscriptions.size();
            double totalAmount = 0.0;
            for (const Subscription& subscription : subscriptions) {
                totalAmount += subscription.price();
                // 添加课程的统计信息到表格中
                book = DatabaseManager::searchTextbookByid(subscription.bookId());
            }
            int totalSubscriptionCount = totalAmount/book.price();
            int row = statisticsTable->rowCount();
            statisticsTable->insertRow(row);
            statisticsTable->setItem(row, 0, new QTableWidgetItem(course.name())); // 课程名
            statisticsTable->setItem(row, 1, new QTableWidgetItem(subscriptions.isEmpty() ? "" : book.name())); // 教材名（取第一个订阅记录的教材名）
            statisticsTable->setItem(row, 2, new QTableWidgetItem(QString::number(totalSubscriptionCount))); // 订阅总数
            statisticsTable->setItem(row, 3, new QTableWidgetItem(QString::number(totalAmount))); // 总金额


        }
    }

}

void AdminWidget::onQueryComboBoxActivated(int index)
{
    // 根据选择的查询方式执行相应的操作
    if (index == 0) {
        // 按人员查询
        // 执行相关操作...
        User user;
        Textbook book;
        Course course;
         if(!queryline->text().isEmpty())
            user = DatabaseManager::searchUserByUsername(queryline->text());
         else{
             QMessageBox::warning(this, "警告","请输入相关用户名");
         }

         QList<Subscription> subscriptions = DatabaseManager::getUserSubscriptions(user.id);

        // 清空表格内容
        queryResultTable->clearContents();

        // 设置表格行数
        queryResultTable->setRowCount(subscriptions.size());

        // 填充表格
        int row = 0;
        for (const Subscription& subscription : subscriptions){
            QTableWidgetItem* nameItem = new QTableWidgetItem(user.username);
            book = DatabaseManager::searchTextbookByid(subscription.bookId());
            QTableWidgetItem* textbookItem = new QTableWidgetItem(book.name());
            course = DatabaseManager::searchCourseByid(subscription.courseId());
            QTableWidgetItem* courseItem = new QTableWidgetItem(course.name());
            QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(subscription.price()));
            QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(subscription.price() / book.price()));
            QTableWidgetItem* timeItem = new QTableWidgetItem(subscription.time().toString());

            queryResultTable->setItem(row, 0, nameItem);
            queryResultTable->setItem(row, 1, textbookItem);
            queryResultTable->setItem(row, 2, courseItem);
            queryResultTable->setItem(row, 3, priceItem);
            queryResultTable->setItem(row, 4, quantityItem);
            queryResultTable->setItem(row, 5, timeItem);

            ++row;
        }
    } else if (index == 1) {
        // 按教材查询
        // 执行相关操作...
        User user;
        Textbook book;
        Course course;
         if(!queryline->text().isEmpty())
            book = DatabaseManager::searchTextbookByname(queryline->text());
         else{
             QMessageBox::warning(this, "警告","请输入相关教材名");
         }

         QList<Subscription> subscriptions = DatabaseManager::getBookSubscriptions(book.id());

        // 清空表格内容
        queryResultTable->clearContents();

        // 设置表格行数
        queryResultTable->setRowCount(subscriptions.size());

        // 填充表格
        int row = 0;
        for (const Subscription& subscription : subscriptions) {
            user = DatabaseManager::getUserFromDatabase(subscription.userId());
            QTableWidgetItem* nameItem = new QTableWidgetItem(user.username);
            QTableWidgetItem* textbookItem = new QTableWidgetItem(book.name());
            course = DatabaseManager::searchCourseByid(subscription.courseId());
            QTableWidgetItem* courseItem = new QTableWidgetItem(course.name());
            QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(subscription.price()));
            QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(subscription.price() / book.price()));
            QTableWidgetItem* timeItem = new QTableWidgetItem(subscription.time().toString());

            queryResultTable->setItem(row, 0, nameItem);
            queryResultTable->setItem(row, 1, textbookItem);
            queryResultTable->setItem(row, 2, courseItem);
            queryResultTable->setItem(row, 3, priceItem);
            queryResultTable->setItem(row, 4, quantityItem);
            queryResultTable->setItem(row, 5, timeItem);

            ++row;
        }
    } else if (index == 2) {
        // 按课程查询
        // 执行相关操作...
        User user;
        Textbook book;
        Course course;
         if(!queryline->text().isEmpty())
            course = DatabaseManager::searchCourseByName(queryline->text());
         else{
             QMessageBox::warning(this, "警告","请输入相关课程名");
         }

         QList<Subscription> subscriptions = DatabaseManager::getCourseSubscriptions(course.id());

        // 清空表格内容
        queryResultTable->clearContents();

        // 设置表格行数
        queryResultTable->setRowCount(subscriptions.size());

        // 填充表格
        int row = 0;
        for (const Subscription& subscription : subscriptions) {
            user = DatabaseManager::getUserFromDatabase(subscription.userId());
            QTableWidgetItem* nameItem = new QTableWidgetItem(user.username);
            book = DatabaseManager::searchTextbookByid(subscription.bookId());
            QTableWidgetItem* textbookItem = new QTableWidgetItem(book.name());
            QTableWidgetItem* courseItem = new QTableWidgetItem(course.name());
            QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(subscription.price()));
            QTableWidgetItem* quantityItem = new QTableWidgetItem(QString::number(subscription.price() / book.price()));
            QTableWidgetItem* timeItem = new QTableWidgetItem(subscription.time().toString());

            queryResultTable->setItem(row, 0, nameItem);
            queryResultTable->setItem(row, 1, textbookItem);
            queryResultTable->setItem(row, 2, courseItem);
            queryResultTable->setItem(row, 3, priceItem);
            queryResultTable->setItem(row, 4, quantityItem);
            queryResultTable->setItem(row, 5, timeItem);

            ++row;
        }
    }
}


void AdminWidget::addTextbook()
{
    // 创建一个对话框以获取新教材的信息
    AddTextbookDialog addDialog(this);
    addDialog.setWindowTitle("添加教材");
    // 显示对话框，并等待用户操作完成
    if (addDialog.exec() == QDialog::Accepted) {
        // 用户点击了对话框中的确认按钮

        // 获取用户在对话框中输入的新教材信息
        QString name = addDialog.getName();
        QString author = addDialog.getAuthor();
        double price = addDialog.getPrice();

        // 在数据库中插入新教材的信息
        bool success = DatabaseManager::insertTextbook(name, author, price);

        if (success) {
            // 插入成功，刷新教材表格以显示新数据
            populateTextTable();
            QMessageBox::information(this, "添加教材", "教材添加成功！");
        } else {
            // 插入失败，显示错误消息
            QMessageBox::critical(this, "添加教材", "教材添加失败！");
        }
    }
}


void AdminWidget::deleteTextbook()
{
    // 获取当前选中的行索引
    int selectedRow = textbookTable->currentRow();

    if (selectedRow != -1) {
        QTableWidgetItem *itemId = textbookTable->item(selectedRow, 0);
        if (itemId) {

            int textbookID  = itemId->text().toInt();

            // 在这里执行删除用户的操作
            bool success = DatabaseManager::deleteTextbook(textbookID);

            if (success) {
                populateTextTable();
                // 更新用户界面以反映已删除用户的更改
                QMessageBox::critical(this, "成功", "删除教材成功！");
            } else {
                // 如果删除失败，显示错误消息给用户
                QMessageBox::critical(this, "错误", "删除教材失败！");
            }
        }
    } else {
        // 如果没有选中任何行，显示提示给用户
        QMessageBox::information(this, "提示", "请先选中要删除的教材！");
    }
}

void AdminWidget::editTextbook()
{
    // 获取选中的当前行
    int selectedRow = textbookTable->currentRow();
    if (selectedRow < 0) {
        // 如果没有选中用户，显示错误消息或者返回
        QMessageBox::warning(this, "编辑教材", "请先选中要编辑的教材！");
        return;
    }

    // 获取选中行的教材ID（假设ID位于第一列）
    int textbookId = textbookTable->item(selectedRow, 0)->text().toInt();

    // 从数据库中获取选中教材的信息
    Textbook textbook = DatabaseManager::getTextbookFromDatabase(textbookId);
    if (textbook.isValid()) {
        // 创建一个对话框用于编辑教材信息
        AddTextbookDialog editTextbookDialog(textbook, this);
        editTextbookDialog.setWindowTitle("修改教材信息");
        // 显示对话框，并等待用户操作完成
        if (editTextbookDialog.exec() == QDialog::Accepted) {
            // 用户点击了对话框中的确认按钮

            // 获取用户在对话框中输入的新的用户信息
            QString newBookname = editTextbookDialog.getName();
            QString newBookAuthor = editTextbookDialog.getAuthor();
            double newPrice = editTextbookDialog.getPrice();
            qDebug() << newBookname << " " << newBookAuthor << " " << newPrice;

            // 更新数据库中的用户信息
            textbook.setBookname(newBookname);
            textbook.setAuthor(newBookAuthor);
            textbook.setPrice(newPrice);
            bool success = DatabaseManager::updateTextbookInDatabase(textbook);
            qDebug() << success;
            if (success) {
                populateTextTable();
                QMessageBox::information(this, "编辑教材", "教材信息已更新！");
            } else {

                // 更新失败，显示错误消息
                QMessageBox::critical(this, "编辑教材", "更新教材信息失败！");
            }
        }
    } else {
        // 无法从数据库中获取用户信息，显示错误消息
        QMessageBox::critical(this, "编辑教材", "无法获取选中教材的信息！");
    }
}

void AdminWidget::searchTextbook()
{
    // 获取搜索关键字，这里假设使用 QLineEdit 输入框进行输入
    QString keyword = searchTextbookLineEdit->text().trimmed(); // 获取输入的搜索关键字并去除首尾空格

    // 如果搜索关键字为空，则不执行搜索操作
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "搜索教材", "请输入搜索教材名！");
        return;
    }

    // 执行搜索操作，从数据库中检索用户信息
    Textbook searchResult = DatabaseManager::searchTextbookByname(keyword);

    // 如果搜索结果为空，则提示用户未找到匹配项
    if (!searchResult.isValid()) {
        QMessageBox::information(this, "搜索教材", "未找到匹配教材！");
        return;
    }

    // 显示搜索结果，可以将搜索结果显示在另一个表格中，或者在当前表格中覆盖显示
    // 这里仅做演示，假设将搜索结果显示在当前表格中
    // 清空表格中原有的内容
    textbookTable->clearContents();

    // 设置表格行数为1，因为只有一个搜索结果
    textbookTable->setRowCount(1);

    // 填充搜索结果到表格中
    QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(searchResult.id()));
    QTableWidgetItem* nameItem = new QTableWidgetItem(searchResult.name());
    QTableWidgetItem* authorItem = new QTableWidgetItem(searchResult.author());
    QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(searchResult.price()));

    textbookTable->setItem(0, 0, idItem);
    textbookTable->setItem(0, 1, nameItem);
    textbookTable->setItem(0, 2, authorItem);
    textbookTable->setItem(0, 3, priceItem);
}




void AdminWidget::setupUi()
{
    setWindowTitle("管理员界面");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建左右布局
    splitter = new QSplitter(Qt::Horizontal);

    // 创建左侧导航栏;
    QWidget *navigationWidget = new QWidget;
    navigationLayout = new QVBoxLayout(navigationWidget);

    QPushButton *userManagementButton = new QPushButton("用户管理");
    QPushButton *textbookManagementButton = new QPushButton("教材管理");
    QPushButton *queryButton = new QPushButton("订阅查询");
    QPushButton *statisticsButton = new QPushButton("统计表");

    // 设置按钮的高度、宽度和背景颜色
    const int buttonWidth = 120;
    const int buttonHeight = 40;
    const QString buttonColor = "#87CEFA"; // 浅蓝色
    const QString buttonStyleSheet = QString("QPushButton {"
                                             "    width: %1px;"
                                             "    height: %2px;"
                                             "    background-color: %3;"
                                             "    border: none;" // 去掉按钮边框
                                             "}"
                                             "QPushButton:pressed {"
                                          "    background-color: #6495ED;" // 按下时的背景颜色
                                          "}").arg(buttonWidth).arg(buttonHeight).arg(buttonColor);
    userManagementButton->setStyleSheet(buttonStyleSheet);
    textbookManagementButton->setStyleSheet(buttonStyleSheet);
    queryButton->setStyleSheet(buttonStyleSheet);
    statisticsButton->setStyleSheet(buttonStyleSheet);

    // 创建垂直布局，用于容纳按钮，并设置布局的间距
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(userManagementButton);
    buttonLayout->addWidget(textbookManagementButton);
    buttonLayout->addWidget(queryButton);
    buttonLayout->addWidget(statisticsButton);
    buttonLayout->addStretch(); // 添加伸缩项，使按钮垂直居中

    // 将按钮布局添加到左侧导航栏布局中
    navigationLayout->addLayout(buttonLayout);

    // 创建右侧顶部管理功能和中间结果展示区域
    QWidget *rightWidget = new QWidget;
    rightLayout = new QVBoxLayout(rightWidget);

    rightWidget->setFixedSize(400, 500);


    // 将左右布局添加到分割器中
    splitter->addWidget(navigationWidget);
    splitter->addWidget(rightWidget);

    mainLayout->addWidget(splitter);

    connect(userManagementButton, &QPushButton::clicked, this, &AdminWidget::setupUserManagementPage);
    connect(textbookManagementButton, &QPushButton::clicked, this, &AdminWidget::setupTextbookManagementPage);
    connect(queryButton, &QPushButton::clicked, this, &AdminWidget::setupQueryPage);
    connect(statisticsButton, &QPushButton::clicked, this, &AdminWidget::setupStatisticsPage);


}

void AdminWidget::clearRightLayout()
{
    QLayoutItem *child;
    while ((child = rightLayout->takeAt(0)) != nullptr) {
        if (child->layout() != nullptr) {
                    clearLayout(child->layout()); // 递归清空子布局
                }
        delete child->widget();
        delete child;
    }
}

void AdminWidget::clearLayout(QLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->layout() != nullptr) {
            clearLayout(child->layout()); // 递归清空子布局
        }
        delete child->widget(); // 删除控件
        delete child; // 删除布局项
    }
}

