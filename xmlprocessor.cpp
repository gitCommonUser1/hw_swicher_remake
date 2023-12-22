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
