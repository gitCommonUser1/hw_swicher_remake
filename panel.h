#ifndef PANEL_H
#define PANEL_H

#include <QObject>

class Panel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ButtonBrightness READ ButtonBrightness WRITE setButtonBrightness NOTIFY ButtonBrightnessChanged)
    int m_ButtonBrightness;
    int m_ButtonBrightness_max;
    int m_ButtonBrightness_min;

public:
    explicit Panel(QObject *parent = nullptr);

int ButtonBrightness() const
{
    return m_ButtonBrightness;
}

public slots:
void setButtonBrightness(int ButtonBrightness)
{
    if(ButtonBrightness > m_ButtonBrightness_max)
        ButtonBrightness = m_ButtonBrightness_max;
    if(ButtonBrightness < m_ButtonBrightness_min)
        ButtonBrightness = m_ButtonBrightness_min;

    if (m_ButtonBrightness == ButtonBrightness)
        return;

    m_ButtonBrightness = ButtonBrightness;
    emit ButtonBrightnessChanged(m_ButtonBrightness);
}

signals:

void ButtonBrightnessChanged(int ButtonBrightness);
};

#endif // PANEL_H
