import QtQuick 2.0

//w:474   h:264

Item {
    property var isTest: testQml.value

    visible:{
        if(isTest)
            true
        else
            settings.audioVisible
    }
    property var audioNumberFontSize: 13
    property var audioTitleFontSize: 13
    property var audioScaleFontSize: 13

    Rectangle{
        width:474
        height:264
        color:"transparent"

        Rectangle{
            visible: {
                if(isTest)
                    false
                else
                    true
            }

            width:parent.width
            height:74 //74  81
            color:"#29282e"
            y:5

            //Stream

            Rectangle{
                id:stream1_rect
                x:42
                y:15
                width:63
                height:20
//                color:"#626262"
                color:{
                    if(settings.liveStreamStatus1 == 0){
                        //success!
                        "red"
                    }else if(settings.liveStreamStatus1 == 3){
                        // lost packet
                        "yellow"
                    }else{
                        //fail or quit
                        "#626262"
                    }
                }

                radius: 3
                Text {
                    text: qsTr("STREAM 1")
                    font.pixelSize: 11
                    color:{
                        if(settings.liveStreamStatus1 == 3){
                            "black"
                        }else{
                            "white"
                        }
                    }
                    anchors.centerIn: parent
                }
            }
            Image{
                x:stream1_rect.x + 76//118
                y:stream1_rect.y
                width:20
                height:20
                source:{
                    if(!settings.streamOutputList[0])
                        "file:./images/stream_enabled.png"
                    else
                        "file:./images/stream_disabled.png"
                }
            }

            Rectangle{
                id:stream2_rect
                x:200
                y:15
                width:63
                height:20
//                color:"#626262"
                color:{
                    if(settings.liveStreamStatus2 == 0){
                        //success!
                        "red"
                    }else if(settings.liveStreamStatus2 == 3){
                        // lost packet
                        "yellow"
                    }else{
                        //fail or quit
                        "#626262"
                    }
                }
                radius: 3
                Text {
                    text: qsTr("STREAM 2")
                    font.pixelSize: 11
                    color:{
                        if(settings.liveStreamStatus2 == 3){
                            "black"
                        }else{
                            "white"
                        }
                    }
                    anchors.centerIn: parent
                }
            }
            Image{
                x:stream2_rect.x + 76
                y:stream2_rect.y
                width:20
                height:20
                source:{
                    if(!settings.streamOutputList[1])
                        "file:./images/stream_enabled.png"
                    else
                        "file:./images/stream_disabled.png"
                }
            }


            Rectangle{
                id:stream3_rect
                x:348
                y:15
                width:63
                height:20
//                color:"#626262"
                color:{
                    if(settings.liveStreamStatus3 == 0){
                        //success!
                        "red"
                    }else if(settings.liveStreamStatus3 == 3){
                        // lost packet
                        "yellow"
                    }else{
                        //fail or quit
                        "#626262"
                    }
                }
                radius: 3
                Text {
                    text: qsTr("STREAM 3")
                    font.pixelSize: 11
                    color:{
                        if(settings.liveStreamStatus3 == 3){
                            "black"
                        }else{
                            "white"
                        }
                    }
                    anchors.centerIn: parent
                }
            }
            Image{
                x:stream3_rect.x + 76
                y:stream3_rect.y
                width:20
                height:20
                source:{
                    if(!settings.streamOutputList[2])
                        "file:./images/stream_enabled.png"
                    else
                        "file:./images/stream_disabled.png"
                }
            }

            //Record

            Rectangle{
                id:record_rect
                x:stream1_rect.x
                y:50
                width:53
                height:20
                color:{
                    if(settings.recordLedStatus == 0){
                        "red"
                    }
                    else if(settings.recordLedStatus == 8){
                        "yellow"
                    }
                    else{
                        "#626262"
                    }
                }
                radius: 3
                Text {
                    text: qsTr("RECORD")
                    font.pixelSize:11
                    color:{
                        if(settings.recordLedStatus == 8){
                              "black"
                          }else{
                              "white"
                          }
                    }
                    anchors.centerIn: parent
                }
            }

            Text{
                text: {
                    if(isTest)
                        "00:00:00"
                    else
                        settings.recordTimeStr
                }
                color:"white"
                font.pixelSize: 11
                x:record_rect.x + 66
                y:record_rect.y + 4
            }

            Rectangle{
                id:space_rect
                x:stream2_rect.x
                y:record_rect.y
                width:83
                height:20
                color:{
                    if(settings.recordLedStatus == 0){
                        "red"
                    }
                    else if(settings.recordLedStatus == 8){
                        "yellow"
                    }
                    else{
                        "#626262"
                    }
                }
                radius: 3
                Text {
                    text: qsTr("SPACE(TIME)")
                    font.pixelSize:11
                    color:{
                        if(settings.recordLedStatus == 8){
                              "black"
                          }else{
                              "white"
                          }
                    }
                    anchors.centerIn: parent
                }
            }

            Text{
                text: media_sd.gbFree
                color:"white"
                font.pixelSize: 11
                x:space_rect.x + 96
                y:space_rect.y + 4
            }


//            Text {
//                id:rec
//                text: qsTr("REC")
//                color:"red"
//                font.pixelSize: 25
//                font.bold: false
//                anchors.verticalCenter: parent.verticalCenter
//                x:20
//                visible: false
//            }
//            Text {
//                visible: false
//                text: {
//                    if(isTest)
//                        "00:00:00"
//                    else
//                        settings.recordTimeStr
//                }
//                color:"white"
//                font.pixelSize: 20
//                font.bold: false
//                anchors.verticalCenter: parent.verticalCenter
//                x:80
//            }

//            Text {
//                text: media_sd.gbFree + " GB"
//                color:"white"
//                font.pixelSize: 20
//                font.bold: false
//                anchors.top:rec.bottom
//                x:50
//            }

//            Text {
//                text: qsTr("LIVE")
//                color:"red"
//                font.pixelSize: 25
//                font.bold: false
//                anchors.verticalCenter: parent.verticalCenter
//                x:200
//            }
//            Text {
//                text: {
//                    if(isTest)
//                        "00:00:00"
//                    else
//                        settings.liveTimeStr
//                }
//                color:"white"
//                font.pixelSize: 20
//                font.bold: false
//                anchors.verticalCenter: parent.verticalCenter
//                x:265
//            }
        }


        Rectangle{
            width:parent.width
            height:parent.height
            color:"transparent"

//            y:-10

            //in1
            Text {
                id: in1
                x:26+4
                y:90+4
                text: qsTr("IN1")
                font.pixelSize: audioTitleFontSize
                color:{
                    getAudioTextColor(1)
                }
            }

            Rectangle{
                width:18
                height:6
                x:28+4
                y:111+4
                color:{
                    getAudioRectColor(1)
                }
            }

            Text {
                id: in1_number
                x:20-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[2]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(1)
            }

            //in2
            Text {
                id: in2
                x:78+4
                y:90+4
                text: qsTr("IN2")
                font.pixelSize: audioTitleFontSize
                color:getAudioTextColor(2)
            }

            Rectangle{
                width:18
                height:6
                x:80+4
                y:111+4
                color:getAudioRectColor(2)
            }

            Text {
                id: in2_number
                x:72-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[3]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(2)
            }

            //in3
            Text {
                id: in3
                x:130+4
                y:90+4
                text: qsTr("IN3")
                font.pixelSize: audioTitleFontSize
                color:getAudioTextColor(4)
            }

            Rectangle{
                width:18
                height:6
                x:132+4
                y:111+4
                color:getAudioRectColor(4)
            }

            Text {
                id: in3_number
                x:124-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[4]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(4)
            }

            //in4
            Text {
                id: in4
                x:182+4
                y:90+4
                text: qsTr("IN4")
                font.pixelSize: audioTitleFontSize
                color:getAudioTextColor(8)
            }

            Rectangle{
                width:18
                height:6
                x:184+4
                y:111+4
                color:getAudioRectColor(8)
            }

            Text {
                id: in4_number
                x:178-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[5]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(8)
            }

            //aux
            Text {
                id: aux
                x:230+4
                y:90+4
                text: qsTr("AUX")
                font.pixelSize: audioTitleFontSize
                color:getAudioTextColor(16)
            }

            Rectangle{
                width:18
                height:6
                x:236+4
                y:111+4
                color:getAudioRectColor(16)
            }

            Text {
                id: aux_number
                x:230-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[6]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(16)
            }

            Text {
                text: qsTr("0")
                x:265
                y:120
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-10")
                x:260
                y:149
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-20")
                x:260
                y:178
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-40")
                x:260
                y:212+4
                font.pixelSize: audioScaleFontSize
                color:"white"
            }

            //mic1
            Text {
                id: mic1
                x:286+4
                y:90+4
                text: qsTr("MIC 1")
                font.pixelSize: audioTitleFontSize
                color:getAudioTextColor(32)
            }

            Rectangle{
                width:18
                height:6
                x:294+4
                y:111+4
                color:getAudioRectColor(32)
            }

            Text {
                id: mic1_number
                x:284-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[0]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(32)
            }

            //mic2
            Text {
                id: mic2
                x:338+4
                y:90+4
                text: qsTr("MIC 2")
                font.pixelSize: audioTitleFontSize
                color:getAudioTextColor(64)
            }

            Rectangle{
                width:18
                height:6
                x:346+4
                y:111+4
                color:getAudioRectColor(64)
            }

            Text {
                id: mic2_number
                x:338-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[1]
                }
                font.pixelSize: audioNumberFontSize
                color:getAudioTextColor(64)
            }

            Text {
                text: qsTr("0")
                x:375
                y:120
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-10")
                x:370
                y:149
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-20")
                x:370
                y:178
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-40")
                x:370
                y:212+4
                font.pixelSize: audioScaleFontSize
                color:"white"
            }

            //pgm
            Text {
                id: pgm
                x:398+4
                y:90+4
                text: qsTr("PGM")
                font.pixelSize: audioTitleFontSize
                color:"white"
            }

            Rectangle{
                width:18
                height:6
                x:404+4
                y:111+4
                color:"red"
            }

            Text {
                id: pgm_number
                x:396-8
                y:250
                text: {
                    if(isTest)
                        ""
                    else
                        settings.audioNumbers[7]
                }
                font.pixelSize: audioNumberFontSize
                color:{
                    "white"
                }
            }

            Text {
                text: qsTr("0")
                x:433
                y:120
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-10")
                x:428
                y:149
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-20")
                x:428
                y:178
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            Text {
                text: qsTr("-40")
                x:428
                y:212+4
                font.pixelSize: audioScaleFontSize
                color:"white"
            }
            //
        }


        }



    function getAudioTextColor(index){
        if(isTest){
            return "white"
        }

        if(settings.audioOnFlag & index || settings.audioAfvFlag & index){
            return "white"
        }else{
            return "grey"
        }
    }

    function getAudioRectColor(index){
        if(isTest){
            return "red"
        }

        if(settings.audioOnFlag & index){
            return "red"
        }else if((settings.audioAfvFlag & index) && !(index &  settings.pgmTally)){
            return "green"
        }else if((settings.audioAfvFlag & index) && (index & settings.pgmTally)){
            return "red"
        }
        else{
            return "grey"
        }
    }

}
