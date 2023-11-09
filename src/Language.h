#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include <QEvent>
#include <QTranslator>

class LanguageChangeEvent : public QEvent {
  public:
    static const QEvent::Type type =
        static_cast<QEvent::Type>(QEvent::User + 201);
    LanguageChangeEvent() : QEvent(static_cast<QEvent::Type>(type)) {}
};

class Language {
    QTranslator translator;

    Language();
    Language(const Language &other) = delete;
    Language &operator=(const Language &other) = delete;

  public:
    static Language &getInstance();

    static void setLanguage(QString newLanguage);
    static QString getLanguage();
    static QString getSystemLanguage();
    static void notifyAll();

  private:
    static QString correct(QString language);
};

#endif