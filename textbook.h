#ifndef TEXTBOOK_H
#define TEXTBOOK_H
#include <QString>
class Textbook {
public:
    Textbook(){}
    Textbook(int id, const QString& name, const QString& author, double price)
        : m_id(id), m_name(name), m_author(author), m_price(price) {}

    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString author() const { return m_author; }
    double price() const { return m_price; }
    bool isValid() const
    {
        return !m_name.isEmpty() && !m_author.isEmpty() && m_price;
    }

    void setBookname(const QString& bn)
    {
        m_name = bn;
    }
    void setAuthor(const QString& author)
    {
        m_author = author;
    }
    void setPrice(const double price)
    {
        m_price = price;
    }

private:
    int m_id;
    QString m_name;
    QString m_author;
    double m_price;
};

#endif // TEXTBOOK_H
