import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Rectangle{
    property alias text: label.text
    width: 150; height: 30
    color:"#0D3632"
    border.color: "#144D46"

    Label{
        id: label
        anchors.centerIn: parent
        font.family: "微软雅黑"
        font.pixelSize: 14
        color: "#FFFFFF"
    }
    //阴影效果
    layer.enabled: true
    layer.effect: DropShadow{
        transparentBorder: true
        color:"#80000000"
        radius: 15.0
        horizontalOffset: 3
        verticalOffset: 3
        samples: 16
        smooth: true
    }
}
