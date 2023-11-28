import QtQuick 2.12


Column{
    z:1
    width: 404
    anchors.centerIn: parent
    id:myColumn
    Rectangle{
        width:parent.width
        height:366
        color:"transparent"

        Image{
            x:40
            y:36
            source:"file:./images/update1.png"
        }
        Text{
            text:qsTr("NDI License is \navailable on Osee \nOfficial Online \nStore.")
            font.pixelSize: 18
            color:"white"
            x:200
            y:55
        }

        Image{
            x:40
            y:204
            source:"file:./images/update2.png"
        }
        Text{
            text:qsTr("Please upgrade to \nour latest firmware \nfor a better \nperformance.")
            font.pixelSize: 18
            color:"white"
            x:200
            y:221
        }
    }

    Repeater{
        model:1
        Rectangle{
            width:parent.width
            height:32+1
            color:"transparent"
            Rectangle{
                color:"#c4c4c4"
                width:parent.width
                height:1
            }
            Rectangle{
                y:1
                height:32
                width:parent.width
                color:"transparent"
                Text {
                    id: cancel
                    text: qsTr("Press menu button for 2s to close this window")
                    font.pixelSize: 16
                    color:{
                        current == index?"#0080FF":"#c4c4c4"
                    }
                    anchors.centerIn: parent
                }
            }
        }
    }
}
