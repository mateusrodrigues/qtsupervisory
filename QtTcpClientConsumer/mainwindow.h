#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setIpAddr(QString ipAddr);
    void setInterval(int interval);

    void tcpConnect();
    void tcpDisconnect();
    void getData();

private:
    QString ip;
    int interval;

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer *timer;

    void getIps();
};

#endif // MAINWINDOW_H
