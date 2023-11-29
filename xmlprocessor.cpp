#include "xmlprocessor.h"
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QMetaObject>
#include <QMetaMethod>
#include "profile_include.h"
#include "hw_config.h"

#define MACRO_OP "Op"



void XmlProcessor::writeXml(QObject *object)
{
    QString objName = object->objectName();
    if(objName == "")
        return ;

    objName = DATA_PATH "/" + objName + ".xml";
    std::string stdObjName = objName.toStdString();

    QFile file(objName);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "open file fail!";
        return ;
    }

    QXmlStreamWriter stream(&file);
    //xml info
    stream.setAutoFormatting(true);
    //doc start
    stream.writeStartDocument();

    writeXml(object,stream);

    //doc end
    stream.writeEndDocument();

    file.close();

    //断电无法保存，c语言方式再写一次
    file.setFileName(objName);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();

    FILE *fp;
    fp = fopen(stdObjName.data(), "wt+");
    fwrite(ba, 1, ba.size(), fp);
    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
}

void XmlProcessor::writeXml(QObject *object, QXmlStreamWriter &stream)
{
    auto obj = object->metaObject();
    stream.writeStartElement(obj->className());
    //par
    for(int i = 0;i < obj->propertyCount();++i)
    {
        auto property = obj->property(i);
        auto name = property.name();
        auto type = property.type();
        if(strcmp(name,"objectName") != 0)
        {
            //排除objectName
            if(type != QVariant::UserType)
            {
                //排除自定义类型，只读取qt内置类型
                //这里是节点参数列表
                stream.writeAttribute(name,property.read(object).toString());
            }
            else
            {
                //这里是子节点列表
                writeXml(object->findChild<QObject*>(name),stream);
            }
        }
    }
    stream.writeEndElement();
}

int XmlProcessor::readXml(QObject *object)
{
    QString objName = object->objectName();
    if(objName == ""){
        lastErrorInfo = "objName is empty!";
        return -1;
    }

    objName = DATA_PATH "/" + objName + ".xml";
    std::string stdObjName = objName.toStdString();

    qDebug() << "objName:" << objName;

    QFile file(objName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open file fail!";
        lastErrorInfo = "open file fail!";
        return -1;
    }
    auto obj = object->metaObject();
    lastErrorInfo = "";
    bool profileFlag = true;

    QXmlStreamReader stream(&file);
    XmlElement *currentElement = nullptr;
    // xml info
    stream.readNext();
    stream.readNext();
    if(stream.name() == object->objectName()){
        //profile
        auto attributes = stream.attributes();
        for(int i = 0;i < obj->propertyCount();++i)
        {
            auto property = obj->property(i);
            auto name = property.name();
            auto type = property.type();
            if(strcmp(name,"objectName") != 0 && type != QVariant::UserType)
            {
                if(!attributes.hasAttribute(name))
                {
                    profileFlag = false;
                    break;
                }
                else
                {
                    if(attributes.value(name).toString() != property.read(object).toString())
                    {
                        profileFlag = false;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        profileFlag = false;
    }
    qDebug() << "profileFlag :" << profileFlag;
    if(profileFlag)
    {
        stream.readNext();
        auto startObject = object;
        while(!stream.atEnd())
        {
            if(stream.hasError())
            {
                qDebug() << "stream has error!";
                break;
            }
            auto type = stream.tokenType();
            auto name = stream.name();
            auto attributes = stream.attributes();
            switch (type) {
            case QXmlStreamReader::StartElement:
                qDebug() << "start:" << name;
                for(int i = 0;i < object->children().size();++i)
                {
                    auto childrenMetaObj = object->children()[i]->metaObject();
                    if(childrenMetaObj->className() == name)
                    {
                        object = object->children()[i];
                        obj = object->metaObject();
                        break;
                    }
                }

                for(int i = 0; i < obj->propertyCount();++i)
                {
                    auto property = obj->property(i);
                    auto propertyName = property.name();
                    if(attributes.hasAttribute(propertyName))
                    {
                        if(property.type()  == QVariant::Int)
                            property.write(object,attributes.value(propertyName).toInt());
                        else if(property.type()  == QVariant::Double)
                            property.write(object,attributes.value(propertyName).toDouble());
                        else if(property.type()  == QVariant::String)
                            property.write(object,attributes.value(propertyName).toString());
                    }
                }
                break;
            case QXmlStreamReader::EndElement:
                if(object != startObject)
                {
                    object = object->parent();
                    obj = object->metaObject();
                }
                qDebug() << "end:" << name;
                break;
            default:
                break;
            }
            stream.readNext();
        }
    }
    file.close();

    if(stream.hasError()){
        qDebug() << "read error:" << stream.errorString() << " at line:" << stream.lineNumber();
        lastErrorInfo = stream.errorString() + " at line:" + QString::number(stream.lineNumber());
        return -1;
    }
    return 0;
}

void XmlProcessor::readXml(QObject *object, QXmlStreamWriter &stream)
{

}

void XmlProcessor::writeXml(QList<XmlElementManager *> xmlList)
{

}

void XmlProcessor::readXml(QList<XmlElementManager *> xmlList)
{

}

//目前仅支持macro的接口
void XmlProcessor::writeXml(XmlElementManager *xml)
{
    QFile file(xml->fileName());
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "open file fail!";
    }

    QString managerName = xml->metaObject()->className();
    auto xmlList = xml->data();


    QXmlStreamWriter stream(&file);
    //xml info
    stream.setAutoFormatting(true);
    //doc start
    stream.writeStartDocument();

    //<Profile product="GoStream Deck">
    //profile start
    stream.writeStartElement("Profile");
    stream.writeAttribute("product","GoStream Deck");

    //class Name (manager) start
    stream.writeStartElement(managerName);

    for(int i = 0;i < xmlList.size();++i)
    {
        auto mapList = xmlList[i]->data();
        if(mapList.isEmpty())
            continue;
        auto itemName = xmlList[i]->metaObject()->className();
        stream.writeStartElement(itemName);
        stream.writeAttribute("index",QString::number(i+1));
        for(int j = 0;j < mapList.size();++j)
        {
            stream.writeStartElement(MACRO_OP);
            auto mapData = mapList[j];
            for(auto it = mapData.begin();it != mapData.end();++it)
            {
                stream.writeAttribute(it->first,it->second);
            }
            stream.writeEndElement();
        }
        stream.writeEndElement();
    }

    //class Name (manager) end
    stream.writeEndElement();
    //profile end
    stream.writeEndElement();
    //doc end
    stream.writeEndDocument();

    file.close();

    //断电无法保存，c语言方式再写一次
    file.setFileName(xml->fileName());
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();

    FILE *fp;
    fp = fopen(xml->fileName().toUtf8().data(), "wt+");
    fwrite(ba, 1, ba.size(), fp);
    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
}

//目前仅支持macro的接口
int XmlProcessor::readXml(XmlElementManager *xml)
{
    QFile file(xml->fileName());
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open file fail!";
    }

    lastErrorInfo = "";

    QXmlStreamReader stream(&file);
    // xml info
    stream.readNext();

    auto xmlList = xml->data();
    XmlElement *currentElement = nullptr;

    while(!stream.atEnd())
    {
        if(stream.hasError())
        {
            qDebug() << "stream has error!";
            break;
        }
        auto type = stream.tokenType();
        switch (type) {
        case QXmlStreamReader::StartElement:
            if(stream.name() == "Macro")
            {
                auto macro_index = stream.attributes().value("index");
                QString macro_index_str = macro_index.toString();
                int macro_index_int = macro_index.toInt();
                macro_index_int--;
                if(macro_index_str != "" && (macro_index_int >= 0 && macro_index_int < xmlList.size())){
                    currentElement = xmlList[macro_index_int];
                }
            }
            else if(stream.name() == MACRO_OP)
            {
                if(!currentElement)
                    break;

                auto list = stream.attributes();
                std::map<QString,QString>map;
                for(int i = 0;i < list.size();++i)
                {
                    map[list[i].name().toString()] = list[i].value().toString();
                }
                currentElement->append(map);
            }
            break;
        case QXmlStreamReader::EndElement:
            if(stream.name() == "Macro")
            {
                currentElement = nullptr;
            }
            break;
        default:
            break;
        }
        stream.readNext();
    }

    file.close();

    if(stream.hasError()){
        qDebug() << "read error:" << stream.errorString() << " at line:" << stream.lineNumber();
        lastErrorInfo = stream.errorString() + " at line:" + QString::number(stream.lineNumber());
        return -1;
    }
    return 0;
}

void XmlProcessor::readStreamingXml(StreamData &map)
{
#define STREAM_PATH "/oem/hw_rockchip_qt/Streaming.xml"

    QFile file(STREAM_PATH);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open file fail!";
        return ;
    }
    map.clear();

    QXmlStreamReader stream(&file);
    // xml info
    stream.readNext();

    bool service = false;
    bool servers = false;
    bool profiles = false;
    bool profile = false;

    std::map<QString,QString>name_url;
    std::map<QString,QList<STREAM_PROFILE>>name_config;
    STREAM_SERVER data;
    QString last_service_name;
    QString last_name_url_name;
    QString last_name_config_name;
    STREAM_PROFILE config;

    while(!stream.atEnd())
    {
        if(stream.hasError())
        {
            qDebug() << "stream has error!";
            break;
        }
        auto type = stream.tokenType();
        switch (type) {
        case QXmlStreamReader::StartElement:
            if(stream.name() == "service"){
                service = 1;
            }else if(stream.name() == "servers"){
                servers = 1;
            }else if(stream.name() == "profiles"){
                profiles = 1;
            }else if(stream.name() == "profile"){
                profile = 1;
            }
            if(stream.name() == "name" && service == 1 && servers == 0 && profiles == 0){
                //service
                last_service_name = stream.readElementText();
            }else if(stream.name() == "name" && service == 1 && servers == 1 && profiles == 0){
                //server name
                last_name_url_name = stream.readElementText();
            }else if(stream.name() == "url" && service == 1 && servers == 1 && profiles == 0){
                //server url
                name_url[last_name_url_name] = stream.readElementText();
            }else if(stream.name() == "name" && service == 1 && servers == 0 && profiles == 1){
                //profile name
                last_name_config_name = stream.readElementText();
            }else if(stream.name() == "config" && service == 1 && servers == 0 && profiles == 1){
                QString resolution =  stream.attributes().value("resolution").toString();
                QString fps =  stream.attributes().value("fps").toString();
                config.resolution = resolution;
                config.fps = fps;
            }else if(stream.name() == "bitrate" && service == 1 && servers == 0 && profiles == 1){
                int bitrate = stream.readElementText().toInt();
                config.bitrate = bitrate;
            }else if(stream.name() == "audio-bitrate" && service == 1 && servers == 0 && profiles == 1){
                int audio_bitrate = stream.readElementText().toInt();
                config.audio_bitrate = audio_bitrate;
            }
            break;
        case QXmlStreamReader::EndElement:
            if(stream.name() == "service"){
                service = 0;
                map[last_service_name] = data;
            }else if(stream.name() == "servers"){
                servers = 0;
                data.name_url = name_url;
                name_url.clear();
            }else if(stream.name() == "profiles"){
                profiles = 0;
                data.name_config = name_config;
                name_config.clear();
            }else if(stream.name() == "profile"){
                profile = 0;
            }else if(stream.name() == "config"){
                name_config[last_name_config_name].push_back(config);
            }
            break;
        default:
            break;
        }
        stream.readNext();
    }
    file.close();
}

void XmlProcessor::readRecordData(std::map<QString,QList<STREAM_PROFILE>> & map)
{
#define RECORDING_PATH "/oem/hw_rockchip_qt/Recording.xml"

    QFile file(RECORDING_PATH);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open file fail!";
        return ;
    }

    map.clear();

    QXmlStreamReader stream(&file);
    // xml info
    stream.readNext();

    bool profile = false;
    bool config = false;

    QList<STREAM_PROFILE> list;
    STREAM_PROFILE data;
    QString name = "";

    while(!stream.atEnd())
    {
        if(stream.hasError())
        {
            qDebug() << "stream has error!";
            break;
        }
        auto type = stream.tokenType();
        switch (type) {
        case QXmlStreamReader::StartElement:
            if(stream.name() == "profile"){
                profile = 1;
            }else if(stream.name() == "config"){
                config = 1;
                data.resolution = stream.attributes().value("resolution").toString();
                data.fps = stream.attributes().value("fps").toString();
            }else if(stream.name() == "bitrate"){
                data.bitrate = stream.readElementText().toInt();
            }else if(stream.name() == "audio-bitrate"){
                data.audio_bitrate = stream.readElementText().toInt();
            }else if(stream.name() == "name"){
                name = stream.readElementText();
            }
            break;
        case QXmlStreamReader::EndElement:
            if(stream.name() == "profile"){
                profile = 0;
                map[name] = list;
                list.clear();
            }else if(stream.name() == "config"){
                config = 0;
                list.push_back(data);
            }
            break;
        default:
            break;
        }
        stream.readNext();
    }
    file.close();
}

QString XmlProcessor::getLastErrorInfo()
{
    return lastErrorInfo;
}
