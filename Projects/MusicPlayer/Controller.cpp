#include "Controller.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "Settings.h"

Controller::Controller(QObject *parent) : QObject{parent} {}

QString Controller::musicDataPath() {
  QString path = Settings::instance()->musicDataPath;
  if (path.isEmpty()) {
    auto docPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QString docPath;
    if (docPaths.isEmpty()) {
      qCritical() << "no documents path";
      docPath = QDir::homePath();
    } else {
      docPath = docPaths.first();
    }
    auto notesPath = docPath + "/MyMusics/";
    if (!QFile(notesPath).exists()) {
      qDebug() << "mkdir" << notesPath;
      QDir().mkdir(notesPath);
    }
    return notesPath;
  }
  return path;
}

void Controller::setMusicDataPath(QString path) { Settings::instance()->musicDataPath = path; }

bool Controller::isDesktop() {
#ifdef Q_OS_ANDROID
  return false;
#else
  return false;
#endif
}

bool Controller::isMobile() {
#ifdef Q_OS_ANDROID
  return true;
#else
  return true;
#endif
}
