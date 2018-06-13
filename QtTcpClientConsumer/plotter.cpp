#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <cmath>
#include "plotter.h"

#define MAX_ITEMS 30.0

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    data = new QMap<qint64, int>();
}

Plotter::~Plotter()
{
    delete data;
}

void Plotter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush   brush;
    QPen     pen;

    // turn on anti-aliasing
    painter.setRenderHint(QPainter::Antialiasing);

    // set brush to the yellow color by RGB
    brush.setColor(QColor(255, 255, 100));
    brush.setStyle(Qt::SolidPattern);

    // pass on the brush created over to the painter
    painter.setBrush(brush);

    // create a red pen by RGB
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(2);

    // pass on the pen to the painter
    painter.setPen(pen);

    // fill the component with a rectangle
    painter.drawRect(0, 0, this->width(), this->height());

    // draw a blue solid-lined sine function
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(0, 0, 255));
    painter.setPen(pen);

    int dataCount = data->count();

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = height();

    QMap<qint64, int>::iterator iter;

    int i = 0;
    for (iter = data->begin(); iter != data->end(); iter++)
    {
        double max = *std::max_element(data->begin(), data->end());

        x1 = x2;
        y1 = y2;

        x2 = (i++ / MAX_ITEMS) * width();
        y2 = height() - ((iter.value() / max) * height());

        painter.drawLine(x1, y1, x2, y2);
    }
}

void Plotter::addPoint(qint64 timestamp, int value)
{
    data->insert(timestamp, value);
    if (data->count() > MAX_ITEMS)
    {
        qint64 key = data->firstKey();
        data->remove(key);
    }

    this->repaint();
}

void Plotter::clear()
{
    data->clear();
    this->repaint();
}
