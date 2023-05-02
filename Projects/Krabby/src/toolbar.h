#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

class Toolbar : public QWidget
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = nullptr);

signals:
    void openSettings();
    void reset();
    void randomKeyboardPracticeRequest();
    void chooseArticle();
    void showChart();
public slots:
};

#endif // TOOLBAR_H
