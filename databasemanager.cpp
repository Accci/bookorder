#include "databasemanager.h"
#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
DatabaseManager::DatabaseManager(QWidget *parent):QWidget (parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bookdb");
}

bool DatabaseManager::open()
{
    if(!db.open()){
        qDebug() << "失败: 无法打开数据库";
        return false;
    }
        return true;
}

void DatabaseManager::close()
{
    db.close();
}

bool DatabaseManager::createTable(const QString &tableName, QWidget *parent)
{
    QSqlQuery query;
    QString createQuery;

    if (tableName == "User") {
            createQuery = "CREATE TABLE IF NOT EXISTS User (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, password TEXT NOT NULL, role TEXT)";
        } else if (tableName == "Textbook") {
            createQuery = "CREATE TABLE IF NOT EXISTS Textbook (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, author TEXT NOT NULL, price REAL NOT NULL)";
        } else if (tableName == "Course") {
            createQuery = "CREATE TABLE IF NOT EXISTS Course (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL)";
        } else if (tableName == "Subscription") {
            createQuery = "CREATE TABLE IF NOT EXISTS Subscription (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, userId INTEGER NOT NULL, textbookId INTEGER NOT NULL, courseId INTEGER NOT NULL, time TEXT NOT NULL, price REAL NOT NULL)";
        } else{
        if (parent) {
            QMessageBox::critical(parent, "失败", QString("未知数据库表: %1").arg(tableName));
        }
        return false;
    }

    if (!query.exec(createQuery)) {
        if (parent) {
            QMessageBox::critical(parent, "失败", QString("无法创建数据库表 '%1': %2").arg(tableName).arg(query.lastError().text()));
        }
        return false;
    } else {
        if (parent) {
            QMessageBox::information(parent, "成功", QString("数据库表 '%1' 创建成功").arg(tableName));
        }
        return true;
    }
}

bool DatabaseManager::verifyUser(const QString &username, const QString &password)
{
        // 执行查询，检查用户名和密码是否匹配
        QSqlQuery query;
        query.prepare("SELECT * FROM User WHERE username = :username AND password = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        if (!query.exec() || !query.next()) {
            qDebug() << "Error: Unable to execute query or user not found";
            return false;
        }

        // 用户验证成功
        return true;
}

bool DatabaseManager::addUser(const QString& username, const QString& password, const QString& role) {

        // 检查用户名是否已存在
        QSqlQuery query;
        query.prepare("SELECT * FROM User WHERE username = :username");
        query.bindValue(":username", username);
        if (query.exec() && query.next()) {
            qDebug() << "Error: User already exists";
            return false;
        }
        // 添加新用户到数据库
        query.prepare("INSERT INTO User (username, password, role) VALUES (:username, :password, :role)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        if(role.isEmpty() || role == "普通用户")
            query.bindValue(":role", "normal");
        else {
            query.bindValue(":role", "admin");
        }
        if (!query.exec()) {
            qDebug() << query.lastError();
            return false;
        }

        // 用户添加成功
        return true;
}

QString DatabaseManager::DatabaseManager::getUserRole(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT role FROM User WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Error: Unable to get user role from database";
        return QString(); // 返回空字符串表示错误
    }
}

bool DatabaseManager::delUser(int id)
{
    QSqlQuery query;
    // 执行删除用户的 SQL 查询语句
    query.prepare("DELETE FROM User WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to delete user!";
        return false;
    }
    return true;
}

QList<User> DatabaseManager::getUserListFromDatabase()
{
   QList<User> userList;
   QSqlQuery query;
   query.prepare("SELECT * FROM User");
   if(query.exec()){
    while (query.next()) {
           int id = query.value("id").toInt();
           QString username = query.value("username").toString();
           QString password = query.value("password").toString();
           QString role = query.value("role").toString();

           // 创建用户对象并添加到列表中
           User user(id, username, password, role);
           userList.append(user);
       }

   }else{
       // 查询失败处理
       qDebug() << "Failed to execute query:" << query.lastError().text();
   }
   return userList;
}

bool DatabaseManager::updateUserInDatabase(const User &user)
{
    QSqlQuery query;
    query.prepare("UPDATE User SET password = :password, role = :role,username = :username WHERE id = :id");
    query.bindValue(":id", user.id);
    query.bindValue(":username", user.username);
    query.bindValue(":password", user.password);
    query.bindValue(":role", user.role);
    if (query.exec()) {
        qDebug() << "用户信息更新成功";
        return true;
    } else {
        qDebug() << "用户信息更新失败: " << query.lastError().text();
        return false;
    }
}

User DatabaseManager::getUserFromDatabase(int userId) {
    User user;
    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE id = :id");
    query.bindValue(":id", userId);
    if (query.exec()) {
        if (query.next()) {
            int id = query.value("id").toInt();
            QString username = query.value("username").toString();
            QString password = query.value("password").toString();
            QString role = query.value("role").toString();
            user = User(id, username, password, role);
        }
    } else {
        qDebug() << "Error getting user from database:" << query.lastError().text();
    }
    return user;
}

User DatabaseManager::searchUserByUsername(const QString &username)
{
    User user;

    // 执行 SQL 查询从数据库中检索匹配给定用户名的用户信息
    QSqlQuery query;
    query.prepare("SELECT id, username, password, role FROM User WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        int id = query.value("id").toInt();
        QString username = query.value("username").toString();
        QString password = query.value("password").toString();
        QString role = query.value("role").toString();
        user = User(id, username, password, role);
    } else {
        qDebug() << "搜索用户失败: " << query.lastError().text();
    }

    return user;
}

QList<Textbook> DatabaseManager::getTextbookListFromDatabase()
{
    QList<Textbook> textbookList;

    // 查询数据库中的教材信息
    QSqlQuery query("SELECT * FROM Textbook");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString author = query.value("author").toString();
        double price = query.value("price").toDouble();

        Textbook textbook(id, name, author, price);
        textbookList.append(textbook);
    }

    return textbookList;
}


bool DatabaseManager::insertTextbook(const QString& name, const QString& author, double price)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Textbook (name, author, price) VALUES (:name, :author, :price)");
    query.bindValue(":name", name);
    query.bindValue(":author", author);
    query.bindValue(":price", price);
    if (query.exec()) {
        qDebug() << "教材信息插入成功";
        return true;
    } else {
        qDebug() << "教材信息插入失败: " << query.lastError().text();
        return false;
    }
}

bool DatabaseManager::deleteTextbook(int textbookID)
{
    QSqlQuery query;
    // 执行删除用户的 SQL 查询语句
    query.prepare("DELETE FROM Textbook WHERE id = :id");
    query.bindValue(":id", textbookID);

    if (!query.exec()) {
        qDebug() << "Failed to delete user!";
        return false;
    }
    return true;
}

Textbook DatabaseManager::getTextbookFromDatabase(int textbookId)
{
    Textbook book;
    QSqlQuery query;
    query.prepare("SELECT * FROM Textbook WHERE id = :id");
    query.bindValue(":id", textbookId);
    if (query.exec()) {
        if (query.next()) {
            int id = query.value("id").toInt();
            QString bookName = query.value("name").toString();
            QString bookAuthor = query.value("author").toString();
            double price = query.value("price").toDouble();
            book = Textbook(id, bookName, bookAuthor, price);
        }
    } else {
        qDebug() << "Error getting user from database:" << query.lastError().text();
    }
    return book;
}

bool DatabaseManager::updateTextbookInDatabase(const Textbook &book)
{
    QSqlQuery query;
    query.prepare("UPDATE Textbook SET name = :name, author = :author, price = :price WHERE id = :id");
    query.bindValue(":id", book.id());
    query.bindValue(":name", book.name());
    query.bindValue(":author", book.author());
    query.bindValue(":price", book.price());
    if (query.exec()) {
        qDebug() << "教材信息更新成功";
        return true;
    } else {
        qDebug() << "教材信息更新失败: " << query.lastError().text();
        return false;
    }
}

Textbook DatabaseManager::searchTextbookByname(const QString &bookname)
{
    Textbook book;

    // 执行 SQL 查询从数据库中检索匹配给定用户名的用户信息
    QSqlQuery query;
    query.prepare("SELECT id, name, author, price FROM Textbook WHERE name = :name");
    query.bindValue(":name", bookname);
    if (query.exec() && query.next()) {
        int id = query.value("id").toInt();
        QString bookname = query.value("name").toString();
        QString author= query.value("author").toString();
        double price = query.value("price").toDouble();
        book = Textbook(id, bookname, author, price);
    } else {
        qDebug() << "搜索教材失败: " << query.lastError().text();
    }

    return book;
}
QList<Subscription> DatabaseManager::getSubscriptionsByField(const QString& field, const QVariant& value)
{
    QList<Subscription> subscriptions;
    QSqlQuery query;
    query.prepare(QString("SELECT id, userId, textbookId, courseId, price, time FROM Subscription WHERE %1 = :value").arg(field));
    query.bindValue(":value", value);
    if (query.exec()) {
        while (query.next()) {
            int subscriptionId = query.value("id").toInt();
            int userId = query.value("userId").toInt();
            int bookId = query.value("textbookId").toInt();
            int courseId = query.value("courseId").toInt();
            double price = query.value("price").toDouble();
            QDateTime time = query.value("time").toDateTime();

            Subscription subscription(subscriptionId, userId, bookId, courseId, price, time);
            subscriptions.append(subscription);
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return subscriptions;
}

QList<Subscription> DatabaseManager::getUserSubscriptions(int userId)
{
    return getSubscriptionsByField("userId", userId);
}

QList<Subscription> DatabaseManager::getBookSubscriptions(int bookId)
{
    return getSubscriptionsByField("textbookId", bookId);
}

QList<Subscription> DatabaseManager::getCourseSubscriptions(int courseId)
{
    return getSubscriptionsByField("courseId", courseId);
}

QList<Course> DatabaseManager::getAllCourses()
{
    QList<Course> courses;

    // 执行查询数据库中所有课程的操作
    QSqlQuery query;
    if (query.exec("SELECT id, name FROM Course")) {
        while (query.next()) {
            int courseId = query.value("id").toInt();
            QString courseName = query.value("name").toString();
            Course course(courseId, courseName);
            courses.append(course);
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return courses;
}

QList<Textbook> DatabaseManager::getAllTextbooks()
{
    QList<Textbook> books;

    // 执行 SQL 查询从数据库中检索匹配给定用户名的用户信息
    QSqlQuery query;
    query.prepare("SELECT * FROM Textbook ");
    if (query.exec()) {
        while(query.next()){
            int id = query.value("id").toInt();
            QString bookname = query.value("name").toString();
            QString author= query.value("author").toString();
            double price = query.value("price").toDouble();
            Textbook textbook(id, bookname, author, price);
            books.append(textbook);
        }
    } else {
        qDebug() << "搜索教材失败: " << query.lastError().text();
    }

    return books;
}

QList<User> DatabaseManager::getAllUsers()
{
    QList<User> users;

    // 执行 SQL 查询从数据库中检索匹配给定用户名的用户信息
    QSqlQuery query;
    query.prepare("SELECT * FROM User");
    if (query.exec()) {
        while(query.next()){
            int id = query.value("id").toInt();
            QString username = query.value("username").toString();
            QString password = query.value("password").toString();
            QString role = query.value("role").toString();
            User user(id, username, password, role);
            users.append(user);
        }
    } else {
        qDebug() << "搜索教材失败: " << query.lastError().text();
    }

    return users;
}

bool DatabaseManager::addSubscription(int userId, int textbookId, int courseId,  double totalPrice, const QDateTime &time)
{
    QSqlQuery query;
        query.prepare("INSERT INTO Subscription (userId, textbookId, courseId, price, time) VALUES (:userId, :textbookId, :courseId, :price, :time)");
        query.bindValue(":userId", userId);
        query.bindValue(":textbookId", textbookId);
        query.bindValue(":courseId", courseId);
        query.bindValue(":price", totalPrice);
        query.bindValue(":time", time);

        if (query.exec()) {
            return true;
        } else {
            qDebug() << "Error executing query:" << query.lastError().text();
            return false;
        }
}


Textbook DatabaseManager::searchTextbookByid(int bookid)
{
    Textbook textbook;

    // 连接数据库并执行查询
    QSqlQuery query;
    query.prepare("SELECT id, name, author, price FROM Textbook WHERE id = :id");
    query.bindValue(":id", bookid);
    if (query.exec() && query.next()) {
        int textbookId = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString author = query.value("author").toString();
        double price = query.value("price").toDouble();

        textbook = Textbook(textbookId, name, author, price);
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return textbook;
}

Course DatabaseManager::searchCourseByid(int courseid)
{
    Course course;

    // 连接数据库并执行查询
    QSqlQuery query;
    query.prepare("SELECT id, name FROM Course WHERE id = :id");
    query.bindValue(":id", courseid);
    if (query.exec() && query.next()) {
        int courseId = query.value("id").toInt();
        QString name = query.value("name").toString();
        course = Course(courseId, name);
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return course;
}

Course DatabaseManager::searchCourseByName(const QString &courseName)
{
    Course course;

    // 执行 SQL 查询从数据库中检索匹配给定用户名的用户信息
    QSqlQuery query;
    query.prepare("SELECT id, name FROM Course WHERE name = :name");
    query.bindValue(":name", courseName);
    if (query.exec() && query.next()) {
        int id = query.value("id").toInt();
        QString coursename = query.value("name").toString();
        course = Course(id, coursename);
    } else {
        qDebug() << "搜索教材失败: " << query.lastError().text();
    }

    return course;
}






