import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Window {
    width: 400
    height: 800
    visible: true
    title: qsTr("My Music")

    Connections {
        target: $KeyFilter
        function onKeyBackPress() {
            console.log('back press')
            if (stack.depth === 1) {
                console.log('no page to destroy')
                Qt.quit()
            } else {
                popPageInStack()
            }
        }
    }

    StackView {
        id: stack
        initialItem: mainView
        anchors.fill: parent
    }

    Component {
        id: mainView
        Page {
            header: ToolBar {
                id: toolBar
                width: stack.width
                RowLayout {
                    anchors.fill: parent
                    Label {
                        text: qsTr("My Music")
                        elide: Label.ElideRight
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        Layout.fillWidth: true
                    }
                    Image {
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                        source: "settings_64x64.png"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                var page = Qt.createComponent(
                                            "FileListView.qml").createObject(
                                            stack, {
                                                "width": stack.width,
                                                "height": stack.height,
                                                "path": $FileSystem.defaultPath()
                                            })
                                stack.push(page)
                            }
                        }

                    }
                }
            }

            MusicPlayer {
                width: stack.width
                height: stack.height - toolBar.height
            }
        }
    }

    function popPageInStack() {
        stack.pop()
    }

}
