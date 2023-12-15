import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.12


Dialog {

    property var imagePath: "/mnt/sdcard/videos/"

    property var list: settings.playList
    property var current: settings.playListDialogCurrent
    property var currentFileColor: Qt.rgba(0/255,128/255,255/255,1)
    property var currentFileButRecordingColor: "red"
    property var fileColor: "white"
    property var fileFontSize: 16
    property var titleFontSize: 18
    property var titleIcon: "file:./images/titleIcon.png"
    property var currentFileIcon: "file:./images/currentFileIcon.png"
    property var currentFileButRecordingIcon: "file:./images/currentFileButRecordingIcon.png"
    property var fileIcon: "file:./images/fileIcon.png"
    property var nextPageIconDisabled: "file:./images/nextPageIconDisabled.png"
    property var nextPageIconEnable: "file:./images/nextPageIconEnable.png"
    property var previousPageIconDisable: "file:./images/previousPageIconDisable.png"
    property var previousPageIconEnable: "file:./images/previousPageIconEnable.png"

    property var rowSpace: 16

    onListChanged: {
        var lth = list.length
        playbackListModel.clear()
        for(var i = 0;i < lth;++i){
            playbackListModel.append({name:list[i]})
        }
    }

    visible:settings.playListDialogVisible

    onVisibleChanged: {
        if(visible){
            settings.setPlayListDialogCurrent(0)
            settings.setPlayListDialogCurrent(settings.playListCurrent)
        }
    }

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
            x:40
            y:71
            width:400
            height:395
            color:"#1a1a1a"
            radius: 4

            //title
            Image{
                x:17
                y:35
                source:titleIcon
            }
            Text {
                id: title
                x:42
                y:31
                text: qsTr("Video Select")
                font.pixelSize: titleFontSize
                color:fileColor
            }

            //list
            ListView{
                x:17
                y:80
                width:parent.width - x
                height:264 + 24
                id:playbackListview
                model:playbackListModel
                focus: true
                clip: true
                delegate: playbackListDelegate
                currentIndex: current
                spacing: rowSpace

                onVisibleChanged: {
                    list_scrollBar.position = 0
                }
                ScrollBar.vertical: ScrollBar {
                        id:list_scrollBar
                            policy: ScrollBar.AlwaysOn      // 滚动条ce'l策略，这里是永远显示
                            opacity:0
                        }
                PropertyAnimation{
                        id: list_scrollBar_opacity
                        target: list_scrollBar
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: 1500
                        easing.type: Easing.InExpo
                    }
                onCurrentIndexChanged: {
                    list_scrollBar_opacity.restart()
                }
            }

            //model
            ListModel {
                id:playbackListModel
             }

            //delegate
            Component {
                  id: playbackListDelegate
                  Rectangle {
                      color:"transparent"
                      id: menu
                      width: parent.width - x
                      height: 22
                      Image{
                          y:6
                          source:{
                              if(index == playbackListview.currentIndex){
                                      currentFileIcon
                              }else{
                                  fileIcon
                              }
                          }
                      }
                      Text {
                          x:25
                          text: settings.toLocal8bitString(name)
                          color:{
                              if(index == playbackListview.currentIndex){
                                    currentFileColor
                              }else{
                                  fileColor
                              }
                          }
                          font.pixelSize: fileFontSize
                          elide:Text.ElideMiddle
                          width:350
                      }
                  }
              }

            //bottom
            Row{
                visible: false
                y:364
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                Image{
                    y:4
                    source:previousPageIconDisable
                }
                Text {
                    color:fileColor
                    font.pixelSize: fileFontSize
                    text: currentPage + "/" + maxPage
                }
                Image{
                    y:4
                    source:nextPageIconDisabled
                }
            }
        }
    }
}
