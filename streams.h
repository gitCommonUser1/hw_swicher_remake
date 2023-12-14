#ifndef STREAMS_H
#define STREAMS_H

#include <QObject>

class Stream;

class Streams : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(Stream* stream1 READ stream1)
    Q_PROPERTY(Stream* stream2 READ stream2)
    Q_PROPERTY(Stream* stream3 READ stream3)
public:
    explicit Streams(QObject *parent = nullptr);

    static int srcStringToIndex(QString src);
    static QString srcIndexToString(int index);
    static int qualityStringToIndex(QString quality);
    static QString qualityIndexToString(int index);

    enum QUALITYSRC{
        RECORDING = 0,
        STREAMING
    };

    enum QUALITY{
        HIGH = 0,
        MID,
        LOW
    };

    enum STREAMS{
        STREAM1 = 0,
        STREAM2,
        STREAM3,
        STREAM_MAX
    };

    Stream* stream1() const
    {
        return m_stream1;
    }

    Stream* stream2() const
    {
        return m_stream2;
    }

    Stream* stream3() const
    {
        return m_stream3;
    }

    int quality() const
    {
        return m_quality;
    }

public slots:
    void setQuality(int quality)
    {
        if(quality >= LOW)
            quality = LOW;
        if(quality <= HIGH)
            quality = HIGH;

        if (m_quality == quality)
            return;

        m_quality = quality;
        emit qualityChanged(m_quality);
    }

private:

    Stream* m_stream1;

    Stream* m_stream2;

    Stream* m_stream3;

    int m_quality;

signals:

void qualityChanged(int quality);
};

#endif // STREAMS_H
