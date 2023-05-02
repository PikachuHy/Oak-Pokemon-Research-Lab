#include "mainwindow.h"
#include "typewidget.h"
#include "choosearticledialog.h"
#include "settingdialog.h"
#include "keyboardwidget.h"
#include "bottom.h"
#include "toolbar.h"
#include <QtWidgets>
#include <chrono>
#include "database.h"
#include <QtCharts>
#include "scorechartwidget.h"
#include "setting.h"
#include <QPropertyAnimation>
using namespace std::chrono_literals;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_database = new Database();
    m_typeWidget = new TypeWidget(m_database);
    m_keyboardWidget = new KeyboardWidget();
    m_chooseArticleDialog = new ChooseArticleDialog();
    auto mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    setAutoFillBackground(true);
    auto toolbar = new Toolbar();
    toolbar->setFixedHeight(60);
    connect(toolbar, &Toolbar::openSettings, this, [this](){
        SettingDialog dialog;
        connect(&dialog, &SettingDialog::showKeyboardChanged, this, &MainWindow::handleShowKeyboardChanged);
        dialog.exec();
    });
    connect(toolbar, &Toolbar::reset, m_typeWidget, &TypeWidget::reset);
    connect(toolbar, &Toolbar::randomKeyboardPracticeRequest, m_typeWidget, &TypeWidget::randomKeyboardPractice);
    connect(toolbar, &Toolbar::chooseArticle, m_typeWidget, &TypeWidget::pause);
    connect(toolbar, &Toolbar::chooseArticle, m_chooseArticleDialog, &ChooseArticleDialog::exec);
    connect(toolbar, &Toolbar::showChart, this, [this](){
        auto scores = m_database->fetch_scores();
        ScoreChartWidget *w = new ScoreChartWidget();
        w->init(scores);
        w->resize(800, 600);
        w->show();
    });

    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(m_typeWidget);
    Setting setting;
    auto hbox = new QHBoxLayout();
    hbox->addStretch(1);
    hbox->addWidget(m_keyboardWidget);
    hbox->addStretch(1);
    if (!setting.showKeyboard()) {
        m_keyboardWidget->hide();
    }
    mainLayout->addLayout(hbox);
    auto bottom = new Bottom(this);
    mainLayout->addWidget(bottom);

    auto w = new QWidget();
    w->setLayout(mainLayout);
    setCentralWidget(w);

    connect(m_typeWidget, &TypeWidget::updateTime, bottom, &Bottom::updateTime);
    connect(m_typeWidget, &TypeWidget::updateAccuracy, bottom, &Bottom::updateAccuracy);
    connect(m_typeWidget, &TypeWidget::updateProgress, bottom, &Bottom::updateProgress);
    connect(m_typeWidget, &TypeWidget::updateSpeed, bottom, &Bottom::updateSpeed);
    connect(m_typeWidget, &TypeWidget::requestHighlightKey, m_keyboardWidget, &KeyboardWidget::highlight);

    connect(m_chooseArticleDialog, &ChooseArticleDialog::chooseArticle, m_typeWidget, &TypeWidget::resetText);

    this->moveToCenter();
}

MainWindow::~MainWindow()
{
    delete m_database;
}

void MainWindow::moveToCenter()
{
    QTimer::singleShot(10ms, this, [this](){
        auto sz = QApplication::primaryScreen()->availableSize();
        auto win_sz = this->size();
        auto x = (sz.width() - win_sz.width()) / 2;
        auto y = (sz.height() - win_sz.height()) / 2;
        this->move(x, y);
    });
}

void MainWindow::handleShowKeyboardChanged()
{
    Setting setting;
    auto start = this->geometry();
    QPropertyAnimation* anim = new QPropertyAnimation(this, "geometry", this);
    anim->setDuration(200);
    auto sz = QApplication::primaryScreen()->availableSize();
    auto w = this->width();
    if (setting.showKeyboard()) {
        m_keyboardWidget->show();
        anim->setStartValue(start);
        auto win_sz = this->size();
        auto x = (sz.width() - win_sz.width()) / 2;
        auto y = (sz.height() - win_sz.height()) / 2;
        auto end = QRect(x, y, w, this->height());
        anim->setEndValue(end);
    }
    else {
        auto h = m_keyboardWidget->height();
        auto new_h = this->height() - h;
        m_keyboardWidget->hide();
        auto x = (sz.width() - w) / 2;
        auto y = (sz.height() - new_h) / 2;
        auto end = QRect(x, y, w, new_h);
        anim->setStartValue(start);
        anim->setEndValue(end);
    }
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_typeWidget->keyReleaseEvent(event);
}
