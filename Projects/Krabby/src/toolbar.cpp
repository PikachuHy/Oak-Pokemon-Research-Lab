#include "toolbar.h"
#include <QtWidgets>
#include <QFontDatabase>
auto create_btn(QString path) {
    auto btn = new QPushButton();
    btn->setFixedSize(50, 50);
    btn->setIconSize(QSize(25, 25));
    btn->setFlat(true);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setIcon(QIcon(path));
    btn->setFocusPolicy(Qt::NoFocus);
    return btn;
}
Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);

    QLabel *iconLabel = new QLabel();
    iconLabel->setFixedSize(28, 28);
    iconLabel->setScaledContents(true);
    iconLabel->setPixmap(QPixmap(":/images/logo.png"));

    layout->addWidget(iconLabel);
    auto settingsBtn = create_btn(":/icon/icons8-settings-50.png");
    auto resetBtn = create_btn(":/icon/icons8-reset-50.png");
    auto keyboardBtn = create_btn(":/icon/icons8-keyboard-50.png");
    auto articleBtn = create_btn(":/icon/icons8-document-50.png");
    auto chartBtn = create_btn(":/icon/icons8-combo-chart-50.png");
    layout->addWidget(settingsBtn);
    layout->addWidget(resetBtn);
    layout->addWidget(keyboardBtn);
    layout->addWidget(articleBtn);
    layout->addWidget(chartBtn);
#ifndef Q_OS_MAC
    layout->addStretch(1);
    auto title = new QLabel(tr("Krabby"));
    auto font = QFontDatabase::systemFont(QFontDatabase::TitleFont);
    font.setPixelSize(26);
    title->setFont(font);
    layout->addWidget(title);
#endif
    layout->addStretch(1);

    connect(settingsBtn, &QPushButton::clicked, this, &Toolbar::openSettings);
    connect(resetBtn, &QPushButton::clicked, this, &Toolbar::reset);
    connect(keyboardBtn, &QPushButton::clicked, this, &Toolbar::randomKeyboardPracticeRequest);
    connect(articleBtn, &QPushButton::clicked, this, &Toolbar::chooseArticle);
    connect(chartBtn, &QPushButton::clicked, this, &Toolbar::showChart);
}
