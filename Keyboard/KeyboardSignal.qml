pragma Singleton
import QtQuick 2.0

QtObject {
    signal sendKey(var key)
    signal sendSpace()
    signal sendUpperCase()//大小写、符号数字符号切换
    signal sendSymbolChange()//字母、数字切换
    signal sendBackSpace()
    signal sendArrowLeft()
    signal sendArrowRight()
}
