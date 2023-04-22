#include "toolbar.h"
#include <QtWidgets>
Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);

    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/images/logo32.png"));

    layout->addWidget(iconLabel);
    auto settingsBtn = new QPushButton();
    settingsBtn->setFixedSize(32, 32);
    settingsBtn->setFlat(true);
    settingsBtn->setCursor(Qt::PointingHandCursor);
    settingsBtn->setIcon(QIcon(":/icon/settings.png"));
    settingsBtn->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(settingsBtn);
    auto resetBtn = new QPushButton();
    resetBtn->setFixedSize(32, 32);
    resetBtn->setFlat(true);
    resetBtn->setCursor(Qt::PointingHandCursor);
    resetBtn->setIcon(QIcon(":/icon/reset.png"));
    resetBtn->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(resetBtn);
    auto articleBtn = new QPushButton();
    articleBtn->setFixedSize(32, 32);
    articleBtn->setFlat(true);
    articleBtn->setCursor(Qt::PointingHandCursor);
    articleBtn->setIcon(QIcon(":/icon/article.png"));
    articleBtn->setFocusPolicy(Qt::NoFocus);
    layout->setSpacing(0);
    layout->addWidget(articleBtn);
    layout->addStretch(1);
    layout->addWidget(new QLabel(tr("Krabby")));
    layout->addStretch(1);

    connect(settingsBtn, &QPushButton::clicked, this, &Toolbar::openSettings);
    connect(resetBtn, &QPushButton::clicked, this, &Toolbar::reset);
    connect(articleBtn, &QPushButton::clicked, this, &Toolbar::chooseArticle);
}
