#ifndef SCORECHARTWIDGET_H
#define SCORECHARTWIDGET_H

#include <QObject>
#include <QWidget>
#include "score.h"
#include <vector>
class ScoreChartWidget: public QWidget
{
public:
    ScoreChartWidget(QWidget* parent = nullptr);
    void init(std::vector<Score> scores);
};

#endif // SCORECHARTWIDGET_H
