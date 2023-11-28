import QtQuick 2.0
import QtQuick.Controls 2.12

Button {
    property string name: ""
    property string image: ""
    id:bt
    width:268
    height:100
    background:Rectangle{
        color:bt.pressed?"grey":"transparent"
        border.width: 2
        border.color: Qt.rgba(113/255,113/255,113/255,0.3)
    }
    Text{
        font.pixelSize: 42
        color:"white"
        text:name
        anchors.centerIn: parent
    }
    Image{
        anchors.centerIn: parent
        source:image
        width:46
        height:46
    }
}
