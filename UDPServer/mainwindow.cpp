#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket.bind(5678);//绑定端口号
    connect(&udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processPendingDatagrams(){
    QByteArray datagram;
    do{
        datagram.resize(udpSocket.pendingDatagramSize());//让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        udpSocket.readDatagram(datagram.data(),datagram.size());//接收数据报，将其存放到datagram中
        char* str = datagram.data();
        HH= QString::number(str[0], 16);
        mm =  QString::number(str[1], 16);
        ss =  QString::number(str[2], 16);
        week =  QString::number(str[3], 16);
        MM =  QString::number(str[4], 16);
        dd =  QString::number(str[5], 16);
        yy = QString::number(str[6], 16);
        Update();
        qDebug() << QString(str);
    }while(udpSocket.hasPendingDatagrams());//拥有等待的数据报
}

void MainWindow::Update()
{
    ui->year->setText("200"+yy);
    ui->month->setText(MM);
    ui->day->setText(dd);
    ui->week->setText(week);
    ui->hour->setText(HH);
    ui->min->setText(mm);
    ui->second->setText(ss);
}
