#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <arpa/inet.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    timer = new QTimer(this);
    tcpConnect();

    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(getData()));
    connect(ui->buttonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));
    connect(ui->buttonDisconnet,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpDisconnect()));
}

void MainWindow::setIpAddr(QString ipAddr)
{
    struct sockaddr_in sa;
    const char* ipAddrCStr = ipAddr.toStdString().c_str();

    // Validate IP address being typed in.
    // Red means invalid, green means valid.
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

void MainWindow::setSource()
{
    // Clear the currently plotted graph
    // TODO: Implement the graph clear

    // get the currently selected item of the
    // IP list.
    QString ipAddr;
    ipAddr = ui->listIps->selectedItems().first()->text();

    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(true);

    this->source = ipAddr;
}

void MainWindow::setInterval(int interval)
{
    this->interval = interval;
}

void MainWindow::tcpConnect()
{
    socket->connectToHost(ip,1234);
    if(socket->waitForConnected(3000))
    {
        ui->statusBar->showMessage("Connected!");
        qDebug() << "Connected";

        ui->buttonConnect->setEnabled(false);
        ui->buttonUpdate->setEnabled(true);
        ui->sliderTiming->setEnabled(true);

        getIps();
    }
    else
    {
        qDebug() << "Disconnected";
    }
}

void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();

    ui->buttonConnect->setEnabled(true);
    ui->buttonStart->setEnabled(false);
    ui->buttonStop->setEnabled(false);
    ui->buttonUpdate->setEnabled(false);
    ui->sliderTiming->setEnabled(false);

    ui->listIps->clear();

    statusBar()->showMessage("Disconnected!");
    qDebug() << "Disconnected";
}

void MainWindow::startDataRead()
{
    ui->buttonStart->setEnabled(false);
    ui->buttonStop->setEnabled(true);

    timer->setInterval(interval * 1000);
    timer->setSingleShot(false);
    timer->start();
}

void MainWindow::stopDataRead()
{
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(false);

    timer->stop();
}

void MainWindow::getData(){
    QString str;
    QByteArray array;
    QStringList list;
    qint64 time;
    qDebug() << "getting data...";

    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        if (socket->isOpen())
        {
            // build command
            qDebug() << "reading...";
            QString command = "get " + source + " 1\r\n";
            qDebug() << command;

            // write to socket
            socket->write(command.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();

            qDebug() << socket->bytesAvailable();

            // deserialize information from socket
            while (socket->bytesAvailable())
            {
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    time = str.toLongLong(&ok);
                    str = list.at(1);
                    qDebug() << time << ": " << str;
                }
            }
        }
    }
}

void MainWindow::getIps()
{
    QString str;
    qDebug() << "Getting IP list...";

    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        if (socket->isOpen())
        {
            socket->write("list\r\n");
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();
            while (socket->bytesAvailable())
            {
                str = socket->readLine().replace("\n","").replace("\r","");
                ui->listIps->addItem(str);
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete socket;
    delete timer;
    delete ui;
}
