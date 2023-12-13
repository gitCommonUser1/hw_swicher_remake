#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>

class Format : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int outFormat READ outFormat WRITE setOutFormat NOTIFY outFormatChanged)
public:
    explicit Format(QObject *parent = nullptr);

    int outFormat() const
    {
        return m_outFormat;
    }

public slots:
    void setOutFormat(int outFormat)
    {
        if (m_outFormat == outFormat)
            return;

        m_outFormat = outFormat;
        emit outFormatChanged(m_outFormat);
    }

private:


    int m_outFormat;

signals:

void outFormatChanged(int outFormat);
};

#endif // FORMAT_H
