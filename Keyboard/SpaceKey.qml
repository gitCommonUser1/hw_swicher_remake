import QtQuick 2.0
import QtQuick.Controls 2.12

Button {
    property string name: ""
    id:bt
    width:948
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

    onClicked: {
        KeyboardSignal.sendSpace()
    }
}
