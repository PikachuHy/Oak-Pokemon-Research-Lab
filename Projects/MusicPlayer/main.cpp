#include <QFont>
#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

#include "Controller.h"
#include "FileSystem.h"
#include "KeyFilter.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QFont font = app.font();
  font.setFamily("Noto Sans CJK SC");
  font.setPixelSize(16);
  app.setFont(font);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "MyMusic_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  QQmlApplicationEngine engine;
  auto controller = new Controller();
  engine.rootContext()->setContextProperty("$Controller", controller);
  engine.rootContext()->setContextProperty("$FileSystem", new FileSystem());
  auto keyFilter = new KeyFilter();
  engine.rootContext()->setContextProperty("$KeyFilter", keyFilter);
  const QUrl url(u"qrc:/MyMusic/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url, keyFilter](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
          qCritical() << "load error:" << url;
          QCoreApplication::exit(-1);
        }
        // 拦截安卓返回键
        keyFilter->setFilter(obj);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
