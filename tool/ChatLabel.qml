import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2
Rectangle {
    property alias text: label.text
    width: 199
    height: 28
    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(198, 0)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#155658" }
            GradientStop { position: 0.5; color: "#80089395" }
            GradientStop { position: 1.0; color: "#155758" }
        }
    }

    Label{
      id:label
      anchors.centerIn: parent
      color: "#EEFFFD"
      font.family: "微软雅黑"; font.pixelSize: 18
    }
}
