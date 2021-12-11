import QtQuick
import QtQuick.Controls

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TextField {
        placeholderText: qsTr("Enter name")
    }
}
