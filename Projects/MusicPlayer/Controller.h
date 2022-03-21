#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class Controller : public QObject {
  Q_OBJECT
 public:
  explicit Controller(QObject *parent = nullptr);

  Q_INVOKABLE QString musicDataPath();
  Q_INVOKABLE void setMusicDataPath(QString path);

  Q_INVOKABLE bool isDesktop();
  Q_INVOKABLE bool isMobile();
 signals:
};

#endif  // CONTROLLER_H