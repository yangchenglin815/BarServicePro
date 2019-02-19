import QtQuick 2.0
import QtQuick.Controls 2.2

Item{
    property alias text: label.text
    property alias url: image.source
    Rectangle{
         width: 78; height: 78
         id: user
         color:"#0D3632"
         border.color: "#144D46"
         radius: 10
         Image {
             id: image
             anchors.centerIn: parent
             width: 63; height: 64
         }
    }
    Label{
        id:label
        color: "#FFFFFF"
        font.family: "微软雅黑"
        font.pixelSize: 14
        anchors.top: user.bottom
        anchors.horizontalCenter: user.horizontalCenter
        anchors.topMargin: 5
    }
}
