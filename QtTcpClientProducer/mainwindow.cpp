#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <arpa/inet.h>

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
    struct sockaddr_in sa;
    const char* ipAddrCStr = ipAddr.toStdString().c_str();

    int isValid = inet_pton(AF_INET, ipAddrCStr, &(sa.sin_addr));
    if (isValid == 0)
    {
        QPalette palette = ui->editIpAddr->palette();
        palette.setColor(ui->editIpAddr->foregroundRole(), Qt::red);
        ui->editIpAddr->setPalette(palette);
    }
    else
    {
        QPalette palette = ui->editIpAddr->palette();
        palette.setColor(ui->editIpAddr->foregroundRole(), Qt::darkGreen);
        ui->editIpAddr->setPalette(palette);

        this->ip = ipAddr;
    }
}

void MainWindow::setMin(int min)
{
    this->min = min;
}

void MainWindow::setMax(int max)
{
    this->max = max;
}

void MainWindow::tcpConnect()
{
    socket->connectToHost(ip,1234);
    if(socket->waitForConnected(3000))
    {
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(true);

        statusBar()->showMessage("Connected!");
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

    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(false);
    statusBar()->showMessage("Disconnected!");
    qDebug() << "Disconnected";
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
