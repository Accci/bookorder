#include "userwidget.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "databasemanager.h"
#include <QTableWidget>

UserWidget::UserWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

}

UserWidget::~UserWidget()
{

}

void UserWidget::setupUserPage()
{
    // 清空右侧布局
    clearRightLayout();

    // 创建布局
    QVBoxLayout* userLayout = new QVBoxLayout;

    // 创建水平布局用于显示和修改用户名
    QHBoxLayout* usernameLayout = new QHBoxLayout;
    QLabel* usernameLabel = new QLabel("用户名:");
    QLineEdit* usernameLineEdit = new QLineEdit;
    usernameLineEdit->setText(currentUser.username); // 显示当前用户的用户名
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLineEdit);

    // 创建水平布局用于显示和修改密码
    QHBoxLayout* passwordLayout = new QHBoxLayout;
    QLabel* passwordLabel = new QLabel("密码:");
    QLineEdit* passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password); // 设置为密码模式
    passwordLineEdit->setText(currentUser.password); // 显示当前用户的密码
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLineEdit);

    // 创建按钮用于修改用户信息
    QPushButton* editUserInfoButton = new QPushButton("修改用户信息");
    connect(editUserInfoButton, &QPushButton::clicked, [=]() {
        QString passwd = passwordLineEdit->text();
        QString username = usernameLineEdit->text();
        currentUser.setUsername(username);
        currentUser.setPassword(passwd);
        bool success = DatabaseManager::updateUserInDatabase(currentUser);
        if(success)
            QMessageBox::information(this, "成功", "修改用户信息成功");
        else
            QMessageBox::information(this, "失败", "修改用户信息失败");
    });

    // 将控件添加到布局中
    userLayout->addLayout(usernameLayout);
    userLayout->addLayout(passwordLayout);
    userLayout->addWidget(editUserInfoButton);

    // 将布局添加到右侧布局中
    rightLayout->addLayout(userLayout);
}



void UserWidget::setupTextbookOrderPage()
{
    // 清空右侧布局
    clearRightLayout();

    // 创建布局
    QVBoxLayout* orderLayout = new QVBoxLayout;

    // 创建下拉框用于选择教材
    QLabel* textbookLable = new QLabel("选择教材");
    textbookComboBox = new QComboBox;

    // 从数据库中获取教材信息并添加到下拉框中
    QList<Textbook> textbooks = DatabaseManager::getAllTextbooks();
    for (const Textbook& textbook : textbooks) {
        textbookComboBox->addItem(textbook.name());
    }

    // 创建下拉框用于选择课程
    QLabel* courseLable = new QLabel("选择课程");
    courseComboBox = new QComboBox;

    // 从数据库中获取课程信息并添加到下拉框中
    QList<Course> courses = DatabaseManager::getAllCourses();
    for (const Course& course : courses) {
        courseComboBox->addItem(course.name());
    }

    // 创建文本框用于输入订阅教材数量
    quantityLineEdit = new QLineEdit;
    quantityLineEdit->setPlaceholderText("输入订阅数量");

    // 创建按钮用于订阅教材
    QPushButton* subscribeButton = new QPushButton("订阅");
    connect(subscribeButton, &QPushButton::clicked, this, &UserWidget::subscribeTextbook);

    // 创建表格用于显示订阅信息
    subscriptionsTable = new QTableWidget;
    subscriptionsTable->setColumnCount(6); // 设置表格列数
    QStringList headers = {"用户名", "教材", "课程", "订阅数量", "总金额", "订阅时间"}; // 设置表头
    subscriptionsTable->setHorizontalHeaderLabels(headers);

    // 将控件添加到布局中
    orderLayout->addWidget(textbookLable);
    orderLayout->addWidget(textbookComboBox);
    orderLayout->addWidget(courseLable);
    orderLayout->addWidget(courseComboBox);
    orderLayout->addWidget(quantityLineEdit);
    orderLayout->addWidget(subscribeButton);
    orderLayout->addWidget(subscriptionsTable);

    // 将布局添加到右侧布局中
    rightLayout->addLayout(orderLayout);
}

bool UserWidget::subscribeTextbook()
{
    // 获取当前用户名
    QString username = currentUser.username;

    // 获取选择的教材和课程
    QString textbookName = textbookComboBox->currentText();
    QString courseName = courseComboBox->currentText();

    // 获取订阅数量
    int quantity = quantityLineEdit->text().toInt();

    // 如果订阅数量小于等于0，则返回false
    if (quantity <= 0) {
        QMessageBox::warning(this, "订阅失败", "订阅数量必须大于0！");
        return false;
    }

    // 查询数据库获取教材和课程的价格
    Textbook textbook = DatabaseManager::searchTextbookByname(textbookName);
    Course course = DatabaseManager::searchCourseByName(courseName);

    // 计算总金额
    double totalPrice = quantity * textbook.price();

    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    qDebug() << currentUser.id;
    // 将订阅订单添加到数据库
    if (DatabaseManager::addSubscription(currentUser.id, textbook.id(), course.id(),  totalPrice, currentTime)) {
        // 添加订阅信息到表格中
        int row = 0;
        subscriptionsTable->clearContents();
        subscriptionsTable->insertRow(row);
        subscriptionsTable->setItem(row, 0, new QTableWidgetItem(username));
        subscriptionsTable->setItem(row, 1, new QTableWidgetItem(textbookName));
        subscriptionsTable->setItem(row, 2, new QTableWidgetItem(courseName));
        subscriptionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(quantity)));
        subscriptionsTable->setItem(row, 4, new QTableWidgetItem(QString::number(totalPrice)));
        subscriptionsTable->setItem(row, 5, new QTableWidgetItem(currentTime.toString(Qt::ISODate)));

        QMessageBox::information(this, "订阅成功", "教材订阅成功！");
        return true;
    } else {
        QMessageBox::warning(this, "订阅失败", "教材订阅失败，请稍后重试！");
        return false;
    }
}



void UserWidget::setupUi()
{
    setWindowTitle("用户界面");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建左右布局
    splitter = new QSplitter(Qt::Horizontal);

    // 创建左侧导航栏;
    QWidget *navigationWidget = new QWidget;
    QVBoxLayout* navigationLayout = new QVBoxLayout(navigationWidget);

    QPushButton *userButton = new QPushButton("用户信息");
    QPushButton *textbookOrderButton = new QPushButton("订阅教材");

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
    userButton->setStyleSheet(buttonStyleSheet);
    textbookOrderButton->setStyleSheet(buttonStyleSheet);

    // 创建垂直布局，用于容纳按钮，并设置布局的间距
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(userButton);
    buttonLayout->addWidget(textbookOrderButton);

    buttonLayout->addStretch(); // 添加伸缩项，使按钮垂直居中

    // 将按钮布局添加到左侧导航栏布局中
    navigationLayout->addLayout(buttonLayout);

    // 创建右侧顶部管理功能和中间结果展示区域
    QWidget *rightWidget = new QWidget;
    rightLayout = new QVBoxLayout(rightWidget);

    rightWidget->setFixedSize(300, 500);


    // 将左右布局添加到分割器中
    splitter->addWidget(navigationWidget);
    splitter->addWidget(rightWidget);

    mainLayout->addWidget(splitter);

    connect(userButton, &QPushButton::clicked, this, &UserWidget::setupUserPage);
    connect(textbookOrderButton, &QPushButton::clicked, this, &UserWidget::setupTextbookOrderPage);
}

void UserWidget::clearRightLayout()
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

void UserWidget::clearLayout(QLayout *layout)
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
