#ifndef OUTPUTCOLORSPACE_H
#define OUTPUTCOLORSPACE_H

#include <QObject>

class OutputColorSpace : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int outputIndex READ outputIndex WRITE setOutputIndex FINAL)
    Q_PROPERTY(int colorSpace READ colorSpace WRITE setColorSpace NOTIFY colorSpaceChanged)
public:
    explicit OutputColorSpace(int index,QObject *parent = nullptr);

    int outputIndex() const
    {
        return m_outputIndex;
    }

    int colorSpace() const
    {
        return m_colorSpace;
    }

public slots:
    void setOutputIndex(int outputIndex)
    {
        m_outputIndex = outputIndex;
    }

    void setColorSpace(int colorSpace)
    {
        if (m_colorSpace == colorSpace)
            return;

        m_colorSpace = colorSpace;
        emit colorSpaceChanged(m_colorSpace);
    }

private:


    int m_outputIndex;

    int m_colorSpace;

signals:

void colorSpaceChanged(int colorSpace);
};

#endif // OUTPUTCOLORSPACE_H
