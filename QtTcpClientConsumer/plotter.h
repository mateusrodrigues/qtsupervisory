#ifndef PLOTTER_H
#define PLOTTER_H

#include <vector>
#include <QMap>
#include <QWidget>

using namespace std;

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();
    /**
     * @brief paintEvent paints boilerplate graph drawings
     * @details this is an event that's fired up automatically by the widget
     * everytime it's necessary. It paints grid, background and border to default
     * and sets the pen to color and thickness necessary to plot the graph
     * @param event
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief setPlot sets the points to plot in terms of time and values
     * @param time is the time points array
     * @param values is the value points array
     */
    void setPlot(vector<float> time, vector<float> values);
    /**
     * @brief clear clears up the plotted graph to blank
     */
    void clear();

signals:

public slots:

private:
    vector<float> *time;
    vector<float> *values;
};

#endif // PLOTTER_H
