import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Todo app on Qt")
    // Material.accent: "gray" для iOS ошибка. Необходимо делать что-то типа ifdef для qml

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.topMargin: 40
        anchors.leftMargin: 20
        anchors.rightMargin: 20


        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            TextField {
                id: inputTask
                placeholderText: "Enter new Task"
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                onAccepted: {
                    if (text.length > 0) {
                        console.log(text)
                    }
                }
            }

            Button {
                text: "Add Task"
                onClicked: {
                    let txt = inputTask.text.trim()
                    if (txt.length > 0) {
                        let taskItem = DB.addTask(txt)
                        // console.log("Task item:", JSON.stringify(taskItem))
                        taskList.model.insert(0, taskItem)
                        inputTask.text = ""
                    }

                    DB.test()
                    console.log("click")
                }
            }
        }

        ListView {
            id: taskList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: taskModel

            ListModel {
                id: taskModel
            }

            delegate: SwipeDelegate {
                width: parent.width
                height: 50

                contentItem: Row {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10

                    Layout.alignment: Qt.AlignHCenter

                    Text {
                        id: taskId
                        text: id
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                    }

                    CheckBox {
                        id: doneCheckBox
                        checked: isDone
                        anchors.verticalCenter: parent.verticalCenter

                        onCheckedChanged: {
                            DB.toggleTask(taskId.text)
                        }
                    }

                    Text {
                        text: title
                        anchors.verticalCenter: parent.verticalCenter
                        font.strikeout: doneCheckBox.checked
                    }

                    /*Button {
                        text: "Test"
                        onClicked: {
                            taskList.updateTasks()
                            console.log(JSON.stringify(modelData))
                        }
                    }*/
                }

                swipe.right: Button {
                    text: qsTr("Delete")
                    anchors.right: parent.right

                    onClicked: {
                        if (DB.delTask(taskId.text))
                            taskModel.remove(index)
                    }
                }
            }

            function updateTasks() {
                taskModel.clear()
                var tasks = DB.getTasks()
                for (var i = 0; i < tasks.length; i++)
                    taskModel.append(tasks[i])
            }

            Component.onCompleted: updateTasks()
        }
    }
}
