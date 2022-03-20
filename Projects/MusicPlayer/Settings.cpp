#include "Settings.h"

#include <QStandardPaths>

const char Settings::KEY_MUSIC_DATA_PATH[] = "path/music_data";
const char Settings::KEY_MAIN_WINDOW_GEOMETRY[] = "main_window/geometry";
Settings *Settings::instance() {
  static Settings singleton;
  return &singleton;
}

QString Settings::configStorePath() { return settings()->fileName(); }

Settings::Settings() {}

QSettings *Settings::settings() {
  static QSettings ret(
      QString("%1/config.ini").arg(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first()),
      QSettings::IniFormat);
  return &ret;
}
