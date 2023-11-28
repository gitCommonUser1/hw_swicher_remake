import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id:root
    width: 1440
    height: 1080
    title: qsTr("Hello World")
    color:"transparent"
    visible: true

    //模式
    property var model: testQml.value

    Rectangle {
        id: rectangle
        x: 0
        y: 330
        width: 1440
        height: 750
        color: "#00ffffff"
        border.color: "#ffffff"
        border.width: 8

        visible: model == 2?true:false

        TestSmallRect {

            id: testSmallRect
            objectName: "live"
            x: 52
            y: 326
        }


        TestSmallRect {
            id: testSmallRect1
            objectName: "mic1"
            x: 154
            y: 119
        }

        TestSmallRect {
            id: testSmallRect2
            objectName: "mic2"
            x: 252
            y: 119
        }

        TestSmallRect {
            id: testSmallRect3
            objectName: "in1"
            x: 154
            y: 191
        }

        TestSmallRect {
            id: testSmallRect4
            objectName: "in2"
            x: 252
            y: 191
        }

        TestSmallRect {
            id: testSmallRect5
            objectName: "in3"
            x: 154
            y: 260
        }

        TestSmallRect {
            id: testSmallRect6
            objectName: "in4"
            x: 252
            y: 260
        }

        TestSmallRect {
            id: testSmallRect7
            objectName: "aux"
            x: 154
            y: 326
        }

        TestSmallRect {
            id: testSmallRect9
            objectName: "pgm"
            x: 252
            y: 326
        }

        TestSmallRect {
            id: testSmallRect10
            objectName: "afv"
            x: 352
            y: 326
        }

        TestSmallRect {
            id: testSmallRect11
            objectName: "on"
            x: 447
            y: 326
        }

        TestSmallRect {
            id: testSmallRect12
            objectName: "mem1"
            x: 578
            y: 119
        }

        TestSmallRect {
            id: testSmallRect13
            objectName: "mem2"
            x: 675
            y: 119
        }

        TestSmallRect {
            id: testSmallRect14
            objectName: "mem3"
            x: 578
            y: 191
        }

        TestSmallRect {
            id: testSmallRect15
            objectName: "mem4"
            x: 675
            y: 191
        }

        TestSmallRect {
            id: testSmallRect17
            objectName: "mem5"
            x: 578
            y: 260
        }

        TestSmallRect {
            id: testSmallRect18
            objectName: "mem6"
            x: 675
            y: 260
        }

        TestSmallRect {
            id: testSmallRect19
            objectName: "mem7"
            x: 578
            y: 326
        }

        TestSmallRect {
            id: testSmallRect20
            objectName: "mem8"
            x: 675
            y: 326
        }

        TestSmallRect {
            id: testSmallRect21
            objectName: "menu"
            x: 777
            y: 326
        }

        TestSmallRect {
            id: testSmallRect22
            objectName: "record"
            x: 961
            y: 119
        }

        TestSmallRect {
            id: testSmallRect23
            objectName: "previous"
            x: 1063
            y: 119
        }

        TestSmallRect {
            id: testSmallRect24
            objectName: "next"
            x: 1157
            y: 119
        }

        TestSmallRect {
            id: testSmallRect25
            objectName: "record_stop"
            x: 961
            y: 191
        }

        TestSmallRect {
            id: testSmallRect26
            objectName: "play"
            x: 1063
            y: 191
        }

        TestSmallRect {
            id: testSmallRect27
            objectName: "play_stop"
            x: 1157
            y: 191
        }

        TestSmallRect {
            id: testSmallRect28
            objectName: "key_on_air"
            x: 961
            y: 260
        }

        TestSmallRect {
            id: testSmallRect29
            objectName: "dsk_on_air"
            x: 1063
            y: 260
        }

        TestSmallRect {
            id: testSmallRect31
            objectName: "key"
            x: 961
            y: 326
        }

        TestSmallRect {
            id: testSmallRect32
            objectName: "dsk"
            x: 1063
            y: 326
        }

        TestSmallRect {
            id: testSmallRect33
            objectName: "bkgd"
            x: 1157
            y: 326
        }

        TestBigRect {
            id: testBigRect
            objectName: "pgm1"
            x: 53
            y: 436
        }

        TestBigRect {
            id: testBigRect1
            objectName: "pvw1"
            x: 53
            y: 569
        }

        TestBigRect {
            id: testBigRect2
            objectName: "pgm2"
            x: 213
            y: 436
        }

        TestBigRect {
            id: testBigRect3
            objectName: "pvw2"
            x: 213
            y: 569
        }

        TestBigRect {
            id: testBigRect4
            objectName: "pgm3"
            x: 374
            y: 436
        }

        TestBigRect {
            id: testBigRect5
            objectName: "pvw3"
            x: 379
            y: 569
        }

        TestBigRect {
            id: testBigRect6
            objectName: "pgm4"
            x: 539
            y: 436
        }

        TestBigRect {
            id: testBigRect7
            objectName: "pvw4"
            x: 539
            y: 569
        }

        TestSmallRect {
            id: testSmallRect34
            objectName: "pgm_black"
            x: 730
            y: 487
            width: 65
            height: 41
        }

        TestSmallRect {
            id: testSmallRect35
            objectName: "pgm_still1"
            x: 809
            y: 436
        }

        TestSmallRect {
            id: testSmallRect36
            objectName: "pgm_still2"
            x: 809
            y: 487
        }

        TestSmallRect {
            id: testSmallRect37
            objectName: "pvw_aux"
            x: 730
            y: 569
        }

        TestSmallRect {
            id: testSmallRect38
            objectName: "pvw_black"
            x: 730
            y: 620
        }

        TestSmallRect {
            id: testSmallRect39
            objectName: "pvw_still1"
            x: 809
            y: 569
        }

        TestSmallRect {
            id: testSmallRect40
            objectName: "pvw_still2"
            x: 809
            y: 620
        }

        TestBigRect {
            id: testBigRect8
            objectName: "cut"
            x: 961
            y: 569
        }

        TestBigRect {
            id: testBigRect9
            objectName: "ftb"
            x: 1122
            y: 436
        }

        TestBigRect {
            id: testBigRect10
            objectName: "auto"
            x: 1122
            y: 569
        }

        TestRound {
            id: testRound
            objectName: "round_audio"
            x: 377
            y: 152
        }

        TestRound {
            id: testRound1
            objectName: "round_menu"
            x: 802
            y: 152
        }

        TestRect {
            id: testRect
            objectName: "push"
            x: 1342
            y: 191
        }

        TestSmallRect {
            id: testSmallRect42
            objectName: "wipe"
            x: 962
            y: 487
            width: 65
            height: 41
        }

        TestSmallRect {
            id: testSmallRect43
            objectName: "dip"
            x: 1041
            y: 436
        }

        TestSmallRect {
            id: testSmallRect44
            objectName: "prev"
            x: 1041
            y: 487
        }

        TestSmallRect {
            id: testSmallRect45
            objectName: "pgm_aux"
            x: 730
            y: 436
            width: 65
            height: 41
        }

        TestSmallRect {
            id: testSmallRect46
            objectName: "mix"
            x: 961
            y: 436
            width: 65
            height: 41
        }
    }

    Rectangle {
        id: rectangle1
        x: 0
        y: 0
        width: 1440
        height: 330
        color: "#00ffffff"
        border.width: 0
        border.color: "#ffffff"

        visible:model>=1?true:false

        Text {
            id: element
            x: 117
            y: 48
            color: "#ffffff"
            text: qsTr("ETHERNET")
            font.pixelSize: 50
        }

        Text {
            id: element1
            x: 470
            y: 48
            color: "#ffffff"
            text: qsTr("USB_OUT")
            font.pixelSize: 50
        }

        Text {
            id: element2
            x: 806
            y: 48
            color: "#ffffff"
            text: qsTr("USB_IN")
            font.pixelSize: 50
        }

        Text {
            id: element3
            x: 1165
            y: 48
            color: "#ffffff"
            text: qsTr("RTC")
            font.pixelSize: 50
        }

        TestRound {
            id: testRound2
            x: 180
            y: 154
            isOk:testQml.ethernetFlag
        }

        TestRound {
            id: testRound3
            x: 520
            y: 154
            isOk:testQml.usbOutFlag
        }

        TestRound {
            id: testRound4
            x: 830
            y: 154
            isOk:testQml.usbInFlag
        }

        TestRound {
            id: testRound5
            x: 1150
            y: 154
            isOk:testQml.rtcFlag
        }
    }

    //音频表绘制
    AudioTable{
        x:1440
        y:800
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
