QT += quick

SOURCES += \
KeyFilter.cpp \
        Controller.cpp \
        FileSystem.cpp \
        Settings.cpp \
        main.cpp

resources.files = main.qml MusicLyrics.qml MusicPlayer.qml FileListView.qml \
left-circle_64x64.png \
pause-circle_64x64.png \
play-circle_64x64.png \
right-circle_64x64.png \
settings_64x64.png \
check_64x64.png \
folder_64x64.png \
note_64x64.png \
disk_600x600.jpeg

resources.prefix = /$${TARGET}
RESOURCES += resources

TRANSLATIONS += \
    MyMusic_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
KeyFilter.h \
    Controller.h \
    FileSystem.h \
    Settings.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
