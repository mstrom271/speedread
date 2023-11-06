#include "Settings.h"
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