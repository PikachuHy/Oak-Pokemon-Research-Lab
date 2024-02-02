import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import "SnowyEffect/export"
import QtQuick.Effects

Item {
    id: musicLyrics
    required property MediaPlayer mediaPlayer
    required property string musicName
    // Lively background created with QQEM.
    SnowyEffect {
        id: backgroundItem
        anchors.fill: parent
        timeRunning: true
    }
    Item {
        id: toolBar
        width: parent.width
        height: 32 + 10


        // Source for the blur effect is lower part of backgroundItem
        ShaderEffectSource {
            id: toolBarEffectSource
            anchors.fill: parent
            sourceItem: backgroundItem
            sourceRect: Qt.rect(0, 0, toolBar.width, toolBar.height + 10)
            visible: false
        }

        // Effect to blur and colorize the toolbar
        MultiEffect {
            anchors.fill: toolBarEffectSource
            source: toolBarEffectSource
            autoPaddingEnabled: false
            blurEnabled: true
            blurMax: 64
            blur: 0.5
            saturation: 0.5
        }
        RowLayout {
            anchors.fill: parent
            Image {
                Layout.leftMargin: 10
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                source: "back-white_64x64.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        $KeyFilter.keyBackPress()
                    }
                }
            }
            Label {
                id: musicNameText
                text: musicLyrics.musicName
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true

                Connections {
                    target: mediaPlayer

                    function onMusicNameChanged() {
                        musicNameText.text = mediaPlayer.musicName
                    }
                }
            }
        }
    }
    Item {
        id: diskImage
        anchors.top: toolBar.bottom
        width: parent.width < parent.height ? parent.width : parent.height
        height: parent.width < parent.height ? parent.width : parent.height

        Image {
            source: "disk_600x600.png"
            anchors.fill: parent
        }
        RotationAnimation on rotation {
            id: diskImageRotation
            loops: Animation.Infinite
            from: 0
            to: 360
            duration: 3600 * 2
        }

        Connections {
            target: mediaPlayer

            function onPlaybackStateChanged() {
                if (mediaPlayer.playbackState == MediaPlayer.PlayingState) {
                    diskImageRotation.resume()
                } else {
                    diskImageRotation.pause()
                }
            }
        }
    }
    Item {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 64 + 20

        // Source for the blur effect is lower part of backgroundItem
        ShaderEffectSource {
            id: effectSource
            anchors.fill: parent
            sourceItem: backgroundItem
            sourceRect: Qt.rect(0, musicLyrics.height - 64 + 20,
                musicLyrics.width, 64 + 20)
            visible: false
        }

        // Effect to blur and colorize the toolbar
        MultiEffect {
            anchors.fill: effectSource
            source: effectSource
            autoPaddingEnabled: false
            blurEnabled: true
            blurMax: 64
            blur: 0.5
            saturation: 0.5
        }
    }

    Item {
        width: parent.width
        height: 64 + 10
        anchors.bottom: parent.bottom
        Column {
            anchors.fill: parent
            Slider {
                id: slider
                width: parent.width
                height: 32
                from: 0
                to: 100
                value: 0
                onMoved: {
                    mediaPlayer.position = value
                }

                Connections {
                    target: mediaPlayer

                    function onPositionChanged(position) {
                        slider.value = position
                        slider.to = mediaPlayer.duration
                    }
                }
            }
            Item {
                width: parent.width
                height: 32
                Item {
                    width: parent.width
                    Item {
                        id: leftItem
                        width: 20
                    }
                    Item {
                        id: rightItem
                        width: 20
                        anchors.right: parent.right
                    }
                    Text {
                        id: currentTime
                        anchors.left: leftItem.right
                        text: "00:00"
                    }
                    Text {
                        anchors.right: rightItem.left
                        id: totalTime
                        text: "00:00"
                    }
                    Connections {
                        id: time
                        target: mediaPlayer

                        function format(time) {

                            time = time / 1000
                            time = Math.floor(time)
                            if (time < 10) {
                                return "00:0" + time
                            }

                            if (time < 60) {
                                return "00:" + time
                            }

                            var m = time / 60
                            m = Math.floor(m)
                            var s = time % 60
                            var ret = m
                            if (m < 10) {
                                ret = "0" + m
                            }
                            ret = ret + ":"
                            if (s < 10) {
                                ret = ret + "0" + s
                            } else {
                                ret = ret + s
                            }
                            return ret
                        }

                        function onPositionChanged(position) {
                            currentTime.text = time.format(position)
                            totalTime.text = time.format(mediaPlayer.duration)
                        }
                    }
                }

                Row {
                    spacing: 30
                    anchors.horizontalCenter: parent.horizontalCenter
                    Image {
                        width: 32
                        height: 32
                        source: "left-circle_64x64.png"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                mediaPlayer.playPrev()
                            }
                        }
                    }
                    Image {
                        id: playOrPauseIcon
                        width: 32
                        height: 32
                        source: "play-circle_64x64.png"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (mediaPlayer.playbackState == MediaPlayer.PlayingState) {
                                    playOrPauseIcon.source = "play-circle_64x64.png"
                                    mediaPlayer.pause()
                                } else {
                                    playOrPauseIcon.source = "pause-circle_64x64.png"
                                    mediaPlayer.play()
                                }
                            }
                        }
                    }
                    Image {
                        width: 32
                        height: 32
                        source: "right-circle_64x64.png"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                mediaPlayer.playNext()
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        slider.value = mediaPlayer.position
        slider.to = mediaPlayer.duration
        if (mediaPlayer.playbackState == MediaPlayer.PlayingState) {
            diskImageRotation.running = true
        } else {
            diskImageRotation.running = false
        }
        if (mediaPlayer.playbackState == MediaPlayer.PlayingState) {
            playOrPauseIcon.source = "pause-circle_64x64.png"
        } else {
            playOrPauseIcon.source = "play-circle_64x64.png"
        }
    }
}
