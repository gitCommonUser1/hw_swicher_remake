#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>

class Language : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)
    int m_language;

public:
    explicit Language(QObject *parent = nullptr);

    enum LANGUAGE{
        ENGLISH = 0,
        CHINESE,
        MAX
    };

int language() const
{
    return m_language;
}

public slots:
void setLanguage(int language)
{
    if(language < 0)
        language = 0;
    if(language >= MAX)
        language = MAX - 1;

    if (m_language == language)
        return;

    m_language = language;
    emit languageChanged(m_language);
}

signals:

void languageChanged(int language);
};

#endif // LANGUAGE_H
