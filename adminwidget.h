#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QComboBox>

class QSplitter;
class QTreeView;
class QTextEdit;

class AdminWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWidget(QWidget *parent = nullptr);
    ~AdminWidget();
    void setupUserManagementPage();
    void setupTextbookManagementPage();
    void setupQueryPage();
    void setupStatisticsPage();

    //用户管理
    void populateUserTable();
//    QTableWidget* paginateUserTable();
    void addUser();
    void deleteUser();
    void editUser();
    void searchUser();
    //教材管理
    void populateTextTable();
    void addTextbook();
    void deleteTextbook();
    void editTextbook();
    void searchTextbook();
    //订阅管理，统计管理
    void calculateStatistics(int index);
    void onQueryComboBoxActivated(int index);

private:
    void setupUi();
    void clearRightLayout();
    void clearLayout(QLayout *layout);
    void copyTableWidget(QTableWidget *source, QTableWidget *destination);

    QSplitter *splitter; // 用于分割左右两个部分
    QVBoxLayout *rightLayout; // 右侧管理功能区域的布局
    QVBoxLayout *navigationLayout;
    QTableWidget *statisticsTable; // 用于显示统计结果的表格
    QComboBox *statisticsComboBox;
    QTableWidget* userTable;
    QLineEdit *searchLineEdit;
    QTableWidget* textbookTable;
    QLineEdit* searchTextbookLineEdit;
    QTableWidget* queryResultTable;
    QLineEdit *queryline;

};

#endif // ADMINWIDGET_H
