import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle{
    property alias url: image.source
    property alias text: label.text
    property int index: 0
    width: 26
    Image {
        id: image
        width: 26; height: 26
        MouseArea{
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                 console.log("clicked " + parent.parent.index)
            }
        }
    }
    Label{
        id:label
        color: "#FFFFFF"
        font.family: "微软雅黑"
        font.pixelSize: 12
        anchors.top: image.bottom
        anchors.topMargin: 3
        anchors.horizontalCenter: image.horizontalCenter
    }
}
