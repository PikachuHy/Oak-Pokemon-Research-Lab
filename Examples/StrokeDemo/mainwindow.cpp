#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <vector>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
template<typename T>
void drawStroke(QPainter& painter,
                const std::vector<T>& xs,
                const std::vector<T>& ys) {
    int len = xs.size();
    // qDebug() << "len:" << len;
    for (int i=0;i<len -1;i++) {
        // qDebug() << xs[i] << ys[i];
        painter.drawLine(xs[i], ys[i], xs[i+1], ys[i+1]);
    }


}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    auto pen = painter.pen();
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(m_penWidth);
    painter.setPen(pen);
    std::vector<float> xs = {
        185, 194, 208, 241, 289, 329, 351, 371, 385, 396, 397,
    };
    std::vector<float> ys = {
        245, 242, 238, 224, 202, 185, 175, 167, 160, 157, 236,
    };
    // drawStroke(painter, xs, ys);
    if (m_drawStroke) {
        for(const auto& stroke: m_strokes) {
            drawStroke(painter, stroke.m_xs, stroke.m_ys);
        }

    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_recordStroke) {
        auto p = event->pos();
        auto& stroke = m_strokes.back();
        stroke.m_xs.push_back(p.x());
        stroke.m_ys.push_back(p.y());
        // qDebug() << p;
        if (m_drawStroke) update();
    }
}


void MainWindow::on_drawButton_clicked()
{
    m_drawStroke = !m_drawStroke;
    if (m_drawStroke) {
        ui->drawButton->setText("Drawing");
        update();
    } else {
        ui->drawButton->setText("Start Draw");
    }
}


void MainWindow::on_recordButton_clicked()
{
    m_recordStroke = !m_recordStroke;
    if (m_recordStroke) {
        ui->recordButton->setText("Recording");
    } else {
        ui->recordButton->setText("Start Record");
    }
}


void MainWindow::on_penWidthBox_valueChanged(int arg1)
{
    m_penWidth = arg1;
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    m_strokes.push_back(Stroke());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

}

