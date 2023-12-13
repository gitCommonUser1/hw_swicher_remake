#ifndef OUTFORMAT_H
#define OUTFORMAT_H

#include <QObject>

class Format;
class OutputColorSpace;

class OutFormat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Format* format READ format)
    Q_PROPERTY(OutputColorSpace* out1ColorSpace READ out1ColorSpace)
    Q_PROPERTY(OutputColorSpace* out2ColorSpace READ out2ColorSpace)
public:
    explicit OutFormat(QObject *parent = nullptr);

    static int outFormatStringToIndex(QString outFormat);
    static QString outFormatIndexToString(int index);

    static int outputStringToIndex(QString output);
    static QString outputIndexToString(int index);

    enum OUTPUTS{
        OUTPUT1 = 0,
        OUTPUT2
    };

    enum OUTFORMAT{
        OUT_1080p24,
        OUT_1080p25,
        OUT_1080p30,
        OUT_1080p50,
        OUT_1080p60,
        OUT_1080pMAX
    };


    Format* format() const
    {
        return m_format;
    }

    OutputColorSpace* out1ColorSpace() const
    {
        return m_out1ColorSpace;
    }

    OutputColorSpace* out2ColorSpace() const
    {
        return m_out2ColorSpace;
    }

public slots:

private:


    Format* m_format;

    OutputColorSpace* m_out1ColorSpace;

    OutputColorSpace* m_out2ColorSpace;

signals:

};

#endif // OUTFORMAT_H
