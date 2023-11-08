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

    Settings();
    Settings(const Settings &other) = delete;
    Settings &operator=(const Settings &other) = delete;

  public:
    static Settings &getInstance();
    static bool getFirstRun();
    static uint getWindowWidth();
    static void setWindowWidth(uint newWidth);

    static uint getWindowHeight();
    static void setWindowHeight(uint newHeight);

    static QFont getFont();
    static void setFont(QFont newFont);

    static uint getBookWidth();
    static void setBookWidth(uint newBookWidth);

    static uint getFragSize();
    static void setFragSize(uint newFragSize);

    static uint getSpeed();
    static void setSpeed(uint newSpeed);

    static uint getBookPosChar(uint hash);
    static void setBookPosChar(uint hash, uint newBookPosChar);

    static QString getLastBookFile();
    static void setLastBookFile(const QString &filename);

    static QStringList getLastBooksList();
    static void setLastBooksList(const QStringList &list);
};

#endif