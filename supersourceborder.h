#ifndef SUPERSOURCEBORDER_H
#define SUPERSOURCEBORDER_H

#include <QObject>

class SuperSourceBorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex FINAL)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int colorHue READ colorHue WRITE setColorHue NOTIFY colorHueChanged)
    Q_PROPERTY(int colorSaturation READ colorSaturation WRITE setColorSaturation NOTIFY colorSaturationChanged)
    Q_PROPERTY(int colorBrightness READ colorBrightness WRITE setColorBrightness NOTIFY colorBrightnessChanged)

public:
    explicit SuperSourceBorder(int index,QObject *parent = nullptr);

    int index() const
    {
        return m_index;
    }

    int width() const
    {
        return m_width;
    }

    int colorHue() const
    {
        return m_colorHue;
    }

    int colorSaturation() const
    {
        return m_colorSaturation;
    }

    int colorBrightness() const
    {
        return m_colorBrightness;
    }

public slots:
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

    void setWidth(int width)
    {
        if(width < m_width_min)
            width = m_width_min;
        if(width > m_width_max)
            width = m_width_max;
        if (m_width == width)
            return;

        m_width = width;
        emit widthChanged(m_width);
    }

    void setColorHue(int colorHue)
    {
        if(colorHue < m_colorHue_min)
            colorHue = m_colorHue_min;
        if(colorHue > m_colorHue_max)
            colorHue = m_colorHue_max;
        if (m_colorHue == colorHue)
            return;

        m_colorHue = colorHue;
        emit colorHueChanged(m_colorHue);
    }

    void setColorSaturation(int colorSaturation)
    {
        if(colorSaturation < m_colorSaturation_min)
            colorSaturation = m_colorSaturation_min;
        if(colorSaturation > m_colorSaturation_max)
            colorSaturation = m_colorSaturation_max;
        if (m_colorSaturation == colorSaturation)
            return;

        m_colorSaturation = colorSaturation;
        emit colorSaturationChanged(m_colorSaturation);
    }

    void setColorBrightness(int colorBrightness)
    {
        if(colorBrightness < m_colorBrightness_min)
            colorBrightness = m_colorBrightness_min;
        if(colorBrightness > m_colorBrightness_max)
            colorBrightness = m_colorBrightness_max;
        if (m_colorBrightness == colorBrightness)
            return;

        m_colorBrightness = colorBrightness;
        emit colorBrightnessChanged(m_colorBrightness);
    }

private:


    int m_index;

    int m_width;
    int m_width_min;
    int m_width_max;

    int m_colorHue;
    int m_colorHue_min;
    int m_colorHue_max;

    int m_colorSaturation;
    int m_colorSaturation_min;
    int m_colorSaturation_max;

    int m_colorBrightness;
    int m_colorBrightness_min;
    int m_colorBrightness_max;

signals:

void indexChanged(int index);
void widthChanged(int width);
void colorHueChanged(int colorHue);
void colorSaturationChanged(int colorSaturation);
void colorBrightnessChanged(int colorBrightness);
};

#endif // SUPERSOURCEBORDER_H
