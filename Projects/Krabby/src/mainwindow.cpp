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
using namespace std::chrono_literals;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_database = new Database();
    m_typeWidget = new TypeWidget(m_database);
    m_keyboardWidget = new KeyboardWidget();
    m_chooseArticleDialog = new ChooseArticleDialog();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    setAutoFillBackground(true);
    auto toolbar = new Toolbar();
    toolbar->setFixedHeight(60);
    connect(toolbar, &Toolbar::openSettings, this, [this](){
        SettingDialog dialog;
        dialog.exec();
    });
    connect(toolbar, &Toolbar::reset, m_typeWidget, &TypeWidget::reset);
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
    auto hbox = new QHBoxLayout();
    hbox->addStretch(1);
    hbox->addWidget(m_keyboardWidget);
    hbox->addStretch(1);
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

    QTimer::singleShot(100ms, this, [this](){
        auto sz = QApplication::primaryScreen()->availableSize();
        auto win_sz = this->size();
        auto x = (sz.width() - win_sz.width()) / 2;
        auto y = (sz.height() - win_sz.height()) / 2;
        this->move(x, y);
    });
}

MainWindow::~MainWindow()
{
    delete m_database;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_typeWidget->keyReleaseEvent(event);
}
