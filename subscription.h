#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <QString>
#include <QDateTime>

class Subscription {
public:
    Subscription(int id, int userId, int bookId, int courseId, double price, const QDateTime& time)
        : m_id(id), m_userId(userId), m_bookId(bookId), m_courseId(courseId), m_price(price), m_time(time) {}

    int id() const { return m_id; }
    int userId() const { return m_userId; }
    int bookId() const { return m_bookId; }
    int courseId() const { return m_courseId; }
    double price() const { return m_price; }
    QDateTime time() const { return m_time; }

private:
    int m_id;
    int m_userId;
    int m_bookId;
    int m_courseId;
    double m_price;
    QDateTime m_time;
};

#endif // SUBSCRIPTION_H

