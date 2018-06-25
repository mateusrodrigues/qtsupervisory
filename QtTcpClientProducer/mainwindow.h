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
    /**
     * @brief MainWindow is the class constructor
     * @param parent is the parent QWidget
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief setIpAddr sets the ip address to which the generation
     * tool should post the data
     * @param ipAddr is the ip address value
     */
    void setIpAddr(QString ipAddr);
    /**
     * @brief setMin sets the minimum value for posted data range
     * @param min is the minimum value
     */
    void setMin(int min);
    /**
     * @brief setMax sets the maximum value for posted data range
     * @param max is the maximum value
     */
    void setMax(int max);

    /**
     * @brief putData posts a randomly-generated value to the server
     */
    void putData();
    /**
     * @brief updateProgressBar counts the time left for next putData trigger,
     * according to timer, and updates the progress bar accordingly
     */
    void updateProgressBar();
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
     * @brief startFeed starts up the timer to trigger the data feed
     */
    void startFeed();
    /**
     * @brief stopFeed stops the data feed timer
     */
    void stopFeed();

private:
    QString ip;
    int min, max, interval, currSecs;

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer *progTimer;
    QStringList *list;
    QStringListModel *listModel;
};

#endif // MAINWINDOW_H
