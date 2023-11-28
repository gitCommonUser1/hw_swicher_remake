import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import "./Keyboard/"

Window {
    id:root
    width: 1920
    height: 1080
    title: qsTr("Hello World")
    color:"transparent"

    visible: true

    property var still_show: 0
    property var still_name: ""
    onStill_nameChanged: {
        still_image.source = ""
        still_image.source = "image://still/"+still_name
    }

    Connections{
        target:models
        onLoadStillImage:{
            still_show = flag
            still_name = name
        }
    }

    Rectangle{
        anchors.fill:parent
        color:"black"
        visible: still_show
        z:3
    }

    Image{
        id:still_image
        anchors.fill:parent
        visible: still_show//still_show
        fillMode:Image.Pad//Image.PreserveAspectFit
//        horizontalAlignment: Image.AlignLeft
//        verticalAlignment: Image.AlignTop
        z:4
        onProgressChanged: {
            if(progress == 1){
                switch(settings.loadStillProgress){
                case 2:
                    models.loadStill2();
                    break;
                case 3:
                    models.loadStill3();
                    break;
                case 4:
                    models.loadStill4();
                    break;
                }
            }
        }
    }

//    菜单
    Menu{

    }

    //音频表绘制
    AudioTable{
        x:1440
        y:800
    }

    Keyboard{
        x:1440
        y:540
        z:5
    }

    MessageDialog{
        x:1440
        y:540
    }

    PlaybackList{
        x:1440
        y:540
    }

    NdiList{
        x:1440
        y:540
    }
}
