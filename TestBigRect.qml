import QtQuick 2.0

Rectangle{
    property var isOk: false
    width:145
    height:92
    color:isOk?"green":"red"
    radius: 15
}
