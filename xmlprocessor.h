#ifndef XMLPROCESSOR_H
#define XMLPROCESSOR_H

#include <QObject>
//#include "models.h"
#include "settings.h"

class QXmlStreamWriter;

class XmlElement:public QObject{
    Q_OBJECT
    int m_status;

public:
    XmlElement(QObject*parent = nullptr){

    }
    virtual void init() = 0;
    //正常记录过程++
    virtual void push(std::map<QString,QString>) = 0;
    //读取文件++
    virtual void append(std::map<QString,QString>) = 0;

    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)

    QList<std::map<QString,QString>> data(){
        return m_data;
    }
    int status() const
    {
        return m_status;
    }

public slots:
    void setStatus(int status)
    {
        if (m_status == status)
            return;

        m_status = status;
        emit statusChanged(m_status);
    }

    void setData(QList<std::map<QString,QString>> data){
        if (m_data == data)
            return;

        m_data = data;
        emit dataChanged(m_data);
    }

    void clearData(){
        m_data.clear();
        emit dataChanged(m_data);
    }

signals:
    void statusChanged(int status);
    void dataChanged(QList<std::map<QString,QString>>);

protected:
    QList<std::map<QString,QString>>m_data;
};

class XmlElementManager:public QObject{
    Q_OBJECT
public:
    XmlElementManager(QObject *parent = nullptr){

    }
    XmlElementManager(QString fileName,QObject *parent = nullptr){
        m_fileName = fileName;
    }

    QList<XmlElement*> data(){
        return m_data;
    }
    QString fileName(){
        return m_fileName;
    }
    void setFileName(QString filename){
        m_fileName = filename;
    }

protected:
    QList<XmlElement*>m_data;
    QString m_fileName;
};

class XmlProcessor : public QObject
{
    Q_OBJECT
public:
    static XmlProcessor& getInstance(){
        static XmlProcessor xml;
        return xml;
    }

    //
    void writeXml(QObject *object);
    void writeXml(QObject *object,QXmlStreamWriter & stream);

    //
    int readXml(QObject *object);
    void readXml(QObject *object,QXmlStreamWriter &stream);

    void writeXml(QList<XmlElementManager*>xmlList);
    void readXml(QList<XmlElementManager*>xmlList);

    void writeXml(XmlElementManager* xml);
    int readXml(XmlElementManager* xml);

    void readStreamingXml(StreamData&map);
    void readRecordData(std::map<QString,QList<STREAM_PROFILE>> &map);

    QString getLastErrorInfo();

private:
    XmlProcessor() {} // private constructor
    ~XmlProcessor() {} // private destructor
    XmlProcessor(const XmlProcessor&) = delete; // delete copy constructor
    XmlProcessor& operator=(const XmlProcessor&) = delete; // delete assignment operator

    QString lastErrorInfo;

signals:

};

#endif // XMLPROCESSOR_H
