#include "Controller.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "Settings.h"

#ifdef Q_OS_ANDROID
#include <QCoreApplication>
#include <QtCore/private/qandroidextras_p.h>
#include <QOperatingSystemVersion>
#endif

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

int Controller::currentSongIndex() {
  return Settings::instance()->currentSongIndex;
}

void Controller::setCurrentSongIndex(int index) {
  Settings::instance()->currentSongIndex = index;
}

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

bool Controller::requestStoragePermission()
{
#ifdef Q_OS_ANDROID
  // copy from https://ekkesapps.wordpress.com/qt-6-cmake/android-scopedstorage-filedialog/
  if(QOperatingSystemVersion::current() < QOperatingSystemVersion(QOperatingSystemVersion::Android, 11)) {
    qDebug() << "it is less then Android 11 - ALL FILES permission isn't possible!";
    return false;
  }
  // Here you have to set your PackageName
#define PACKAGE_NAME "package:cn.net.pikachu"
  jboolean value = QJniObject::callStaticMethod<jboolean>("android/os/Environment", "isExternalStorageManager");
  if(value == false) {
    qDebug() << "requesting ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION";
    QJniObject ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION = QJniObject::getStaticObjectField( "android/provider/Settings", "ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION","Ljava/lang/String;" );
    QJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION.object());
    QJniObject jniPath = QJniObject::fromString(PACKAGE_NAME);
    QJniObject jniUri = QJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", jniPath.object<jstring>());
    QJniObject jniResult = intent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;", jniUri.object<jobject>() );
    QtAndroidPrivate::startActivity(intent, 0);
    return false;
  } else {
    qDebug() << "SUCCESS ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION";
    return true;
  }
#else
  return true;
#endif
}
