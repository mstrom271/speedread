#include "BookWidget.h"
#include <QXmlStreamReader>

bool loadBookTXT(const QString &filename, QString &book) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File openning error:" << file.errorString();
        return false;
    }

    QTextStream in(&file);
    in.setAutoDetectUnicode(true);
    book = in.readAll();
    if (in.status() != QTextStream::Ok) {
        qDebug() << "File read error:" << in.status();
        return false;
    }

    return true;
}

bool loadBookFB2(const QString &filename, QString &book) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File openning error:" << file.errorString();
        return false;
    }

    book.clear();
    QXmlStreamReader xmlReader(&file);
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (xmlReader.name().toString() == "p" ||
                xmlReader.name().toString() == "title") {
                QString text;
                while (!(xmlReader.isEndElement() &&
                         xmlReader.name().toString() == "p")) {
                    if (xmlReader.tokenType() == QXmlStreamReader::Characters) {
                        text += xmlReader.text().toString() + "\n\n";
                    }
                    xmlReader.readNext();
                }
                book += text;
            }
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "XML parsing error:" << xmlReader.errorString();
        return false;
    }

    return true;
}
