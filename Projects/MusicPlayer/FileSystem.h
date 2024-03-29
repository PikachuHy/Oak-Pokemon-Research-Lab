#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QStringList>
class FileSystem : public QObject {
  Q_OBJECT
 public:
  Q_INVOKABLE QStringList listDir(QString path);
  Q_INVOKABLE bool isDir(QString path);
  Q_INVOKABLE bool isFile(QString path);
  Q_INVOKABLE QString defaultPath();
  Q_INVOKABLE bool isReadable(QString path);
  Q_INVOKABLE bool isWritable(QString path);
  Q_INVOKABLE QString fileDir(QString path);
};

#endif  // FILESYSTEM_H
