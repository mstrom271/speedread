#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QByteArray>
#include <QFont>
#include <QSettings>
#include <QString>

QByteArray serializeFontToByteArray(const QFont &font);
QFont deserializeFontFromByteArray(QByteArray byteArray);

class Settings {
    QSettings settings;

    bool firstRun;
    uint windowPosX, windowPosY;
    uint windowWidth, windowHeight;
    QFont font;
    uint bookWidth;
    uint fragSize;
    uint speed;

    Settings() {
        firstRun = settings.value("/firstrun", true).toBool();
        if (firstRun)
            settings.setValue("/firstrun", false);

        windowWidth = settings.value("/windowWidth", 400).toUInt();
        windowHeight = settings.value("/windowHeight", 600).toUInt();
        font = deserializeFontFromByteArray(
            settings
                .value("/font", serializeFontToByteArray(
                                    QFont("Arial", 20, QFont::Normal)))
                .toByteArray());

        bookWidth = settings.value("/bookWidth", 0).toUInt();
        fragSize = settings.value("/fragSize", 50).toUInt();
        speed = settings.value("/speed", 1500).toUInt();
    }

    Settings(const Settings &other) = delete;
    Settings &operator=(const Settings &other) = delete;

  public:
    static Settings &getInstance() {
        static Settings instance;
        return instance;
    }

    static bool getFirstRun() { return Settings::getInstance().firstRun; }

    static uint getWindowWidth() { return Settings::getInstance().windowWidth; }
    static void setWindowWidth(uint newWidth) {
        Settings::getInstance().settings.setValue("/windowWidth", newWidth);
        Settings::getInstance().windowWidth = newWidth;
    }

    static uint getWindowHeight() {
        return Settings::getInstance().windowHeight;
    }
    static void setWindowHeight(uint newHeight) {
        Settings::getInstance().settings.setValue("/windowHeight", newHeight);
        Settings::getInstance().windowHeight = newHeight;
    }

    static QFont getFont() { return Settings::getInstance().font; }
    static void setFont(QFont newFont) {
        Settings::getInstance().settings.setValue(
            "/font", serializeFontToByteArray(newFont));
        Settings::getInstance().font = newFont;
    }

    static uint getBookWidth() { return Settings::getInstance().bookWidth; }
    static void setBookWidth(uint newBookWidth) {
        Settings::getInstance().settings.setValue("/bookWidth", newBookWidth);
        Settings::getInstance().bookWidth = newBookWidth;
    }

    static uint getFragSize() { return Settings::getInstance().fragSize; }
    static void setFragSize(uint newFragSize) {
        Settings::getInstance().settings.setValue("/fragSize", newFragSize);
        Settings::getInstance().fragSize = newFragSize;
    }

    static uint getSpeed() { return Settings::getInstance().speed; }
    static void setSpeed(uint newSpeed) {
        Settings::getInstance().settings.setValue("/speed", newSpeed);
        Settings::getInstance().speed = newSpeed;
    }

    static uint getBookPosChar(uint hash) {
        QString key = "/bookHash/" + QString::number(hash);
        return Settings::getInstance().settings.value(key, 0).toUInt();
    }
    static void setBookPosChar(uint hash, uint newBookPosChar) {
        QString key = "/bookHash/" + QString::number(hash);
        Settings::getInstance().settings.setValue(key, newBookPosChar);
    }

    static QString getLastBookFile() {
        if (Settings::getFirstRun())
            return ":/rcc/book.txt";
        else
            return Settings::getInstance()
                .settings.value("/lastBookFile", 0)
                .toString();
    }
    static void setLastBookFile(const QString &filename) {
        Settings::getInstance().settings.setValue("/lastBookFile", filename);

        auto list = getLastBooksList();
        auto index = list.indexOf(filename);
        list.removeAll(filename);
        list.append(filename);
        setLastBooksList(list);
    }

    static QStringList getLastBooksList() {
        return Settings::getInstance()
            .settings.value("/lastBooksList", 0)
            .toStringList();
    }
    static void setLastBooksList(const QStringList &list) {
        Settings::getInstance().settings.setValue("/lastBooksList", list);
    }
};

#endif