#include "finishdialog.h"
#include "ui_finishdialog.h"

FinishDialog::FinishDialog(int score, QString time, int words, int speed, int accuracy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinishDialog)
{
    ui->setupUi(this);
    ui->labelScore->setText(tr("%1").arg(score));
    ui->labelTime->setText(time);
    ui->labelWords->setText(tr("%1 w").arg(words));
    ui->labelSpeed->setText(tr("%1 w/m").arg(speed));
    ui->labelAccuracy->setText(tr("%1 %").arg(accuracy));
    QString level = "10";
    if (score < 10) {
        level = "1";
    }
    else if (score < 40) {
        level = "2";
    }
    else if (score < 70) {
        level = "3";
    }
    else if (score < 100) {
        level = "4";
    }
    else if (score < 120) {
        level = "5";
    }
    else if (score < 140) {
        level = "6";
    }
    else if (score < 160) {
        level = "7";
    }
    else if (score < 180) {
        level = "8";
    }
    else if (score < 200) {
        level = "9";
    }
    ui->labelLevel->setText(level);

}

void FinishDialog::init_score_chart(std::vector<Score> scores)
{
    ui->widgetScoreChart->init(scores);
}

FinishDialog::~FinishDialog()
{
    delete ui;
}
