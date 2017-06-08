#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void processPendingDatagrams();
    void Update();
    
private:
    Ui::MainWindow *ui;
    QUdpSocket udpSocket;
    QString yy, MM, dd, HH, mm, ss, week;
};

#endif // MAINWINDOW_H
