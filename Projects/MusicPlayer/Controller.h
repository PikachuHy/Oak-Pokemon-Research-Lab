#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class Controller : public QObject {
  Q_OBJECT
 public:
  explicit Controller(QObject *parent = nullptr);

  Q_INVOKABLE QString musicDataPath();
  Q_INVOKABLE void setMusicDataPath(QString path);

  Q_INVOKABLE int currentSongIndex();
  Q_INVOKABLE void setCurrentSongIndex(int index);

  Q_INVOKABLE bool isDesktop();
  Q_INVOKABLE bool isMobile();

  Q_INVOKABLE bool requestStoragePermission();
 signals:
};

#endif  // CONTROLLER_H
