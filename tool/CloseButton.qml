import QtQuick 2.0

Rectangle{
    width: 36; height: 36
    color: "transparent"
    Image{
        id: imageClose
        anchors.fill: parent
        source: "/images/close.png"
    }
   MouseArea{
       anchors.fill: parent
       onClicked: {
           Qt.quit()
       }
   }
}
