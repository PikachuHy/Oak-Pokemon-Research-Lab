#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Stroke{
    std::vector<int> m_xs;
    std::vector<int> m_ys;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<Stroke> m_strokes;
    bool m_drawStroke = false;
    bool m_recordStroke = false;
    int m_penWidth = 2;
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;


    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
private slots:
    void on_drawButton_clicked();
    void on_recordButton_clicked();
    void on_penWidthBox_valueChanged(int arg1);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
