#ifndef MYPROVIDER_H
#define MYPROVIDER_H

#include <QQuickImageProvider>
#include "settings.h"
#include "MenuEnums.h"
#include "include/hw_config.h"
#include <QDebug>

extern Settings *settings;

class MyProvider : public QQuickImageProvider
{
public:
    MyProvider(): QQuickImageProvider(QQuickImageProvider::Image){

    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize){
//        QString fileName = SD_IMAGE_PATH + settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE]->getText();
        QImage image;
        image.load(m_fileName);
        if(id == "key"){
            //a
            QSize size = image.size();
            auto bits = image.bits();
            for(int i = 0;i < size.width();++i){
                for(int j = 0;j < size.height();++j){
                    bits[j * size.width() * 4 + i * 4 + 0] = bits[j * size.width() * 4 + i * 4 + 3];
                    bits[j * size.width() * 4 + i * 4 + 1] = bits[j * size.width() * 4 + i * 4 + 3];
                    bits[j * size.width() * 4 + i * 4 + 2] = bits[j * size.width() * 4 + i * 4 + 3];
//                    int a = qAlpha(image.pixel(i,j));
//                    QColor color(a,a,a);
//                    image.setPixel(i,j,color.rgb());
                }
            }
        }else if(id == "fill"){
            //rgb
        }
        qDebug() << "requestImage:" << m_fileName;
        return image;
    }
private:
    QString m_fileName;

public:
    void setStillImageFileName(QString name){
        m_fileName = name;
    }
};


class myStillLoadClass :public QObject{
public:
    Q_OBJECT
    myStillLoadClass(){
        myProvider = new MyProvider();
    }
private:
    MyProvider *myProvider;
signals:

};

#endif // MYPROVIDER_H
