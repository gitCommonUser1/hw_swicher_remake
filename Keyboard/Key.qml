import QtQuick 2.0
import QtQuick.Controls 2.12

Button {
    property string name: ""
    property string image:""
    property var w: 45
    property var h: 45
    property var choose: 0
    property var btIndex: 0

    id:bt
    width:w
    height:h
    background:Rectangle{
        border.width: 2
        border.color: "#D6D1D1"
        radius: 10
//        color:choose == 0?Qt.rgba(0xDC/255,0xD9/255,0xD9/255,0.5):Qt.rgba(0x2B/255,0x7F/255,0xFF/255,0.75)   //
        color:btIndex != settings.keyboardCurrentIndex?Qt.rgba(0xDC/255,0xD9/255,0xD9/255,0.5):Qt.rgba(0x2B/255,0x7F/255,0xFF/255,0.75)   //
    }
    Text{
        id:label
        font.pixelSize: 25
        color:"white"
        text:name
        anchors.centerIn: parent
    }
    Image{
        id:icon
        width:25
        height:25
        anchors.centerIn: parent
        source:image
    }

    onImageChanged: {
        console.log("onImageChanged:" + image)
        icon.source = ""
        icon.source = image
    }

    onClicked: {
        KeyboardSignal.sendKey(name)
    }

}
