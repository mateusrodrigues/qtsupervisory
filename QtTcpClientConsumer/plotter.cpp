#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <cmath>
#include "plotter.h"

#define MAX_ITEMS 30.0

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    time = new vector<float>();
    values = new vector<float>();
}

Plotter::~Plotter()
{
    delete time;
    delete values;
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

    // draw the grid
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    painter.setPen(pen);

    int gap = 30;
    for (int i = 0; i < width(); i += gap)
    {
        painter.drawLine(i, 0, i, height());
    }
    for (int i = 0; i < height(); i += gap)
    {
        painter.drawLine(0, i, width(), i);
    }

    // draw a blue solid-lined function
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(0, 0, 255));
    painter.setPen(pen);

    int data_count = this->time->size();

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = height();

    int i = 0;
    double values_max = 0.0;
    if (this->values->size() > 0)
        values_max = *std::max_element(values->begin(), values->end());

    for (vector<float>::iterator iter = values->begin(); iter != values->end(); iter++)
    {
        x1 = x2;
        y1 = y2;

        x2 = (i++ / (float)data_count) * width();
        y2 = height() - ((*iter / values_max) * height());

        painter.drawLine(x1, y1, x2, y2);
    }
}

void Plotter::setPlot(vector<float> time, vector<float> values)
{
    // If sizes are different, return
    if (time.size() != values.size())
        return;
    this->time->clear();
    this->values->clear();

    // Copy vector values to class ones
    for (vector<float>::iterator i = time.begin(); i != time.end(); i++)
        this->time->push_back(*i);
    for (vector<float>::iterator i = values.begin(); i != values.end(); i++)
        this->values->push_back(*i);

    this->repaint();
}

void Plotter::clear()
{
    this->time->clear();
    this->values->clear();

    this->repaint();
}
