#ifndef STREAMS_H
#define STREAMS_H

#include <QObject>

class Stream;

class Streams : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Stream* stream1 READ stream1)
    Q_PROPERTY(Stream* stream2 READ stream2)
    Q_PROPERTY(Stream* stream3 READ stream3)
public:
    explicit Streams(QObject *parent = nullptr);

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

private:

    Stream* m_stream1;

    Stream* m_stream2;

    Stream* m_stream3;

signals:

};

#endif // STREAMS_H
