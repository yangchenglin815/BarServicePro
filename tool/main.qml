import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 351
    height: 860
    x: Screen.desktopAvailableWidth - width
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool
    color: "transparent"
    Image{
        id: imageHome
        anchors.fill: parent
        source: "images/desktop.png"
    }
    CloseButton{
        id: closeButton; x: 295; y: 19
    }
    Text{
        id:pcNo
        text: "机号108"
        color: "#FEFEFE"
        font.family: "微软雅黑"
        font.pixelSize: 22
        y: 32
        anchors.horizontalCenter: parent.horizontalCenter
    }
    CommonButton{
        id: showdownButton
        url: "images/shutdown.png"
        text: "关机"
        x:265; y:103
        index: 0
    }
    CommonButton{
        id: rebootButton
        url: "images/reboot.png"
        text: "重启"
        x:214; y:103
        index: 1
    }
    CommonButton{
        id: complaintsButton
        url: "images/complaints.png"
        text: "投诉"
        x:162; y:103
        index: 2
    }
    CommonLineEdit{
        text:"钱包： 1000"
        x:151; y:159
    }
    CommonLineEdit{
        text:"网费： 1000"
        x:151; y:194
    }
    CommonLabel{
        id:userLabel
        url:"images/avatar.png"
        text: "隔壁老王"
        x:50; y:110
    }
    FuncButton{
        id:order
        url:"images/order.png"
        x:12; y:316
    }
    Label{
        id:orderLabel
        text: "在线点餐"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 363; x:34
    }
    FuncButton{
        id:tel
        url:"images/tel.png"
        x:12; y:405
    }
    Label{
        id:telLabel
        text: "呼叫服务"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 453; x:34
    }
    FuncButton{
        id:recharge
        url:"images/recharge.png"
        x:90; y:360
    }
    Label{
        id:rechargeLabel
        text: "网费充值"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 407; x:112
    }
    FuncButton{
        id:wifi
        url:"images/wifi.png"
        x:90; y:449
    }
    Label{
        id:wifiLabel
        text: "wifi连接"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 497; x:112
    }
    FuncButton{
        id:attendance
        url:"images/attendance.png"
        x:169; y:319
    }
    Label{
        id:attendanceLabel
        text: "每日签到"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 367; x:190
    }
    FuncButton{
        id:game
        url:"images/game.png"
        x:168; y:408
    }
    Label{
        id:gameLabe
        text: "游戏陪玩"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 457; x:189
    }
    FuncButton{
        id:activity
        url:"images/activity.png"
        x:247; y:366
    }
    Label{
        id:activityLabe
        text: "活动中心"
        color:"#8CFFFC"
        font.family: "微软雅黑"; font.pixelSize: 12
        y: 413; x:268
    }
    ChatLabel{
        id:chat
        y:572
        anchors.horizontalCenter: parent.horizontalCenter
        text: "网吧聊天"
    }
    Rectangle{
        id:lineEdit
        width: 205; height: 39
        x: 27; y: 738
        color: "#0D3632"
        border.color: "#144D46"
        border.width: 1

        Label{
            text: "输入聊天内容"
            color: "#22B4B0"
            font.family: "微软雅黑"
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
        }
    }
    Button{
        id:sendButton
        width: 80; height: 39
        x: 242; y: 737
       background: Rectangle{
           anchors.fill: parent
           color: "#0D3632"
           border.color: "#144D46"
           border.width: 1
           Text {
               anchors.centerIn: parent
               text: qsTr("发送")
               color: "#FEFFFF"
               font.family: "微软雅黑"
               font.pixelSize: 18
           }
       }
    }
    Flickable{
        id:flickable
        width: parent.width - 24
        height: 126
        x: 12; y: chat.y + chat.height + 6
        TextArea.flickable:  TextArea{
            id: taMessage
            wrapMode: TextArea.Wrap
            background: Rectangle{
                color: "transparent"
            }
            readOnly: true
            selectByMouse: true
        }
        ScrollBar.vertical: ScrollBar{}
    }
}
