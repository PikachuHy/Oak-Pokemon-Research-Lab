#include "toolbar.h"
#include <QtWidgets>
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
    iconLabel->setPixmap(QPixmap(":/images/logo32.png"));

    layout->addWidget(iconLabel);
    auto settingsBtn = create_btn(":/icon/icons8-settings-50.png");
    auto resetBtn = create_btn(":/icon/icons8-reset-50.png");
    auto articleBtn = create_btn(":/icon/icons8-document-50.png");
    layout->addWidget(settingsBtn);
    layout->addWidget(resetBtn);
    layout->addWidget(articleBtn);

    layout->addStretch(1);
    layout->addWidget(new QLabel(tr("Krabby")));
    layout->addStretch(1);

    connect(settingsBtn, &QPushButton::clicked, this, &Toolbar::openSettings);
    connect(resetBtn, &QPushButton::clicked, this, &Toolbar::reset);
    connect(articleBtn, &QPushButton::clicked, this, &Toolbar::chooseArticle);
}
