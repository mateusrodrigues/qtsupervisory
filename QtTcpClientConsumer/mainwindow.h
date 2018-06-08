#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
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

public slots:
    void tcpConnect();
    void getData();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString *ip;
};

#endif // MAINWINDOW_H
