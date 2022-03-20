#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QtDebug>
#include <QFile>
#include <QLabel>
#include <QIcon>
#include "SystemTrayIcon.h"
#include "FontIconDatabase.h"
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon/icon.png"));
    FontIconDatabase::init();
    QQmlApplicationEngine engine;
    qmlRegisterType<FontIconDatabase>("DB",1,0,"DB");
    qmlRegisterSingletonType( QUrl("qrc:/qml/controller/QmlController.qml"), "controller", 1, 0, "QmlController" );
#ifdef Q_OS_MAC
    QString qmlPath = "../../../../huorong/qml/main.qml";
#else
    QString qmlPath = "../huorong/qml/main.qml";
#endif
    engine.load(QUrl(qmlPath));
    if (engine.rootObjects().isEmpty()){

        qDebug()<<"QQmlApplicationEngine is Empty";
        return -1;
    }
    SystemTrayIcon icon(&engine);
    icon.show();
    return app.exec();
}
