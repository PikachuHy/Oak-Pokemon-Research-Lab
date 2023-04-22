#include "mainwindow.h"
#include "typewidget.h"
#include "choosearticledialog.h"
#include "settingdialog.h"
#include "keyboardwidget.h"
#include "bottom.h"
#include "toolbar.h"
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_typeWidget = new TypeWidget();
    m_keyboardWidget = new KeyboardWidget();
    m_chooseArticleDialog = new ChooseArticleDialog();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    auto toolbar = new Toolbar();
    toolbar->setFixedHeight(50);
    connect(toolbar, &Toolbar::openSettings, this, [this](){
        SettingDialog dialog;
        dialog.exec();
    });
    connect(toolbar, &Toolbar::reset, m_typeWidget, &TypeWidget::reset);
    connect(toolbar, &Toolbar::chooseArticle, m_typeWidget, &TypeWidget::pause);
    connect(toolbar, &Toolbar::chooseArticle, m_chooseArticleDialog, &ChooseArticleDialog::exec);
 
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
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_typeWidget->keyReleaseEvent(event);
}
