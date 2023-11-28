#ifndef COLORBACK_H
#define COLORBACK_H

#include <QObject>

class ColorBack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int colorIndex READ colorIndex WRITE setColorIndex NOTIFY colorIndexChanged)
    Q_PROPERTY(int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
public:
    explicit ColorBack(QObject *parent = nullptr);

    int hue() const
    {
        return m_hue;
    }

    int colorIndex() const
    {
        return m_colorIndex;
    }

    int saturation() const
    {
        return m_saturation;
    }

    int brightness() const
    {
        return m_brightness;
    }

public slots:
    void setHue(int hue)
    {
        if (m_hue == hue)
            return;

        m_hue = hue;
        emit hueChanged(m_hue);
    }

    void setColorIndex(int colorIndex)
    {
        if (m_colorIndex == colorIndex)
            return;

        m_colorIndex = colorIndex;
        emit colorIndexChanged(m_colorIndex);
    }

    void setSaturation(int saturation)
    {
        if (m_saturation == saturation)
            return;

        m_saturation = saturation;
        emit saturationChanged(m_saturation);
    }

    void setBrightness(int brightness)
    {
        if (m_brightness == brightness)
            return;

        m_brightness = brightness;
        emit brightnessChanged(m_brightness);
    }

private:

    int m_hue;

    int m_colorIndex;

    int m_saturation;

    int m_brightness;

signals:

void hueChanged(int hue);
void colorIndexChanged(int colorIndex);
void saturationChanged(int saturation);
void brightnessChanged(int brightness);
};

#endif // COLORBACK_H
