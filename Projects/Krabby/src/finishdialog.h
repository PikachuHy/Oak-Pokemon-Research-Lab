#ifndef FINISHDIALOG_H
#define FINISHDIALOG_H

#include <QDialog>
#include "score.h"

namespace Ui {
class FinishDialog;
}

class FinishDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinishDialog(
            int score,
            QString time,
            int words,
            int speed,
            int accuracy,
            QWidget *parent = 0);
    void init_score_chart(std::vector<Score> scores);
    ~FinishDialog();

private:
    Ui::FinishDialog *ui;
};

#endif // FINISHDIALOG_H
