#include "Settings.h"
#include "Language.h"
#include <QDataStream>
#include <QIODevice>

QByteArray serializeFontToByteArray(const QFont &font) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << font;
    return byteArray;
}

QFont deserializeFontFromByteArray(QByteArray byteArray) {
    QDataStream stream(&byteArray, QIODevice::ReadOnly);
    QFont font;
    stream >> font;
    return font;
}

Settings::Settings() {
    firstRun = settings.value("/firstrun", true).toBool();
    if (firstRun)
        settings.setValue("/firstrun", false);

    windowWidth = settings.value("/windowWidth", 400).toUInt();
    windowHeight = settings.value("/windowHeight", 600).toUInt();
    font = deserializeFontFromByteArray(
        settings
            .value("/font",
                   serializeFontToByteArray(QFont("Arial", 20, QFont::Normal)))
            .toByteArray());
    if (firstRun)
        language = Language::getSystemLanguage();
    else
        language = settings.value("/language", "en").toString();

    bookWidth = settings.value("/bookWidth", 0).toUInt();
    fragSize = settings.value("/fragSize", 50).toUInt();
    speed = settings.value("/speed", 1500).toUInt();
}

Settings &Settings::getInstance() {
    static Settings instance;
    return instance;
}

bool Settings::getFirstRun() { return Settings::getInstance().firstRun; }

uint Settings::getWindowWidth() { return Settings::getInstance().windowWidth; }
void Settings::setWindowWidth(uint newWidth) {
    Settings::getInstance().settings.setValue("/windowWidth", newWidth);
    Settings::getInstance().windowWidth = newWidth;
}

uint Settings::getWindowHeight() {
    return Settings::getInstance().windowHeight;
}
void Settings::setWindowHeight(uint newHeight) {
    Settings::getInstance().settings.setValue("/windowHeight", newHeight);
    Settings::getInstance().windowHeight = newHeight;
}

QFont Settings::getFont() { return Settings::getInstance().font; }
void Settings::setFont(QFont newFont) {
    Settings::getInstance().settings.setValue(
        "/font", serializeFontToByteArray(newFont));
    Settings::getInstance().font = newFont;
}

QString Settings::getLanguage() { return Settings::getInstance().language; }
void Settings::setLanguage(QString newLang) {
    Settings::getInstance().settings.setValue("/language", newLang);
    Settings::getInstance().language = newLang;
}

uint Settings::getBookWidth() { return Settings::getInstance().bookWidth; }
void Settings::setBookWidth(uint newBookWidth) {
    Settings::getInstance().settings.setValue("/bookWidth", newBookWidth);
    Settings::getInstance().bookWidth = newBookWidth;
}

uint Settings::getFragSize() { return Settings::getInstance().fragSize; }
void Settings::setFragSize(uint newFragSize) {
    Settings::getInstance().settings.setValue("/fragSize", newFragSize);
    Settings::getInstance().fragSize = newFragSize;
}

uint Settings::getSpeed() { return Settings::getInstance().speed; }
void Settings::setSpeed(uint newSpeed) {
    Settings::getInstance().settings.setValue("/speed", newSpeed);
    Settings::getInstance().speed = newSpeed;
}

uint Settings::getBookPosChar(uint hash) {
    QString key = "/bookHash/" + QString::number(hash);
    return Settings::getInstance().settings.value(key, 0).toUInt();
}
void Settings::setBookPosChar(uint hash, uint newBookPosChar) {
    QString key = "/bookHash/" + QString::number(hash);
    Settings::getInstance().settings.setValue(key, newBookPosChar);
}

QString Settings::getLastBookFile() {
    if (Settings::getFirstRun())
        return ":/rcc/book.txt";
    else
        return Settings::getInstance()
            .settings.value("/lastBookFile", 0)
            .toString();
}
void Settings::setLastBookFile(const QString &filename) {
    Settings::getInstance().settings.setValue("/lastBookFile", filename);

    auto list = getLastBooksList();
    auto index = list.indexOf(filename);
    list.removeAll(filename);
    list.append(filename);
    setLastBooksList(list);
}

QStringList Settings::getLastBooksList() {
    return Settings::getInstance()
        .settings.value("/lastBooksList", 0)
        .toStringList();
}

void Settings::setLastBooksList(const QStringList &list) {
    Settings::getInstance().settings.setValue("/lastBooksList", list);
}