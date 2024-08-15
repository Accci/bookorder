#ifndef USER_H
#define USER_H
#include <QString>

struct User {
    int id;
    QString username;
    QString password;
    QString role;
    User(){}
    User(int id, const QString& name, const QString& ps, const QString& role)
        : id(id), username(name), password(ps), role(role) {}

    bool isValid() const {
        return !username.isEmpty() && !password.isEmpty();
    }
    void setUsername(const QString& name) {
        username = name;
    }

    void setPassword(const QString& ps) {
        password = ps;
    }

    void setRole(const QString& rl) {
        role = rl;
    }

};

#endif // USER_H
