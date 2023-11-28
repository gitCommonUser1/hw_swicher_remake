import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.12

import MessageDialogControl 1.0


Dialog {

    property var context: messageDialogControl.title
    property var options: messageDialogControl.options
    property var current: messageDialogControl.optionCurrent
    property var type: messageDialogControl.type

    property var textNormalColor: "white"
    property var textSelectedColor: "#0080FF"

    visible:messageDialogControl.messageDialogVisible

    width:480
    height:540
    background:Rectangle{
        color:"transparent"
    }

    contentItem:Rectangle{
        anchors.fill:parent
        color:"transparent"
        Image{
            anchors.fill:parent
            source:"file:./images/keyboard_background.png"
        }

        Rectangle{
            anchors.fill:{
                if(type == MessageDialogControl.MESSAGE_UPDATE){
                    update
                }else{
                    myColumn
                }
            }
            color:"#1a1a1a"
            radius: 4
        }

        UpdateMessage{
            id:update
            visible:type == MessageDialogControl.MESSAGE_UPDATE
        }

        Column{
            visible:type != MessageDialogControl.MESSAGE_UPDATE
            z:1
            width: 228
            anchors.centerIn: parent
            id:myColumn
            Rectangle{
                width:parent.width
                height:name.height + 16 * 2
                color:"transparent"
                Text {
                    id: name
                    text:context
                    font.pixelSize: 16
                    color:"#c4c4c4"
                    anchors.centerIn: parent
                    width:228
                    leftPadding: 20
                    rightPadding: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
/************ process **************/
            Rectangle{
                visible: messageDialogControl.progressBarVisible
                width:parent.width
                height:4
                color:"transparent"
                Rectangle{
                    x:20
                    width:parent.width - 20 * 2
                    height:4
                    radius: 2
                    color: "#C4C4C4"
                }
                Rectangle{
                    x:20
                    width:(parent.width - 20 * 2) * (messageDialogControl.progressPercent / 100)
                    height:4
                    radius: 2
                    color: "#0080FF"
                }
            }

            Rectangle{
                visible: messageDialogControl.progressBarVisible
                width:parent.width
                height:26 * 2
                color:"transparent"
                Text {
                    text:{
                        if(messageDialogControl.progressPercent < 100)
                            messageDialogControl.progressPercent + "%"
                        else
                            qsTr("Done!")
                    }
                    font.pixelSize: 16
                    color:"#c4c4c4"
                    anchors.centerIn: parent
                    width:228
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }

/************ process **************/

            Repeater{
                model:options.length
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
                            text: options[index]
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
    }

}
