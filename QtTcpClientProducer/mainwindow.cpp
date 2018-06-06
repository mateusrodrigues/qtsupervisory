#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    // Connect button
    connect(ui->btnConnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpConnect()));
    // Disconnect button
    connect(ui->btnDisconnet,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpDisconnect()));
}

void MainWindow::setIpAddr(QString ipAddr)
{
    this->ip = ipAddr;
}

void MainWindow::tcpConnect()
{
    socket->connectToHost(ip,1234);
    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
    }
    else
    {
        qDebug() << "Disconnected";
    }
}

void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();
    if (socket->waitForDisconnected(3000))
    {
        qDebug() << "Disconnected";
    }
}

void MainWindow::putData()
{
    QDateTime datetime;
    QString str;
    qint64 msecdate;

    if(socket->state()== QAbstractSocket::ConnectedState)
    {
        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " + QString::number(qrand()%35)+"\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        if(socket->waitForBytesWritten(3000))
        {
            qDebug() << "wrote";
        }
    }
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
