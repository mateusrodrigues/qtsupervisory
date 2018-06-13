#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QWidget>

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();
    void paintEvent(QPaintEvent *event);

    void addPoint(qint64 timestamp, int value);
    void clear();

signals:

public slots:

private:
    QMap<qint64, int> *data;
};

#endif // PLOTTER_H
