#include "scorechartwidget.h"
#include <QtCharts>
#include <QHBoxLayout>
#include <QWidget>
#include <QDateTime>>
ScoreChartWidget::ScoreChartWidget(QWidget* parent): QWidget(parent) {}

void ScoreChartWidget::init(std::vector<Score> scores)
{
    QSplineSeries *series = new QSplineSeries();
    int max_val = 0;
    for(std::size_t i=0;i<scores.size();i++) {
        max_val = std::max(max_val, scores[i].score);
        auto datetime = QDateTime::fromSecsSinceEpoch(scores[i].create_time);
        series->append(datetime.toMSecsSinceEpoch(), scores[i].score);
    }
    max_val = (max_val + 24) / 25 * 25 + 25;
    QChart *chart = new QChart();
    chart->legend()->hide();

    chart->setTitle("Score Spline Chart");
    chart->addSeries(series);

    auto *axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Date");
    chart->setAxisX(axisX, series);

    auto *axisY = new QValueAxis();
    axisY->setRange(0, max_val);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Score");
    chart->setAxisY(axisY, series);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    auto hbox = new QHBoxLayout();
    hbox->addWidget(chartView);
    setLayout(hbox);
}
