#include "Language.h"
#include "Settings.h"
#include <QApplication>
#include <QWidget>

Language::Language() {}

Language &Language::getInstance() {
    static Language instance;
    return instance;
}

// notify all widgets to change a language
void Language::notifyAll() {
    QApplication::instance()->installTranslator(
        &Language::getInstance().translator);
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (auto widget : allWidgets) {
        QApplication::instance()->postEvent(widget, new LanguageChangeEvent);
    }
}

// load translations for relative languages
void Language::setLanguage(QString lang) {
    Language::getInstance().translator.load(":/main_" + correct(lang) + ".qm");
    Settings::setLanguage(lang);
}

QString Language::getLanguage() { return Settings::getLanguage(); }

QString Language::getSystemLanguage() {
    QString lang;
    switch (QLocale::system().language()) {
    case QLocale::Russian:
        lang = "ru";
        break;
    case QLocale::English:
        lang = "en";
        break;
    default:
        lang = "en";
        break;
    }

    return correct(lang);
}

// check if lang is allowed. Return default, if lang is incorrect
QString Language::correct(QString lang) {
    if (lang != "ru" && lang != "en")
        lang = "en";
    return lang;
}
