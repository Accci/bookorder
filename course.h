#ifndef COURSE_H
#define COURSE_H
#include <QString>

class Course {
public:
    Course(){}
    Course(int id, const QString& name) : m_id(id), m_name(name) {}

    int id() const { return m_id; }
    QString name() const { return m_name; }

private:
    int m_id;
    QString m_name;
};

#endif // COURSE_H
