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
    /**
     * @brief setIpAddr setIpAddr sets the ip address from which the visualization
     * tool should read the posted the data
     * @param ipAddr is the ip address value
     */
    void setIpAddr(QString ipAddr);
    /**
     * @brief setSource reads the selected source and sets ip address accordingly
     * using the setIpAddr function
     * @param ipAddr is the ip address value
     */
    void setSource(QString ipAddr);
    /**
     * @brief setInterval sets the timing interval in which data should be read
     * @param interval is the interval value in seconds
     */
    void setInterval(int interval);
    /**
     * @brief getIps gets the list of IPs that generated available data in server
     */
    void getIps();

    /**
     * @brief tcpConnect connects to the server according to ip address set
     */
    void tcpConnect();
    /**
     * @brief tcpDisconnect disconnects from the server according to currently
     * connection information
     */
    void tcpDisconnect();

    /**
     * @brief startDataRead fires up the timer and starts the data feed reading
     */
    void startDataRead();
    /**
     * @brief stopDataRead halts the timer and stops the data feed reading
     */
    void stopDataRead();

    /**
     * @brief getData pulls and deserializes data from the server
     */
    void getData();

private:
    QString ip, source;
    int interval;

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer *timer;
};

#endif // MAINWINDOW_H
