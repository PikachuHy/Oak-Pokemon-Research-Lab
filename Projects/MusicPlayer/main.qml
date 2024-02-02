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

    Dialog {
        id: dialog
        title: qsTr("申请存储权限")
        standardButtons: Dialog.Ok | Dialog.Cancel
        anchors.centerIn: parent
        Text {
            text: qsTr("即将跳转权限申请页面...\n如果权限申请成功，重启后权限生效，立马重启？")
        }
        onAccepted: {
            console.log('exit')
            Qt.exit(0)
        }

        onRejected: {
        }
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
                                if ($Controller.requestStoragePermission()) {
                                    var page = Qt.createComponent(
                                                "FileListView.qml").createObject(
                                                stack, {
                                                    "width": stack.width,
                                                    "height": stack.height,
                                                    "path": $FileSystem.defaultPath()
                                                })
                                    stack.push(page)
                                }
                                else {
                                    dialog.visible = true
                                }
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
