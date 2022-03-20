#include "FileSystem.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

QStringList FileSystem::listDir(QString path) {
  qDebug() << "path: " << path;
  QFileInfo fileInfo(path);
  if (fileInfo.isDir()) {
    return QDir(path).entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
  }
  return {};
}

bool FileSystem::isDir(QString path) { return QFileInfo(path).isDir(); }

bool FileSystem::isFile(QString path) { return QFileInfo(path).isFile(); }

QString FileSystem::defaultPath() {
#ifdef Q_OS_ANDROID
  return "/storage/emulated/0";
#else
  return QDir::homePath();
#endif
}

bool FileSystem::isReadable(QString path) { return QFileInfo(path).isReadable(); }

bool FileSystem::isWritable(QString path) { return QFileInfo(path).isWritable(); }

QString FileSystem::fileDir(QString path) { return QFileInfo(path).absolutePath(); }
