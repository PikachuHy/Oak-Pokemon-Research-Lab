#include "bottom.h"
#include "ui_bottom.h"
#include <QFontDatabase>
Bottom::Bottom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bottom)
{
    ui->setupUi(this);
    setFixedHeight(100);

    //auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    //font.setPixelSize(16);
    //font.setBold(false);
    //setFont(font);
    // ui->labelTime->setFont(font);
    // ui->labelSpeed->setFont(font);
    // ui->labelAccuracy->setFont(font);
    // ui->labelProgress->setFont(font);

    // Reset
    updateTime("00:00:00");
    updateSpeed(0);
    updateAccuracy(100);
    updateProgress(0);
}

Bottom::~Bottom()
{
    delete ui;
}

void Bottom::updateTime(QString time)
{
    ui->labelTime->setText(time);
}

void Bottom::updateSpeed(int speed)
{
    ui->labelSpeed->setText(tr("%1 w/m").arg(speed));
}

void Bottom::updateProgress(int progress)
{
    QString path = ":/icon/icons8-0-percent-50.png";
    if (progress >= 100) {
        path = ":/icon/icons8-100-percent-50.png";
    }
    else if (progress >= 90) {
        path = ":/icon/icons8-90-percent-50.png";
    }
    else if (progress >= 80) {
        path = ":/icon/icons8-80-percent-50.png";
    }
    else if (progress >= 70) {
        path = ":/icon/icons8-70-percent-50.png";
    }
    else if (progress >= 50) {
        path = ":/icon/icons8-50-percent-50.png";
    }
    else if (progress >= 30) {
        path = ":/icon/icons8-30-percent-50.png";
    }
    else if (progress >= 20) {
        path = ":/icon/icons8-20-percent-50.png";
    }
    else if (progress >= 10) {
        path = ":/icon/icons8-10-percent-50.png";
    }
    ui->labelProgressIcon->setPixmap(QPixmap(path));
    ui->labelProgress->setText(tr("%1 %").arg(progress));
}

void Bottom::updateAccuracy(int accuracy)
{
    ui->labelAccuracy->setText(tr("%1 %").arg(accuracy));
}
