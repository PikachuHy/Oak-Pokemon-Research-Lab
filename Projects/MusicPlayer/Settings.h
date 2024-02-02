#ifndef SETTINGS_H
#define SETTINGS_H

#include <QRect>
#include <QSettings>

class Settings {
 public:
  static Settings *instance();
  QString configStorePath();
  static const char KEY_MUSIC_DATA_PATH[];
  static const char KEY_MAIN_WINDOW_GEOMETRY[];
  static const char KEY_CURRENT_SONG_INDEX[];

  template <const char *key>
  struct QStringRef {
    operator QString() const {
      // 解决中文乱码问题
      return QString::fromUtf8(settings()->value(key).toByteArray());
    }

    QStringRef &operator=(const QString &newValue) {
      settings()->setValue(key, newValue);
      return *this;
    }
  };

  template <const char *key>
  struct IntRef {
    operator int() const { return settings()->value(key).toInt(); }

    IntRef &operator=(const int &newValue) {
      settings()->setValue(key, newValue);
      return *this;
    }
  };

  template <const char *key>
  struct BoolRef {
    operator bool() const { return settings()->value(key).toBool(); }

    BoolRef &operator=(const bool &newValue) {
      settings()->setValue(key, newValue);
      return *this;
    }
  };

  template <const char *key>
  struct QRectRef {
    operator QRect() const { return settings()->value(key).toRect(); }

    QRectRef &operator=(const QRect &newValue) {
      settings()->setValue(key, newValue);
      return *this;
    }
  };
  template <const char *key>
  struct QStringListRef {
    operator QStringList() const { return settings()->value(key).toStringList(); }

    QStringListRef &operator=(const QStringList &newValue) {
      settings()->setValue(key, newValue);
      return *this;
    }
  };

  QStringRef<KEY_MUSIC_DATA_PATH> musicDataPath;
  QRectRef<KEY_MAIN_WINDOW_GEOMETRY> mainWindowGeometry;
  IntRef<KEY_CURRENT_SONG_INDEX> currentSongIndex;

  // Singleton, to be used by any part of the app
 private:
  static QSettings *settings();

  Settings();

  Settings(const Settings &other);

  Settings &operator=(const Settings &other);
};

#endif  // SETTINGS_H
