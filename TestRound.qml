import QtQuick 2.0

Rectangle{
    property var isOk: false
    width:120
    height:120
    color:isOk?"green":"red"
    radius: 60
}
