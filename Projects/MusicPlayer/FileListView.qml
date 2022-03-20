import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls

Item {
    id: fileListView
    width: 300
    property string path
    property string type: "FileListView"
    ListView {
        anchors.fill: parent
        visible: true
        model: ListModel {
            id: model2
        }

        delegate: Rectangle {
            width: fileListView.width
            height: 64
            Column {
                Rectangle {
                    width: fileListView.width
                    height: 32
                    property string name: ""
                    property string icon: ""
                    Image {
                        source: model.icon
                        width: 32
                        height: 32
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: model.name
                        anchors.left: parent.left
                        anchors.leftMargin: 40
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Row {
                    spacing: 20
                    Text {
                        text: qsTr("Read: ") + model.readable;
                    }
                    Text {
                        text: qsTr("Write: ") + model.writable;
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    fileListView.visible = false
                    console.log(model.name)
                    console.log(model.icon)
                    console.log(model.path)
                    if ($FileSystem.isDir(model.path)) {
                        console.log('load dir:', model.path)

                        var page = Qt.createComponent(
                                    "FileListView.qml").createObject(
                                    stack, {
                                        "width": stack.width,
                                        "height": stack.height,
                                        "path": model.path
                                    })
                        stack.push(page)
                    } else{
                        console.log('click file')
                    }

                }
            }
        }

    }
    Image {
        width: 32
        height: 32
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 20
        anchors.bottomMargin: 20
        source: "check_64x64.png"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log('change data dir to: ', path)
                $Controller.setMusicDataPath(path)
                dialog.open()
            }
        }
    }

    Dialog {
        id: dialog
        title: qsTr("修改配置")
        standardButtons: Dialog.Ok | Dialog.Cancel
        anchors.centerIn: parent
        Text {
            text: qsTr("重启后配置生效，立马重启？")
        }
        onAccepted: {
            console.log('exit')
            Qt.exit(0)
        }

        onRejected: {
        }
    }

    Component.onCompleted: {
        console.log(path)
        const pathList = $FileSystem.listDir(path)
        pathList.forEach(data => {
                             const fullPath = path + '/' + data
                             const type = $FileSystem.isDir(fullPath) ? "folder" : "file";
                             // console.log(fullPath, $FileSystem.isDir(fullPath))
                             if ($FileSystem.isDir(fullPath)) {
                                 model2.append({
                                                   "name": data,
                                                   "icon": "folder_64x64.png",
                                                   "path": fullPath,
                                                   "type": "folder",
                                                   "writable": $FileSystem.isWritable(fullPath),
                                                   "readable": $FileSystem.isReadable(fullPath)
                                               })
                             } else {
                                 model2.append({
                                                   "name": data,
                                                   "icon": "note_64x64.png",
                                                   "path": fullPath,
                                                   "type": "note",
                                                   "writable": $FileSystem.isWritable(fullPath),
                                                   "readable": $FileSystem.isReadable(fullPath)
                                               })
                             }


                         })
    }
}
