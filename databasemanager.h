#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql>
#include <QString>
#include <QDateTime>
#include "user.h"
#include "textbook.h"
#include "subscription.h"
#include "course.h"

class DatabaseManager: public QWidget
{
public:
    DatabaseManager(QWidget* parent = nullptr);
    bool open();
    void close();
    bool createTable(const QString &tableName, QWidget *parent);
    static bool verifyUser(const QString& username, const QString &password);
    static bool addUser(const QString& username, const QString& password, const QString& role = QString());
    static QString getUserRole(const QString& username);
    static bool delUser(int id);
    static QList<User> getUserListFromDatabase();
    static bool updateUserInDatabase(const User& user);
    static User getUserFromDatabase(int userId);
    static User searchUserByUsername(const QString &username);
    static QList<Textbook> getTextbookListFromDatabase();
    static bool insertTextbook(const QString& name, const QString& author, double price);
    static bool deleteTextbook(int textbookID);
    static Textbook getTextbookFromDatabase(int textbookId);
    static bool updateTextbookInDatabase(const Textbook& book);
    static Textbook searchTextbookByname(const QString &bookname);
    static Textbook searchTextbookByid(int id);
    static Course  searchCourseByid(int id);
    static Course searchCourseByName(const QString& courseName);
    static QList<Subscription> getUserSubscriptions(int userid);
    static QList<Subscription> getBookSubscriptions(int bookid);
    static QList<Subscription> getCourseSubscriptions(int courseid);
    static QList<Course> getAllCourses();
    static QList<Textbook> getAllTextbooks();
    static QList<User> getAllUsers();
    static bool addSubscription(int userId, int textbookId, int courseId, double totalPrice, const QDateTime& time);
private:
    QSqlDatabase db;
    static QList<Subscription> getSubscriptionsByField(const QString& field, const QVariant& value);
};

#endif // DATABASEMANAGER_H
