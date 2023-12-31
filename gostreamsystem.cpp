#include "gostreamsystem.h"
#include "mixeffectblocks.h"
#include "colorbacks.h"
#include "downstreamkeys.h"
#include "device_info.h"
#include <QDebug>
#include <QMetaProperty>
#include <QXmlStreamWriter>
#include <QFile>
#include "hw_config.h"
#include "supersources.h"
#include "audiomixer.h"
#include "stillgenerator.h"
#include "still.h"
#include "streams.h"
#include "playback.h"
#include "setting.h"
#include "macropool.h"
#include "macro.h"
#include <QTimer>

#define PRODUCT_NAME "GoStream Deck"

Profile::Profile(QObject *parent) : QObject(parent)
{
    setProduct(PRODUCT_NAME);
    auto str = get_version();
    QString version = QString(str);
    QStringList versions = version.split(".");
    if(versions.size() == 3)
    {
        setMajorVersion(versions[0].toInt());
        setMinorVersion(versions[1].toInt());
        setPatchVersion(versions[2].toInt());
    }
    if(str)
        free(str);

    this->setObjectName("Profile");
    m_mixEffectBlocks = new MixEffectBlocks(this);
    m_mixEffectBlocks->setObjectName("mixEffectBlocks");
    m_colorBacks = new ColorBacks(this);
    m_colorBacks->setObjectName("colorBacks");
    m_downstreamKeys = new DownstreamKeys(this);
    m_downstreamKeys->setObjectName("downstreamKeys");
    m_superSources = new SuperSources(this);
    m_superSources->setObjectName("superSources");
    m_audioMixer = new AudioMixer(this);
    m_audioMixer->setObjectName("audioMixer");
    m_stillGenerator = new StillGenerator(this);
    m_stillGenerator->setObjectName("stillGenerator");
    m_macroPool = new MacroPool(this);
    m_macroPool->setObjectName("macroPool");
    m_streams = new Streams(this);
    m_streams->setObjectName("streams");
    m_playback = new Playback(this);
    m_playback->setObjectName("playback");
    m_setting = new Setting(this);
    m_setting->setObjectName("setting");

    read(this);
}

void Profile::emitSignals()
{
    emitSignal(this);
    macroPool()->checkMacro();
}

void Profile::emitSignal(QObject *object)
{
    auto metaObject = object->metaObject();
    for(int i = 0;i < metaObject->propertyCount();++i)
    {
        auto property = metaObject->property(i);
        auto name = property.name();
        auto typeName = property.typeName();
        qDebug() << name;
        if(QString(typeName).contains("QList"))
            continue;
        if(property.type() == QVariant::UserType)
        {
            //single
            emitSignal(object->findChild<QObject*>(name));
        }
        else
        {
            if(property.hasNotifySignal() && !isHiddenProperty(object,name))
            {
                property.notifySignal().invoke(object,QGenericArgument(" ",property.read(object).data()));
            }
        }
    }
}

void Profile::autoSaveInit(QObject *object)
{
    auto metaObject = object->metaObject();
    for(int i = 0;i < metaObject->propertyCount();++i)
    {
        auto property = metaObject->property(i);
        auto name = property.name();
        auto typeName = property.typeName();
        qDebug() << name;
        if(QString(typeName).contains("QList"))
            continue;
        if(property.type() == QVariant::UserType)
        {
            //single
            autoSaveInit(object->findChild<QObject*>(name));
        }
        else
        {
            if(property.hasNotifySignal() && !isHiddenProperty(object,name))
            {
                QByteArray normalizedSignature = QMetaObject::normalizedSignature("autoSave()");
                int methodIndex = this->metaObject()->indexOfMethod(normalizedSignature);
                QMetaMethod method = this->metaObject()->method(methodIndex);
                connect(object,property.notifySignal(),this,method);
            }
        }
    }
}

void Profile::autoSave()
{
#define WRITE_SPACE 10000
    if(autoSaveTimer == nullptr)
    {
        autoSaveTimer = new QTimer;
        connect(autoSaveTimer,&QTimer::timeout,this,[=](){
            this->write(this);
        });
        autoSaveTimer->setSingleShot(true);
    }
    autoSaveTimer->start(WRITE_SPACE);
}

void Profile::write(QObject *object)
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

    writeRecursion(object,stream);

    //doc end
    stream.writeEndDocument();

    file.flush();
    ::fsync(file.handle());

    file.close();
}

void Profile::writeRecursion(QObject *object, QXmlStreamWriter &stream)
{
    auto obj = object->metaObject();
    stream.writeStartElement(obj->className());
    //par 读取所有静态属性
    for(int i = 0;i < obj->propertyCount();++i)
    {
        auto property = obj->property(i);
        auto name = property.name();
        auto type = property.type();
        auto typeName = property.typeName();
        if(strcmp(name,"objectName") != 0)
        {
            //排除objectName
            if(type != QVariant::UserType)
            {
                if(type == QVariant::Map)
                {
                    //map类型先遍历再写
                    QVariantMap map = property.read(object).toMap();
                    //id提前
                    if(map.contains("id"))
                        stream.writeAttribute("id",map["id"].toString());
                    for(auto it = map.begin();it != map.end();++it)
                    {
                        if(it.key() == "id")
                            continue;
                        stream.writeAttribute(it.key(),it.value().toString());
                    }
                }
                else
                {
                    //排除自定义类型，只读取qt内置类型
                    //这里是节点参数列表
                    if(!isHiddenProperty(object,name))
                        stream.writeAttribute(name,property.read(object).toString());
                }
            }
            else
            {
//                这里是子节点列表
                if(QString(typeName).contains("QList"))
                {
                    QVariant v = object->property(name);
                    QList<QObject*> stills = v.value<QList<QObject*>>();
                    //list
                    for(int j = 0;j < stills.size();++j)
                    {
                        writeRecursion(stills[j],stream);
                    }
                }
                else
                {
                    //single
                    writeRecursion(object->findChild<QObject*>(name),stream);
                }
            }
        }
    }
    stream.writeEndElement();
}

int Profile::read(QObject *object)
{
    QString objName = object->objectName();
    if(objName == ""){
        return -1;
    }

    objName = DATA_PATH "/" + objName + ".xml";
    std::string stdObjName = objName.toStdString();

    qDebug() << "objName:" << objName;

    QFile file(objName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open file fail!";
        return -1;
    }
    auto obj = object->metaObject();
    bool profileFlag = true;

    QXmlStreamReader stream(&file);
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
                qDebug() << "start name:" << name;
                //情况一： 静态子类
            {
                bool  case1Flag = false;
                qDebug() << "name:" << name;
                for(int i = 0;i < object->children().size();++i)
                {
                    auto childrenMetaObj = object->children()[i]->metaObject();
                    //非静态属性类，continue
                    if(object->children()[i]->objectName().isEmpty())
                        continue;
                    if(childrenMetaObj->className() == name)
                    {
                        bool finalFlag = false;
                        bool isSet = false;
                        for(int j = 0;j < childrenMetaObj->propertyCount();++j)
                        {
                            if(childrenMetaObj->property(j).isFinal())
                            {
                                finalFlag = true;
                                if(attributes.hasAttribute(childrenMetaObj->property(j).name()))
                                {
                                    if(attributes.value(childrenMetaObj->property(j).name()).toString() == childrenMetaObj->property(j).read(object->children()[i]))
                                    {
                                        object = object->children()[i];
                                        obj = object->metaObject();
                                        isSet = true;
                                        case1Flag = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if(!finalFlag)
                        {
                            object = object->children()[i];
                            obj = object->metaObject();
                            case1Flag = true;
                            break;
                        }
                        if(isSet)
                            break;
                    }
                }
                if(case1Flag)
                {
                    for(int i = 0; i < obj->propertyCount();++i)
                    {
                        auto property = obj->property(i);
                        auto propertyName = property.name();
                        qDebug() << "propertyName: " << propertyName;
                        if(property.type() == QVariant::Map){
                            QVariantMap map;
                            for(int j = 0;j < attributes.count();++j){
                                map[attributes[j].name().toString()] = attributes[j].value().toString();
                            }
                            property.write(object,map);
                        }
                        else if(attributes.hasAttribute(propertyName))
                        {
                            if(property.type()  == QVariant::Int)
                                property.write(object,attributes.value(propertyName).toInt());
                            else if(property.type()  == QVariant::Double)
                                property.write(object,attributes.value(propertyName).toDouble());
                            else if(property.type()  == QVariant::String)
                                property.write(object,attributes.value(propertyName).toString());
                            else if(property.type()  == QVariant::Bool){
                                if(attributes.value(propertyName).toString().compare("true",Qt::CaseInsensitive) == 0)
                                    property.write(object,true);
                                else
                                    property.write(object,false);
                            }
                        }
                    }
                    break;
                }
            }
                //情况二： QList<QObject*> 动态创建
                //getDynamicChildrenClassName
                //每个读取xml动态创建子类的类中都有一个getDynamicChildrenClassName槽函数
            {
                auto dynamicMetaObject = object->metaObject();
                if(dynamicMetaObject->indexOfSlot("getDynamicChildrenClassName()") != -1)
                {
                    QObject *ret = nullptr;
                    dynamicMetaObject->method(dynamicMetaObject->indexOfSlot("getDynamicChildrenClassName()")).invoke(object,Qt::DirectConnection,Q_RETURN_ARG(QObject*, ret));
                    if(ret != nullptr){
                        object = ret;
                        obj = object->metaObject();
                        for(int i = 0; i < obj->propertyCount();++i)
                        {
                            auto property = obj->property(i);
                            auto propertyName = property.name();
                            qDebug() << "propertyName: " << propertyName;
                            if(property.type() == QVariant::Map){
                                QVariantMap map;
                                for(int j = 0;j < attributes.count();++j){
                                    map[attributes[j].name().toString()] = attributes[j].value().toString();
                                }
                                property.write(object,map);
                            }
                            else if(attributes.hasAttribute(propertyName))
                            {
                                if(property.type()  == QVariant::Int)
                                    property.write(object,attributes.value(propertyName).toInt());
                                else if(property.type()  == QVariant::Double)
                                    property.write(object,attributes.value(propertyName).toDouble());
                                else if(property.type()  == QVariant::String)
                                    property.write(object,attributes.value(propertyName).toString());
                                else if(property.type()  == QVariant::Bool){
                                    if(attributes.value(propertyName).toString().compare("true",Qt::CaseInsensitive) == 0)
                                        property.write(object,true);
                                    else
                                        property.write(object,false);
                                }
                            }
                        }
                    }
                }
            }

                break;
            case QXmlStreamReader::EndElement:
                if(object != startObject)
                {
                    object = object->parent();
                    obj = object->metaObject();
                }
                qDebug() << "end name:" << name;
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
        return -1;
    }
    return 0;
}

//这里规定，如果这个类中有子类名字与属性名字相同，则不写入这个属性。
//实现效果为，动态设置某个属性是否需要写入文件
bool Profile::isHiddenProperty(QObject *object,QString name)
{
    auto children = object->children();
    for(int i = 0;i < children.size();++i)
    {
        if(children[i]->objectName() == name)
            return true;
    }

    return false;
}
