#ifndef SUBSCRIPTIONRESULTITEM_H
#define SUBSCRIPTIONRESULTITEM_H

#include <QString>
#include <QList>

class SubscriptionResultItem {
public:
    SubscriptionResultItem(const QString& username, const QString& textbookName, double subscriptionPrice, int subscriptionQuantity, const QString& subscriptionTime)
        : m_username(username), m_textbookName(textbookName), m_subscriptionPrice(subscriptionPrice), m_subscriptionQuantity(subscriptionQuantity), m_subscriptionTime(subscriptionTime) {}

    QString username() const { return m_username; }
    QString textbookName() const { return m_textbookName; }
    double subscriptionPrice() const { return m_subscriptionPrice; }
    int subscriptionQuantity() const { return m_subscriptionQuantity; }
    QString subscriptionTime() const { return m_subscriptionTime; }

private:
    QString m_username;
    QString m_textbookName;
    double m_subscriptionPrice;
    int m_subscriptionQuantity;
    QString m_subscriptionTime;
};

class SubscriptionResultSet {
public:
    SubscriptionResultSet(const QStringList& headers) : m_headers(headers) {}

    QStringList headers() const { return m_headers; }

    void addResultItem(const SubscriptionResultItem& item) {
        m_results.append(item);
    }

    QList<SubscriptionResultItem> results() const { return m_results; }

private:
    QStringList m_headers;
    QList<SubscriptionResultItem> m_results;
};


#endif // SUBSCRIPTIONRESULTITEM_H
