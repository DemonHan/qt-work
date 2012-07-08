#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
    class MainWindow;
}

class QCalendarWidget;
class QGroupBox;
class QGridLayout;
class QSqlDatabase;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private:
    QCalendarWidget *m_pCalendarWidget;
//    QGroupBox *m_pCalendarBox;
//    QGridLayout *m_pCalendarLayout;
    // 用于连接金钱数据库
    QSqlDatabase m_SqlDateBase;
private:
    void setConnect();
    // 设置日期界面
    void createPreviewGroupBox();
    // 初始化UI
    void InitUiFunc();
    // 连接数据库
    void onConnectMoneyDB();
private slots:
    // 查询本月记录
    void onSelectCurrentMonth();
    void onShowSelectedDayRecord();
    // 插入新的数据
    void onInsertRecord();
    // 选择日历日期
    void onSelectCalendarDate();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
