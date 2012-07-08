#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCalendarWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <sstream>
#include <QDoubleValidator>
#include <QRect>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createPreviewGroupBox();
    setConnect();
    InitUiFunc();

//    pMainLayout->setSizeConstraint( QLayout::SetFixedSize );
//    setLayout( pMainLayout );
    setWindowTitle(tr("Calendar Widget"));
    onConnectMoneyDB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createPreviewGroupBox()
{
    // 初始化日历控件
    m_pCalendarWidget = new QCalendarWidget(this);
    m_pCalendarWidget->setMinimumDate( QDate(2012,1,1) );
    m_pCalendarWidget->setMaximumDate( QDate(2014,1,1) );
    m_pCalendarWidget->setGridVisible( true );
    QDate currentDate = QDate::currentDate();
    QTextCharFormat currentDateFormat;
    currentDateFormat.setForeground( QBrush( QColor(255,30,255) ) );
    m_pCalendarWidget->setDateTextFormat( currentDate, currentDateFormat );
    m_pCalendarWidget->setGeometry( 64, 64, 300, 200 );

    // 设置日历控件布局
//    m_pCalendarLayout = new QGridLayout(this);
//    m_pCalendarLayout->addWidget( m_pCalendarWidget, 0, 0, Qt::AlignAbsolute );
//    m_pCalendarBox = new QGroupBox( "日期" );
//    m_pCalendarBox->setLayout( m_pCalendarLayout );

}

void MainWindow::setConnect()
{
//    connect( ui->BtnConnectMoneyDB, SIGNAL(clicked()), this, SLOT(onConnectMoneyDB()) );
    connect( ui->MouseSelect, SIGNAL(clicked()), this, SLOT(onSelectCurrentMonth())  );
    connect( ui->BtnInserRecord, SIGNAL(clicked()), this, SLOT(onInsertRecord()) );
    connect( m_pCalendarWidget, SIGNAL(selectionChanged()), this, SLOT(onSelectCalendarDate()) );
    connect( ui->DateEdit, SIGNAL(dateChanged(QDate)), m_pCalendarWidget, SLOT(setSelectedDate(QDate)) );
    connect( ui->BtnSelectDay, SIGNAL(clicked()), this, SLOT(onShowSelectedDayRecord()) );
}

void MainWindow::onConnectMoneyDB()
{
    m_SqlDateBase = QSqlDatabase::addDatabase( "QODBC" );
    QString dsn = QString::fromLocal8Bit( "DRIVER={SQL SERVER};SERVER=2011-20120210RS\\SQLEXPRESS;DATABASE=MoneyFirst" );
    m_SqlDateBase.setDatabaseName( dsn );
    m_SqlDateBase.setHostName( "localhost" );
    m_SqlDateBase.setUserName( "MoneyManager" );
    m_SqlDateBase.setPassword( "123456" );
    if( m_SqlDateBase.open() )
    {
        QMessageBox msgBox;
        msgBox.setText( "数据库连接成功" );
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText( m_SqlDateBase.lastError().text() );
        msgBox.exec();
    }

}

void MainWindow::onSelectCurrentMonth()
{
    QSqlQuery query;
    QString str = QString::number( QDate::currentDate().month() );
    if( query.exec( "SELECT * FROM OurMoney" ) )
    {
        ui->MoneyWidget->setRowCount(0);
        while( query.next() )
        {
            QTableWidgetItem *dateItem = new QTableWidgetItem( query.value(0).toString() );
            dateItem->setFlags( dateItem->flags() ^ Qt::ItemIsEditable );
            QTableWidgetItem *moneyItem = new QTableWidgetItem( query.value(1).toString() );
            moneyItem->setFlags( moneyItem->flags() ^ Qt::ItemIsEditable );
            QTableWidgetItem *nameItem = new QTableWidgetItem( query.value(2).toString() );
            nameItem->setFlags( nameItem->flags() ^ Qt::ItemIsEditable );
            QTableWidgetItem *forwhatItem = new QTableWidgetItem( query.value(3).toString() );
            forwhatItem->setFlags( forwhatItem->flags() ^ Qt::ItemIsEditable );

            int nCurrentRow = ui->MoneyWidget->rowCount();
            ui->MoneyWidget->insertRow( nCurrentRow );
            ui->MoneyWidget->setItem( nCurrentRow, 0 ,dateItem );
            ui->MoneyWidget->setItem( nCurrentRow, 1 ,moneyItem );
            ui->MoneyWidget->setItem( nCurrentRow, 2 ,nameItem );
            ui->MoneyWidget->setItem( nCurrentRow, 3 ,forwhatItem );
        }
    }


}

void MainWindow::onInsertRecord()
{
    QString strName = ui->NameEdit->toPlainText();
    QString strReason = ui->ReasonEdit->toPlainText();
    QString strMoney = ui->MoneyEdit->text();
    float fMoney = strMoney.toFloat();

    if( !strName.isEmpty() && !strReason.isEmpty() && fMoney != 0 )
    {
        QSqlQuery query;
        QDateTime time = QDateTime::currentDateTime();
        query.prepare("insert into OurMoney(theDate,money,ProviderName,useFor)"
                      "values(:theDate,:money,:Provider, :useFor)" );
        query.bindValue( ":theDate", time );
        query.bindValue( ":money", strMoney );
        query.bindValue( ":Provider", strName );
        query.bindValue( ":useFor", strReason );
        query.exec();
    }
}

void MainWindow::InitUiFunc()
{

    // 设置日期控件的值
    ui->DateEdit->setDate( QDate::currentDate() );

    // 显示控件设置选中行
    ui->MoneyWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->MoneyWidget->setSelectionMode( QAbstractItemView::SingleSelection );
    // 禁止编辑
    ui->MoneyWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );
    // 设置MoneyEdit只能输入数字
    QDoubleValidator *pdValidator = new QDoubleValidator( 0, 10000, 3, this );
    ui->MoneyEdit->setValidator( pdValidator );
}

void MainWindow::onSelectCalendarDate()
{
    ui->DateEdit->setDate( m_pCalendarWidget->selectedDate() );
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //画渐变背景--线性渐变
    QLinearGradient objLinear(rect().bottomRight(),rect().topRight());
    objLinear.setColorAt(0,Qt::white);
    objLinear.setColorAt(1,Qt::black);
    painter.fillRect(rect(),objLinear);
}

void MainWindow::onShowSelectedDayRecord()
{
    QSqlQuery query;
//    QDateTime time = (QDateTime)ui->DateEdit->date();
    QDateTime time = QDateTime::currentDateTime();
    query.prepare( "exec getSelectDayRecord :time, output,output,output,output" );
    query.bindValue( ":time", "2012-7-4" );
    query.bindValue( 1, 0, QSql::Out );
    query.bindValue( 2, 0, QSql::Out );
    query.bindValue( 3, 0, QSql::Out );
    query.bindValue( 4, 0, QSql::Out );
    if( query.exec() )
    {
        ui->MoneyWidget->setRowCount(0);

        QTableWidgetItem *dateItem = new QTableWidgetItem( query.value( 1 ).toString() );
        dateItem->setFlags( dateItem->flags() ^ Qt::ItemIsEditable );

        int nCurrentRow = ui->MoneyWidget->rowCount();
        ui->MoneyWidget->insertRow( nCurrentRow );
        ui->MoneyWidget->setItem( nCurrentRow, 0,  dateItem );
    }
    else
    {
        QString str = query.lastError().text();
        QMessageBox msg;
        msg.setText( str );
        msg.exec();
    }
}
