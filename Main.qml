import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Todo app on Qt")

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        TextField {
            id: inputTask
            placeholderText: "Enter new Task"
            Layout.fillWidth: true

            onAccepted: {
                if(text.length > 0) {
                    console.log(text)
                }
            }
        }

        Button {
            text: "Add Task"
            Layout.fillWidth: true
            onClicked: {
                console.log("click")
            }
        }

    }

}
