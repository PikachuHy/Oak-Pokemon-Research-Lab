import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "SnowyEffect/export"
import QtQuick.Effects

Page {
    id: musicPlayerRoot
    property bool playing: false
    property int currentIndex: 0
    width: 400
    height: 800
    title: "Music Player"
    onCurrentIndexChanged: () => {
                               $Controller.setCurrentSongIndex(
                                   musicPlayerRoot.currentIndex)
                           }

    MediaPlayer {
        id: mediaPlayer
        audioOutput: AudioOutput {}
        onPositionChanged: position => {
                               mobileProcess.setProcess(
                                   position / mediaPlayer.duration)
                           }
        onPlaybackStateChanged: {
            if (playbackState === MediaPlayer.StoppedState
                    && musicPlayerRoot.playing) {
                musicPlayerRoot.playNext()
            }
        }
        property string musicName
        function playNext() {
            musicPlayerRoot.playNext()
        }
        function playPrev() {
            musicPlayerRoot.playPrev()
        }
    }
    footer: ColumnLayout {
        width: parent.width
        height: parent.height
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ListModel {
                id: listModel
            }
            delegate: Rectangle {
                id: item
                width: musicPlayerRoot.width
                height: 32

                Text {
                    text: model.name
                    leftPadding: 16
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        musicPlayerRoot.play(model.path, model.name)
                        musicPlayerRoot.currentIndex = index
                    }

                    onEntered: {
                        if ($Controller.isDesktop()) {
                            item.color = "#eee"
                        }
                    }
                    onExited: {
                        if ($Controller.isDesktop()) {
                            item.color = "transparent"
                        }
                    }
                }
            }
        }
        Rectangle {
            visible: true
            Layout.fillWidth: true
            Layout.preferredHeight: 64
            Component.onCompleted: {

            }
            // 这个进度条在安卓上无法显示，奇怪
            Rectangle {
                width: parent.width
                height: parent.height
                anchors.bottom: parent.bottom
                Rectangle {
                    id: mobileProcess
                    width: 0
                    height: parent.height
                    color: '#eee'
                    function setProcess(val) {
                        mobileProcess.width = val * parent.width
                    }
                }
            }
            Row {
                width: parent.width - 40
                height: parent.height
                leftPadding: 20
                Text {
                    width: parent.width - mobilePlayStatusIcon.width
                    anchors.verticalCenter: parent.verticalCenter
                    id: mobileMusicName
                    text: ""
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var page = Qt.createComponent(
                                        "MusicLyrics.qml").createObject(
                                        stack, {
                                            "width": stack.width,
                                            "height": stack.height,
                                            "mediaPlayer": mediaPlayer,
                                            "musicName": mobileMusicName.text
                                        })
                            stack.push(page)
                        }
                    }
                }

                Image {
                    id: mobilePlayStatusIcon
                    width: 32
                    height: 32
                    anchors.verticalCenter: parent.verticalCenter
                    source: "play-circle_64x64.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            musicPlayerRoot.playing = !musicPlayerRoot.playing
                        }
                    }
                }
            }
        }
    }

    onPlayingChanged: {
        if (playing) {
            mediaPlayer.play()
            mobilePlayStatusIcon.source = "pause-circle_64x64.png"
        } else {
            mediaPlayer.pause()
            mobilePlayStatusIcon.source = "play-circle_64x64.png"
        }
    }
    function playNext() {
        var newIndex = (musicPlayerRoot.currentIndex + 1) % listModel.count
        var data = listModel.get(newIndex)
        musicPlayerRoot.play(data.path, data.name)
        musicPlayerRoot.currentIndex = newIndex
    }
    function playPrev() {
        var newIndex = (musicPlayerRoot.currentIndex - 1 + listModel.count) % listModel.count
        var data = listModel.get(newIndex)
        musicPlayerRoot.play(data.path, data.name)
        musicPlayerRoot.currentIndex = newIndex
    }

    function play(path, name) {
        if (path.length > 0) {
            console.log('play ', path)
            mediaPlayer.source = "file://" + path
            mobileMusicName.text = name
            mediaPlayer.musicName = name
        }
        musicPlayerRoot.playing = true
        mediaPlayer.play()
    }
    function pause() {
        musicPlayerRoot.playing = false
        mediaPlayer.pause()
    }
    function fetchAllMusic(path) {
        var musics = $FileSystem.listDir(path)
        for (var i = 0; i < musics.length; i++) {
            if ($FileSystem.isDir(path + "/" + musics[i])) {
                fetchAllMusic(path + "/" + musics[i])
            } else {

                listModel.append({
                                     "name": musics[i],
                                     "path": path + "/" + musics[i]
                                 })
            }
        }
    }

    Component.onCompleted: {
        var basePath = $Controller.musicDataPath()
        console.log("music data path:", basePath)
        fetchAllMusic(basePath)
        console.log("list count", listView.count)
        let song_index = $Controller.currentSongIndex()
        if (listView.count > song_index) {
            console.log("song_index", song_index)
            let default_song = listModel.get(song_index)
            musicPlayerRoot.play(default_song.path, default_song.name)
            musicPlayerRoot.currentIndex = 0
            musicPlayerRoot.pause()
        }
    }
}
