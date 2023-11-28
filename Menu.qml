import QtQuick 2.12
import QtQuick.Controls 2.12
import RightMenuData 1.0

Item {
    //1440   540
    id:root
//    height:540
    height:{
        if(settings.menuVisible == 0)//settings.MENU_HIDE
            540
        else if(settings.menuVisible == 1)//settings.MENU_SHOW_BIG
            540
        else if(settings.menuVisible == 2)//settings.MENU_SHOW_SMALL
            270
        else
            540
    }
    width:480
    //left
    Rectangle{
        visible: settings.menuVisible
        id:left_rect
        x:1440
        y:540
        clip: true
        width:206
        height:root.height
        color:Qt.rgba(102/255,102/255,102/255,1)
        Image{
            x:31       //13        31
            y:23 // 23+10
//            width:124
//            height:32
//            width:181
//            height:25
            sourceSize: Qt.size(144,20)//144,20
//            antialiasing: true // 反锯齿
            fillMode: Image.PreserveAspectFit // 图片保持纵横比例
            source:"file:./images/stream.png"
        }

        ListView{
            y:63 //73
            width:left_rect.width
            height:root.height - 80//-80
            id:left_listview
            model:leftMenuModel
            focus: true
            clip: true
            delegate: leftMenuDelegate
            onVisibleChanged: {
                left_scrollBar.position = 0
            }
            ScrollBar.vertical: ScrollBar {
                    id:left_scrollBar
                        policy: ScrollBar.AlwaysOn      // 滚动条ce'l策略，这里是永远显示
                        opacity:0
                    }
            PropertyAnimation{
                    id: left_scrollBar_opacity
                    target: left_scrollBar
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 1500
                    easing.type: Easing.InExpo
                }
            currentIndex: settings.leftListViewCurrent
            onCurrentIndexChanged: {
                settings.setLeftListViewCurrent(currentIndex)
                left_scrollBar_opacity.restart()
            }
        }
    }

    //left
    Component {
          id: leftMenuDelegate
          Rectangle {
              property var current_level: level
              property var current_name: name
              id: menu
              width: 210
              height: 46
              Image{
                  source:level == 0?"file:./images/first_menu_logo_icon.png"
                                   :"file:./images/second_menu_logo_icon"

                  height:level == 0?16:8
                  width:level == 0?16:8
                  x:level == 0?17:21
                  anchors.verticalCenter: parent.verticalCenter
              }
              Image{
                  x:180
                  height:12
                  width:12
                  anchors.verticalCenter: parent.verticalCenter
                  source:unfold == 1?"file:./images/first_menu_unfold.png"
                                    :"file:./images/first_menu_normal.png"
                  visible: level == 0?true:false
              }
              color: {
                  if(settings.lastSecondUnfold == -1){
                      ListView.isCurrentItem ? Qt.rgba(0/255,128/255,255/255,1) : left_rect.color   //0,128,255
                  }else{
                      ListView.isCurrentItem ? right_rect.color : left_rect.color
                  }
              }
              radius: 4
              Text {
                  id: menuInfo
                  text: name
                  font.pixelSize: 16
                  color:"white"
                  anchors.verticalCenter: parent.verticalCenter
                  x:46
                  font.bold: level == 0?true:false
              }
          }
      }


    //right
    Rectangle{
        visible: settings.menuVisible
        id:right_rect
        x:1440+left_rect.width
        y:540
        clip: true
        width:480-left_rect.width
        height:root.height
        color:Qt.rgba(64/255,64/255,64/255,1)
        Image{
//            x:285-left_rect.width
            x:25
            y:21//31
            source:"file:./images/rightListview_logo_icon.png"
            width:20
            height:20
            visible: left_listview.currentItem.current_level == 1
        }
        Text {
//            x:314-left_rect.width
            id:title_text
            x:25 + 30
            y:21//30
            text: left_listview.currentItem.current_name.toUpperCase()
            color:"white"
            font.pixelSize: 20
            font.bold: true
            visible: left_listview.currentItem.current_level == 1
        }

        ListView{
            id:right_listview
            visible: left_listview.currentItem.current_level == 1
            y:63//73
            width:right_rect.width
            height:root.height - 70// - 80
            focus: true
            clip: true
            spacing: 16//21
            model:rightMenuModel
            delegate: rightMenuDelegate

            ScrollBar.vertical: ScrollBar {
                    id:right_scrollBar
                        policy: ScrollBar.AlwaysOn      // 滚动条ce'l策略，这里是永远显示
                        opacity:0
                    }
            PropertyAnimation{
                    id: right_scrollBar_opacity
                    target: right_scrollBar
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 1500
                    easing.type: Easing.InExpo
                }

            currentIndex: settings.rightListViewCurrent
            onCurrentIndexChanged: {
                right_scrollBar_opacity.restart()
                settings.setRightListViewCurrent(currentIndex)
            }
        }
    }

    //right
    Component {
          id: rightMenuDelegate
          Rectangle {
              id: menu
              width: 228
              height: {
                  if(type == "fileName_Image" || type == "fileName_Image_Location" || type == "pattern_icons")
                      183//183
                  else if(type == "Color")
                      55 * 2 + right_listview.spacing
                  else
                      55
              }
              color:"transparent"
              property var current_key: key
              property var current_value: value
              property var current_index: index
              property var type: {
                  if(special == RightMenuData.LOAD_PICTURE){
                      "fileName_Image"
                  }else if(special == RightMenuData.LOCATION){
                      "fileName_Image_Location"
                  }else if(special == RightMenuData.PATTERN){
                      "pattern_icons"
                  }else if(special == RightMenuData.COLOR){
                      "Color"
                  }else if(special == RightMenuData.WSTRING){
                        "text"
                  }else{
                      "normal"
                  }
              }
              //标题
              Text {
                  x:25
                  id: name
                  text:key
                  font.pixelSize: 16
                  color:settings.rightListViewCurrent == index?"white":Qt.rgba(196/255,196/255,196/255,1)
                  font.bold: true
              }
              //选择框
              Rectangle{
                  x:25
                  y:23
                  width:228
                  clip:true
                  height:{
                      if(type == "pattern_icons"){
                        160
                      }else{
                          32
                      }
                  }
                  color:{
                        if(type != "pattern_icons"){
                            settings.rightListViewCurrent == index?Qt.rgba(0/255,128/255,255/255,1):Qt.rgba(102/255, 102/255, 102/255,1)
                        }else{
                            if(settings.lastThirdUnfold != index){
                                settings.rightListViewCurrent == index?Qt.rgba(0/255,128/255,255/255,1):Qt.rgba(102/255, 102/255, 102/255,1)
                            }else{
                                Qt.rgba(102/255, 102/255, 102/255,1)
                            }
                        }
                  }
                  radius: 4
                  //文字内容
                  Text {
                      text: {
                          if(type == "text" || type == "fileName_Image")
                                settings.toLocal8bitString(value)
                          else
                              value
//                          if(type == "fileName_Image"){
//                              //特殊情况，读取的QStringList字符无法自动转换，会乱码
//                                settings.toLocal8bitString(value)
//                          }else{
////                                value
//                              value
//                          }
                      }//settings.toLocal8bitString(value)
                      font.pixelSize: 16
                      width:150
                      verticalAlignment: TextInput.AlignVCenter
                      horizontalAlignment: TextInput.AlignHCenter
                      anchors.centerIn: parent
                      color:settings.rightListViewCurrent == index?"white":Qt.rgba(196/255,196/255,196/255,1)
                      elide:Text.ElideMiddle
                      visible: type != "pattern_icons"
                  }
                  //左箭头
                  Image{
                      x:10
                      anchors.verticalCenter: parent.verticalCenter
                      source:!isMin?"file:./images/left_white.png":"file:./images/left_grey.png"
                      visible:{
                          if(type != "pattern_icons"){
                              settings.lastThirdUnfold == index?true:false
                          }else{
                              false
                          }
                      }
                  }
                  //右箭头
                  Image{
                      x:206
                      anchors.verticalCenter: parent.verticalCenter
                      source:!isMax?"file:./images/right_white.png":"file:./images/right_grey.png"
                      visible:{
                          if(type != "pattern_icons"){
                              settings.lastThirdUnfold == index?true:false
                          }else{
                              false
                          }
                      }
                  }

                  //wipe icon
                  Grid{
                      property var pattern_index: {
                          //这个地方写不了枚举，修改一级菜单排列时，这里需要修改
                          if(settings.isTransitionSelect()) //transition
                              settings.transitionWipePatternIndex
                          else if(settings.isKeyPatternSelect())    //key pattern
                              settings.keyPatternWipePatternIndex
                          else
                              -1
                      }
                      visible: type == "pattern_icons"
                      rows:3
                      columns: 6
                      topPadding: 20//15
                      leftPadding: 10
                      rowSpacing: 15
                      columnSpacing:5
                      Image{width: 30; height: 30;source:parent.pattern_index == 0?"file:./images/icon0_active.png":"file:./images/icon0_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 1?"file:./images/icon1_active.png":"file:./images/icon1_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 2?"file:./images/icon2_active.png":"file:./images/icon2_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 3?"file:./images/icon3_active.png":"file:./images/icon3_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 4?"file:./images/icon4_active.png":"file:./images/icon4_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 5?"file:./images/icon5_active.png":"file:./images/icon5_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 6?"file:./images/icon6_active.png":"file:./images/icon6_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 7?"file:./images/icon7_active.png":"file:./images/icon7_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 8?"file:./images/icon8_active.png":"file:./images/icon8_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 9?"file:./images/icon9_active.png":"file:./images/icon9_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 10?"file:./images/icon10_active.png":"file:./images/icon10_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 11?"file:./images/icon11_active.png":"file:./images/icon11_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 12?"file:./images/icon12_active.png":"file:./images/icon12_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 13?"file:./images/icon13_active.png":"file:./images/icon13_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 14?"file:./images/icon14_active.png":"file:./images/icon14_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 15?"file:./images/icon15_active.png":"file:./images/icon15_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 16?"file:./images/icon16_active.png":"file:./images/icon16_normal.png"}
                      Image{width: 30; height: 30;source:parent.pattern_index == 17?"file:./images/icon17_active.png":"file:./images/icon17_normal.png"}
                  }
              }

              //颜色
              Rectangle{
                    x:25
                    y:55 + right_listview.spacing
                    width:228
                    height:55
                    visible:  type == "Color"
                    Text {
                        text:qsTr("Current Color")
                        font.pixelSize: 16
                        color:Qt.rgba(196/255,196/255,196/255,1)
                        font.bold: true
                    }
                    color:"transparent"
                    Rectangle{
                        y:23
                        width:parent.width
                        clip:true
                        height:32
                        property var colorIndex: {
                            //这个地方写不了枚举，修改一级菜单排列时，这里需要修改
                            if(settings.showColorBack1Flag()){
                                0
                            }else if(settings.showColorBack2Flag()){
                               1
                            }else if(settings.showSuperSourceBorder1ColorFlag()){
                                2
                            }else if(settings.showSuperSourceBorder2ColorFlag()){
                                3
                            }else if(settings.showChromaKeyControlColorFlag()){
                                4
                            }else if(settings.showPIPBorderColorFLag()){
                                5
                            }else
                                -1
                        }
                        color:{
                            if(colorIndex === 0){
                                var rgb = hsvToRgb(settings.color1Data[0],settings.color1Data[1],settings.color1Data[2])
                                Qt.rgba(rgb[0],rgb[1],rgb[2],1)
                            }else if(colorIndex === 1){
                                var rgb = hsvToRgb(settings.color2Data[0],settings.color2Data[1],settings.color2Data[2])
                                Qt.rgba(rgb[0],rgb[1],rgb[2],1)
                            }else if(colorIndex === 2){
                                var rgb = hsvToRgb(settings.color3Data[0],settings.color3Data[1],settings.color3Data[2])
                                Qt.rgba(rgb[0],rgb[1],rgb[2],1)
                            }else if(colorIndex === 3){
                                var rgb = hsvToRgb(settings.color4Data[0],settings.color4Data[1],settings.color4Data[2])
                                Qt.rgba(rgb[0],rgb[1],rgb[2],1)
                            }else if(colorIndex === 4){
                                Qt.rgba(settings.color5Data[0] / 255,settings.color5Data[1] / 255,settings.color5Data[2] / 255,1)
                            }else if(colorIndex === 5){
                                var rgb = hsvToRgb(settings.color6Data[0],settings.color6Data[1],settings.color6Data[2])
                                Qt.rgba(rgb[0],rgb[1],rgb[2],1)
                            }

                            else{
                                "transparent"
                            }
                        }
                        radius: 4
                    }
              }
              //still菜单实时刷新大图片
              Image{
                  property var show: (type == "fileName_Image" || type == "fileName_Image_Location")
                  x:25
                  y:65
                  width:228
                  height:128
                  asynchronous:true
                  sourceSize.height: height
                  sourceSize.width: width
                  source : {
                        if(type == "fileName_Image"){
                            if(value != "")
                                "file:///mnt/sdcard/images/" + value
                            else
                                "file:./images/noimage.png"
                        }
                        else if(type == "fileName_Image_Location"){
                            if(settings.stillImages[value] != "")
                                "file:///userdata/stillImages/" + settings.stillImages[value]
                            else
                                "file:./images/noimage.png"
                        }
                        else
                            ""
                  }
                  visible: show
              }
          }
      }

    function hsvToRgb(h,s,v) {
        s = s / 100;
        v = v / 100;
        var r = 0, g = 0, b = 0;
        var i = parseInt((h / 60) % 6);
        var f = h / 60 - i;
        var p = v * (1 - s);
        var q = v * (1 - f * s);
        var t = v * (1 - (1 - f) * s);
        switch (i) {
            case 0:
                r = v; g = t; b = p;
                break;
            case 1:
                r = q; g = v; b = p;
                break;
            case 2:
                r = p; g = v; b = t;
                break;
            case 3:
                r = p; g = q; b = v;
                break;
            case 4:
                r = t; g = p; b = v;
                break;
            case 5:
                r = v; g = p; b = q;
                break;
            default:
                break;
        }
//        r = parseInt(r * 255.0)
//        g = parseInt(g * 255.0)
//        b = parseInt(b * 255.0)
        return [r, g, b];
    }


}
