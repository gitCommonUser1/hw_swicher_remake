import QtQuick 2.0

Rectangle{
    property var isOk: false
    width:65
    height:41
    color:isOk?"green":"red"
    radius: 8
}
