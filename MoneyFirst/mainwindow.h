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
    // �������ӽ�Ǯ���ݿ�
    QSqlDatabase m_SqlDateBase;
private:
    void setConnect();
    // �������ڽ���
    void createPreviewGroupBox();
    // ��ʼ��UI
    void InitUiFunc();
    // �������ݿ�
    void onConnectMoneyDB();
private slots:
    // ��ѯ���¼�¼
    void onSelectCurrentMonth();
    void onShowSelectedDayRecord();
    // �����µ�����
    void onInsertRecord();
    // ѡ����������
    void onSelectCalendarDate();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
