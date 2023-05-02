#include "scorechartwidget.h"
#include <QtCharts>
#include <QHBoxLayout>
#include <QWidget>
#include <QDateTime>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTabWidget>

ScoreChartWidget::ScoreChartWidget(QWidget* parent): QWidget(parent) {}

void ScoreChartWidget::init(std::vector<Score> scores)
{
    auto tabWidget = new QTabWidget();
    QTableWidget* tableWidget = new QTableWidget();
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setRowCount(scores.size());
    tableWidget->setColumnCount(3);
    QStringList headers;
    headers << "ID";
    headers << "SCORE" << "DATE";
    tableWidget->setHorizontalHeaderLabels(headers);

    QSplineSeries *series = new QSplineSeries();
    int max_val = 0;
    for(std::size_t i=0;i<scores.size();i++) {
        max_val = std::max(max_val, scores[i].score);
        auto datetime = QDateTime::fromSecsSinceEpoch(scores[i].create_time);
        // series->append(datetime.toMSecsSinceEpoch(), scores[i].score);
        series->append(i, scores[i].score);
        int col = 0;
        {
            auto item = new QTableWidgetItem(QString::number(scores[i].id));
            tableWidget->setItem(i, col++, item);
        }
        {
            auto item = new QTableWidgetItem(QString::number(scores[i].score));
            tableWidget->setItem(i, col++, item);
        }
        {
            auto datetime = QDateTime::fromSecsSinceEpoch(scores[i].create_time);
            auto item = new QTableWidgetItem(datetime.toString("yyyy-MM-dd"));
            tableWidget->setItem(i, col++, item);
        }
    }
    max_val = (max_val + 24) / 25 * 25 + 25;
    QChart *chart = new QChart();
    chart->legend()->hide();

    chart->setTitle("Score Spline Chart");
    chart->addSeries(series);

    // auto *axisX = new QDateTimeAxis();
    // axisX->setFormat("yyyy-MM-dd");
    // axisX->setTitleText("Date");
    auto *axisX = new QValueAxis();
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Count");
    chart->setAxisX(axisX, series);

    auto *axisY = new QValueAxis();
    axisY->setRange(0, max_val);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Score");
    chart->setAxisY(axisY, series);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    tabWidget->addTab(chartView, QIcon(":/icon/icons8-line-chart-50.png"), "Chart");
    tabWidget->addTab(tableWidget, QIcon(":/icon/icons8-spreadsheet-50.png"), "Table");
    auto hbox = new QHBoxLayout();
    hbox->addWidget(tabWidget);
    setLayout(hbox);
}
