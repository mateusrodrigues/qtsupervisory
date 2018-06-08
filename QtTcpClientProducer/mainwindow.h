#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QStringListModel>

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
    void setMin(int min);
    void setMax(int max);

    void putData();
    void tcpConnect();
    void tcpDisconnect();
    void startFeed();
    void stopFeed();

private:
    QString ip;
    int min, max;

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer *timer;
    QStringList *list;
    QStringListModel *listModel;
};

#endif // MAINWINDOW_H
