#ifndef ADDTEXTBOOKDIALOG_H
#define ADDTEXTBOOKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "Textbook.h" // 假设Textbook类的定义在Textbook.h中

class AddTextbookDialog : public QDialog
{
    Q_OBJECT

public:
    AddTextbookDialog(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setupUI();
    }

    AddTextbookDialog(const Textbook& book, QWidget *parent ): QDialog(parent)
    {
        setupUI();
        nameLineEdit->setText(book.name());
        authorLineEdit->setText(book.author());
        priceLineEdit->setText(QString::number(book.price()));
    }

    QString getName()const
    {
        return nameLineEdit->text();
    }
    QString getAuthor()const
    {
        return authorLineEdit->text();
    }
    double getPrice()const
    {
        return priceLineEdit->text().toDouble();
    }


private:
    QLineEdit *nameLineEdit;
    QLineEdit *authorLineEdit;
    QLineEdit *priceLineEdit;

    void setupUI()
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 创建教材名输入框和标签
        QLabel *nameLabel = new QLabel("教材名:");
        nameLineEdit = new QLineEdit;
        mainLayout->addWidget(nameLabel);
        mainLayout->addWidget(nameLineEdit);

        // 创建作者输入框和标签
        QLabel *authorLabel = new QLabel("作者:");
        authorLineEdit = new QLineEdit;
        mainLayout->addWidget(authorLabel);
        mainLayout->addWidget(authorLineEdit);

        // 创建单价输入框和标签
        QLabel *priceLabel = new QLabel("单价:");
        priceLineEdit = new QLineEdit;
        mainLayout->addWidget(priceLabel);
        mainLayout->addWidget(priceLineEdit);

        // 创建确认和取消按钮
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *okButton = new QPushButton("确认");
        QPushButton *cancelButton = new QPushButton("取消");
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        // 连接确认和取消按钮的点击信号与槽函数
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }
};

#endif // ADDTEXTBOOKDIALOG_H
