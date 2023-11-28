import QtQuick 2.0

Rectangle {
    property string name: ""
    property var index: value
    property var current: value
    width:80
    height:50
    color:current==index?Qt.rgba(0/255,128/255,255/255,1):"white"
//    border.width: 2
    radius: 10
    Text{
        font.pixelSize: 25
        color:"black"
        text:name
        anchors.centerIn: parent
    }
}
