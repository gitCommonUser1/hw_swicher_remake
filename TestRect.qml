import QtQuick 2.0

Rectangle{
    property var isOk: false
    width:40
    height:400
    color:isOk?"green":"red"
    radius: 15
}
