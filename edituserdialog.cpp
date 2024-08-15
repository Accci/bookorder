#include "edituserdialog.h"

EditUserDialog::EditUserDialog(QWidget *parent) : QDialog(parent)
{
    setupUI();
}


EditUserDialog::EditUserDialog(const User &user, QWidget *parent) : QDialog(parent)
{
    setupUI();

    // 将用户信息设置到对话框中
    usernameLineEdit->setText(user.username);
    passwordLineEdit->setText(user.password);
    int roleIndex = roleComboBox->findText(user.role);
    if (roleIndex != -1) {
        roleComboBox->setCurrentIndex(roleIndex);
    }
}



void EditUserDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建用户名输入框和标签
    QLabel *usernameLabel = new QLabel("用户名:");
    usernameLineEdit = new QLineEdit;
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLineEdit);

    // 创建密码输入框和标签
    QLabel *passwordLabel = new QLabel("密码:");
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLineEdit);

    // 创建角色选择框和标签
    QLabel *roleLabel = new QLabel("角色:");
    roleComboBox = new QComboBox;
    roleComboBox->addItem("普通用户");
    roleComboBox->addItem("管理员");
    mainLayout->addWidget(roleLabel);
    mainLayout->addWidget(roleComboBox);

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

QString EditUserDialog::getUsername() const
{
    return usernameLineEdit->text();
}

QString EditUserDialog::getPassword() const
{
    return passwordLineEdit->text();
}

QString EditUserDialog::getRole() const
{
    return roleComboBox->currentText();
}
