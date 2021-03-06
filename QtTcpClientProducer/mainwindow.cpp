#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include <arpa/inet.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    progTimer = new QTimer(this);
    list = new QStringList();
    listModel = new QStringListModel(this);

    // Progress Bar Timer
    connect(progTimer,
            SIGNAL(timeout()),
            this,
            SLOT(updateProgressBar()));
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
    // Start button
    connect(ui->btnStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(startFeed()));
    // Stop button
    connect(ui->btnStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stopFeed()));

    qsrand(time(NULL));
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
        ui->btnConnect->setEnabled(false);
        ui->btnDisconnet->setEnabled(true);
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

    ui->btnConnect->setEnabled(true);
    ui->btnDisconnet->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(false);
    statusBar()->showMessage("Disconnected!");
    qDebug() << "Disconnected";
}

void MainWindow::startFeed()
{
    putData();
    interval = ui->sliderTiming->value();

    // set progress bar timer
    currSecs = 1;
    ui->progressBar->setMaximum(interval);
    progTimer->setInterval(1000);
    progTimer->start();
}

void MainWindow::stopFeed()
{
    progTimer->stop();
    ui->progressBar->setValue(1);
}

void MainWindow::putData()
{
    QDateTime datetime;
    QString str;
    qint64 msecdate;
    int min = ui->sliderMin->value();
    int max = ui->sliderMax->value();

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " + QString::number(min + (qrand() % (max - min + 1)))+"\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        if(socket->waitForBytesWritten(3000))
        {
            qDebug() << "wrote";
        }

        // add latest information to the list
        list->append(str);
        listModel->setStringList(*list);
        // set the list as the liewview model
        ui->listView->setModel(listModel);
        // scroll listview to bottom
        ui->listView->scrollTo(listModel->index(list->size() - 1));
    }
}

void MainWindow::updateProgressBar()
{
    if (interval == currSecs++)
    {
        currSecs = 1;
        putData();
    }
    ui->progressBar->setValue(currSecs);
}

MainWindow::~MainWindow()
{
    delete socket;
    delete progTimer;
    delete list;
    delete listModel;
    delete ui;
}
