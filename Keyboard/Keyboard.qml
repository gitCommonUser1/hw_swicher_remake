import QtQuick 2.12
import QtQuick.Controls 2.12
import KeyboardEvent 1.0

Item{
    property string letter: "abcdefghijklmnopqrstuvwxyz"
    property var spaceAndCom: ["space",".com"]
    property var bts: []
    property var currentKeyIndex: settings.keyboardCurrentIndex

    property var abcFlag: "123"
    //0:lowercase    1:uppercase   2:#!&    3:123
    property var capslockFlag: "file:./images/capslock.png"
    property var moveFlag: "file:./images/move.png"

    property var ipAddrMaxValue: 256
    property var ipSegmentMax:4
    property var currentIpSegment: 0

    function setIpSegmentValue(value){
        switch(currentIpSegment){
        case 0:
            ip_segment0.name = value
            break
        case 1:
            ip_segment1.name = value
            break
        case 2:
            ip_segment2.name = value
            break
        case 3:
            ip_segment3.name = value
            break
        }
    }

    onCurrentIpSegmentChanged: {
        switch(currentIpSegment){
        case 0:
            settings.setKeyboardCurrentIndex(ip_segment0.name)
            break
        case 1:
            settings.setKeyboardCurrentIndex(ip_segment1.name)
            break
        case 2:
            settings.setKeyboardCurrentIndex(ip_segment2.name)
            break
        case 3:
            settings.setKeyboardCurrentIndex(ip_segment3.name)
            break
        case 4:
            var arr = []
            arr[0] = ip_segment0.name
            arr[1] = ip_segment1.name
            arr[2] = ip_segment2.name
            arr[3] = ip_segment3.name
            settings.setMenuValue(settings.lastFirstUnfold,settings.lastSecondUnfold,settings.lastThirdUnfold,settings.listToIpAddress(arr))//input.text
            settings.setKeyboardVisible(0)
            settings.setLastThirdUnfold(-1)
            break;
        }
    }


    onAbcFlagChanged: {
        if(abcFlag == "123"){
            capslockFlag = "file:./images/capslock.png"
        }else if(abcFlag == "abc"){
            capslockFlag = "file:./images/symbols.png"
        }
    }

    onCapslockFlagChanged: {
        if(capslockFlag == "file:./images/capslock.png"){
            letter = "abcdefghijklmnopqrstuvwxyz"
        }else if(capslockFlag == "file:./images/capslock_active.png"){
            letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        }else if(capslockFlag == "file:./images/symbols.png"){
            letter = "1234567890,./\\<>?*-+=()[];"
        }else if(capslockFlag == "file:./images/numbers.png"){
            letter = "~`!@#$%^&*()_+-=[]{}\\|;:\'\""
        }
    }

    onMoveFlagChanged: {
        if(moveFlag == "file:./images/move_active.png"){
            settings.setKeyboardKetMoveFlag(1)
        }else if(moveFlag == "file:./images/move.png"){
            settings.setKeyboardKetMoveFlag(0)
        }
    }

    visible:settings.keyboardVisible

    onVisibleChanged: {
        if(visible){
            abcFlag = "123"
            capslockFlag = "file:./images/capslock.png"
            moveFlag = "file:./images/move.png"
        }
    }

    //onLoad
    Component.onCompleted: {
//        settings.setKeyboardKeyNumberMax(bts.length)
    }

    //sendKey
    Connections{
        target: models
        onSendKey:{
            if(settings.keyboardType == KeyboardEvent.KEYBOARD_NORMAL){
                keyClick(key)
            }else{
                currentIpSegment++
            }
        }
    }

    //keyboard input text
    Connections{
        target: settings
        onSetKeyboardInputText:{
            input_title.text = title
            if(settings.keyboardType == KeyboardEvent.KEYBOARD_NORMAL){
                settings.setKeyboardKeyNumberMax(bts.length)
                input.text = content
                settings.setKeyboardCurrentIndex(0)
            }else if(settings.keyboardType == KeyboardEvent.KEYBOARD_IPADDR){
                settings.setKeyboardKeyNumberMax(ipAddrMaxValue)
                var segmentList = settings.ipAddressToList(content)
                if(segmentList.length == 0){
                    segmentList = [0,0,0,0]
                }

                ip_segment0.name = segmentList[0]
                ip_segment1.name = segmentList[1]
                ip_segment2.name = segmentList[2]
                ip_segment3.name = segmentList[3]
//                    settings.setKeyboardCurrentIndex(ip_segment0.name)
                currentIpSegment = 0
            }
        }
    }

    Rectangle{
        width:480
        height:540
        color:"transparent"
        visible: keyboardVisible
        Image{
            id:background
            anchors.fill:parent
            source:"file:./images/keyboard_background.png"
        }

        //label
        Rectangle{
            x:0
            y:100
            width:parent.width
            height:30
            color:"transparent"
            Text {
                id:input_title
                text: qsTr("Stream Key")
                font.pixelSize: 25
                color:"white"
                anchors.centerIn: parent
            }
        }

        //key board ip addr
        Row{
            visible: settings.keyboardType == KeyboardEvent.KEYBOARD_IPADDR
            x:0
            y:150
            anchors.horizontalCenter: parent.horizontalCenter
            KeyboardIpAddrInput{
                id:ip_segment0
                current:currentIpSegment
                index:0
            }
            KeyboardIpAddrDot{

            }
            KeyboardIpAddrInput{
                id:ip_segment1
                current:currentIpSegment
                index:1
            }
            KeyboardIpAddrDot{

            }
            KeyboardIpAddrInput{
                id:ip_segment2
                current:currentIpSegment
                index:2
            }
            KeyboardIpAddrDot{

            }
            KeyboardIpAddrInput{
                id:ip_segment3
                current:currentIpSegment
                index:3
            }
            spacing: 10
        }

        Rectangle{
            visible: settings.keyboardType == KeyboardEvent.KEYBOARD_IPADDR
            x:0
            y:parent.height - height
            width:parent.width
            height:304
            color:"black"//#2d2d2d
            ListView {
                id: list
                width:parent.width
                height:150
                y:(parent.height - height) / 2
                highlightRangeMode: ListView.StrictlyEnforceRange
                preferredHighlightBegin: height/3
                preferredHighlightEnd: height/3
                clip: true
                model: ipAddrMaxValue
                currentIndex:currentKeyIndex
                property var lastCurrent: 0
                onCurrentIndexChanged: {
                    if(currentIndex - lastCurrent > 5 || lastCurrent - currentIndex > 5){
                        positionViewAtIndex(currentIndex, ListView.Center)
                    }
                    setIpSegmentValue(currentIndex)
                    lastCurrent = currentIndex
                }
                delegate: Rectangle{
                    width:list.width
                    height:list.height / 3
//                    anchors.horizontalCenter: parent.horizontalCenter
                    color:{
                        if(ListView.isCurrentItem){
                            Qt.rgba(1,1,1,0.25)
                        }else{
                            Qt.rgba(0,0,0,0)
                        }
                    }
                    Text {
                        id: name
                        text: index
                        color:"white"
                        font.pixelSize: 25
                        anchors.centerIn: parent
                    }
                }
            }
            Rectangle {
                width:parent.width
                height:150
                y:(parent.height - height) / 2
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#ff000000" }
                    GradientStop { position: 0.3; color: "#00000000" }
                    GradientStop { position: 0.7; color: "#00000000" }
                    GradientStop { position: 1.0; color: "#ff000000" }
                }
            }
        }


        //input
        Rectangle{
            visible: settings.keyboardType == KeyboardEvent.KEYBOARD_NORMAL
            x:0
            y:150
            width:parent.width
            height:50
            color:"transparent"
            TextField{
                id:input
                anchors.fill:parent
                anchors.leftMargin: 70
                anchors.rightMargin: 70
                focus: true
                onFocusChanged: {
                    focus = true
                }
                background:Rectangle{
                    color:"white"
                    radius: 10
                    border.width: 2
                    border.color:"#707070"
                }
                font.pixelSize: 25
                color:"black"
            }
        }

        //keyboard
        Rectangle{
            visible: settings.keyboardType == KeyboardEvent.KEYBOARD_NORMAL
            id:keyboard
            x:0
            y:parent.height - height
            width:parent.width
            height:304
            color:"black"//2d2d2d
            property var itemSpacing: 8
            property var paddingLeft: 32
            property var keyIndex: 0
            //letters
            Column{
                id:gridLetter
                property var columnMax: 8
                x:keyboard.paddingLeft
                y:19
                spacing: keyboard.itemSpacing
                Repeater{
                    model:{
//                        if(letter.length % gridLetter.columnMax != 0){
//                            letter.length / gridLetter.columnMax + 1
//                        }else{
//                            letter.length / gridLetter.columnMax
//                        }
                        5
                    }
                    Row{
                        property var rowIndex: index
                        spacing: keyboard.itemSpacing
                        Repeater{
                            model:{
//                                if(gridLetter.columnMax * (index + 1) >  letter.length){
//                                    letter.length - gridLetter.columnMax * index + spaceAndCom.length
//                                }else{
//                                    gridLetter.columnMax
//                                }
                                if(index < 3){
                                    gridLetter.columnMax
                                }else if(index == 3){
                                    letter.length - gridLetter.columnMax * index + spaceAndCom.length
                                }else if(index == 4){
                                    5
                                }
                            }
                            Key{
                                id:key
                                property var isLetter: {
                                    if(rowIndex * gridLetter.columnMax + index >= letter.length){
                                        false
                                    }else{
                                        true
                                    }
                                }
                                name:{
                                    if(isLetter){
                                        letter[rowIndex * gridLetter.columnMax + index]
                                    }else{
                                        if(rowIndex == 3){
                                            spaceAndCom[rowIndex * gridLetter.columnMax + index - letter.length]
                                        }else if(rowIndex == 4){
                                            if(index == 1){
                                                abcFlag
                                            }else if(index == 4){
                                                "ok"
                                            }
                                        }
                                    }
                                }

                                image:{
                                    if(!isLetter && rowIndex == 4){
                                        if(index == 0){
                                            //capslock
                                            capslockFlag
                                        }else if(index == 2){
                                            //move
                                            moveFlag
                                        }else if(index == 3){
                                            "file:./images/backspace.png"
                                        }
                                    }
                                }

                                w:{
                                    if(isLetter){
                                        width
                                    }else{
                                        if(rowIndex == 3){
                                            151
                                        }else if(rowIndex == 4){
                                            77
                                        }
                                    }
                                }

                                Component.onCompleted: {
                                    key.btIndex = keyboard.keyIndex++
                                    bts.push(key)
                                }
                            }
                        }
                    }
                }
            }
        }
    }



    function keyClick(key){
        var letterLength = letter.length
        var spaceAndComMin = letterLength
        var spaceAndComMax = letterLength + spaceAndCom.length
        var controlsMin = spaceAndComMax
        var controlsMax = spaceAndComMax + 5
        if(key >= 0 && key < letterLength){
            keyboardEvent.sendKeyEvent(input,letter[key])
        }else if(key >= spaceAndComMin && key < spaceAndComMax){
            if(bts[key].name == spaceAndCom[0]){
                //space
                keyboardEvent.sendKeyEvent(input,"space")
            }else if(bts[key].name == spaceAndCom[1]){
                //.com
                keyboardEvent.sendKeyEvent(input,".com")
            }
        }else if(key >= controlsMin && key < controlsMax){
            if(key == controlsMin){
                //capslock - #!&
                if(capslockFlag == "file:./images/capslock.png"){
                    capslockFlag = "file:./images/capslock_active.png"
                }else if(capslockFlag == "file:./images/capslock_active.png"){
                    capslockFlag = "file:./images/capslock.png"
                }else if(capslockFlag == "file:./images/numbers.png"){
                    capslockFlag = "file:./images/symbols.png"
                }else if(capslockFlag == "file:./images/symbols.png"){
                    capslockFlag = "file:./images/numbers.png"
                }
            }else if(key == controlsMin + 1){
                //abc - 123
                if(abcFlag == "abc"){
                    abcFlag = "123"
                }else{
                    abcFlag = "abc"
                }
            }else if(key == controlsMin + 2){
                //<->
                if(moveFlag == "file:./images/move.png"){
                    moveFlag = "file:./images/move_active.png"
                }else if(moveFlag == "file:./images/move_active.png"){
                    moveFlag = "file:./images/move.png"
                }
            }else if(key == controlsMin + 3){
                //backspace
                keyboardEvent.sendKeyEvent(input,"backspace")
            }else if(key == controlsMin + 4){
                //ok
                if(regExp.exactMatch(settings.keyboardReg,input.text)){
//                    settings.setMenuValue(settings.lastFirstUnfold,settings.lastSecondUnfold,settings.rightListViewCurrent,input.text)//input.text
                    settings.keyboardOk(settings.lastFirstUnfold,settings.lastSecondUnfold,settings.rightListViewCurrent,input.text)
                }else{
                    messageDialogControl.dialogShow(qsTr("You entered the wrong format!!"),[qsTr("Return")])
                }
                settings.setKeyboardVisible(0)
                settings.setLastThirdUnfold(-1)
                settings.setKeyboardReg(-1)
            }
        }else if(key < 0){
            if(key == -2){
                keyboardEvent.sendKeyEvent(input,"left")
            }else if(key == -1){
                keyboardEvent.sendKeyEvent(input,"right")
            }
        }
    }
}



