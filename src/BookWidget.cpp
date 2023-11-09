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

BookWidget::BookWidget(QWidget *wgt) : QWidget(wgt) {}

void BookWidget::init() {
    setBookWidth(Settings::getBookWidth());
    setFragSize(Settings::getFragSize());
    loadBook(Settings::getLastBookFile());
}

void BookWidget::setFont(QFont newFont) {
    Settings::setFont(newFont);
    font = newFont;
    fm_cache = QFontMetrics(font);
    width_cache.clear();
    rows.clear();
    markupType = MarkupType::NoMarkup;
}

void BookWidget::breakdown() {
    if (width_cache.empty()) {
        width_cache.reserve(book.size());
        for (auto ch : book)
            width_cache.push_back(fm_cache.horizontalAdvance(ch));
    }

    rows.clear();
    uint rows_width = getBookWidth();
    book_row current_row{0, 0, false};
    uint i = 0;
    while (i <= book.size()) {
        if (i == book.size() || book[i] == '\n') {
            current_row.end = i;
            rows.push_back(current_row);
            current_row.begin = current_row.end = i + 1;
        } else if (book[i].isSpace()) {
            if (current_row.begin == i)
                current_row.begin = current_row.end = i + 1;
            else
                current_row.end = i;
        } else if (std::accumulate(width_cache.begin() + current_row.begin,
                                   width_cache.begin() + i + 1,
                                   0) > rows_width) {
            if (current_row.begin == current_row.end) { // half word
                current_row.end = i + 1;
                rows.push_back(current_row);
                current_row.begin = current_row.end;
            } else { // whole words
                rows.push_back(current_row);
                current_row.begin = ++current_row.end;
            }
        }
        i++;
    }
    markupType = MarkupType::NoHyphen;

    auto it = std::lower_bound(
        rows.begin(), rows.end(), bookPosChar,
        [](const book_row &row, int value) { return row.begin <= value; });
    bookPos = std::distance(rows.begin(), it) - 1;

    emit updateBookPosRange(getBookPosRange());
    emit updateBookPos(getBookPos());
}

QVector<QSet<QChar>> markerTable{
    {'.', '!', '?', ';', ',', ':', ' ', '\t', '\n'},
    {' ', '\t'},
    {',', ':'},
    {'.', '!', '?', ';'},
    {'\n'},
};
void BookWidget::mapBook(unsigned char level, uint start, uint length) {
    bookMap.resize(book.size(), 0);
    if (level == 0)
        return;

    uint pos = start;
    uint node_end = start + length;
    uint num_chars = 0;
    while (pos != node_end) {
        if (!markerTable[0].contains(book[pos]))
            num_chars++;

        if (markerTable[level].contains(book[pos++])) {
            while (pos != node_end && markerTable[0].contains(book[pos]))
                pos++;
            if (num_chars) {
                bookMap[pos - 1] = std::max(bookMap[pos - 1], level);
                mapBook(level - 1, start, pos - start);
                start = pos;
                num_chars = 0;
            }
        }
    }
    if (num_chars)
        mapBook(level - 1, start, pos - start);
}

uint BookWidget::nextFragment(uint start, uint length) {
    if (length == 0)
        return start;

    uint pos = start;
    char level = 0;
    for (uint i = start; (i < start + length) && (i < bookMap.size()); ++i) {
        if (level == bookMap[i])
            pos = i;
        else if (level < bookMap[i]) {
            level = bookMap[i];
            pos = i;
        }
    }
    // TODO: check whether a fragment fit a screen of a book
    return std::min(pos + 1, (uint)book.size());
}

uint BookWidget::fragmentSize(uint start, uint end) {
    uint count = 0;
    for (uint i = start; i < end; ++i)
        if (!markerTable[0].contains(book[i]))
            count++;

    return count;
}

uint BookWidget::getCurrentFragSize() {
    return fragmentSize(fragStart, fragEnd);
}

double BookWidget::getBookPos() { return bookPos; }

double BookWidget::getBookPosRange() { return rows.size() - 1; }

int BookWidget::getBookWidth() {
    if (bookWidth == 0)
        return width();
    else
        return std::min(bookWidth, width());
}

std::pair<int, int> BookWidget::getBookWidthRange() {
    return std::make_pair(minWidth, width());
}

int BookWidget::getFragSize() { return fragSize; }

std::pair<int, int> BookWidget::getFragSizeRange() {
    return std::make_pair(10, 500);
}

bool BookWidget::loadBook(const QString &filename) {
    if (filename.isEmpty())
        return false;

    book.clear();
    bookMap.clear();
    width_cache.clear();
    rows.clear();
    markupType = MarkupType::NoMarkup;
    bookPos = 0;
    bookPosChar = 0;
    fragStart = fragEnd = 0;

    QFileInfo fileInfo(filename);
    QString fileExtension = fileInfo.suffix();

    bool result = true;
    if (fileInfo.suffix() == "txt")
        result = loadBookTXT(filename, book);
    else if (fileInfo.suffix() == "fb2")
        result = loadBookFB2(filename, book);
    else
        result = false;

    if (result) {
        Settings::setLastBookFile(filename);
        bookHash = qHash(book);
        bookPosChar = Settings::getBookPosChar(bookHash);
        breakdown();
        mapBook(4, 0, book.size());

        auto it = std::lower_bound(
            rows.begin(), rows.end(), bookPosChar,
            [](const book_row &row, int value) { return row.begin <= value; });
        bookPos = std::distance(rows.begin(), it) - 1;
        fragStart = fragEnd = rows[std::ceil(bookPos)].begin;

        emit updateBookWidthRange(getBookWidthRange());
        emit updateBookWidth(getBookWidth());
        emit updateBookPosRange(getBookPosRange());
        emit updateBookPos(getBookPos());
        emit updateFragSizeRange(getFragSizeRange());
        emit updateFragSize(getFragSize());
    }

    update();
    return result;
}

void BookWidget::setBookPos(double newBookPos) {
    bookPos = std::clamp(newBookPos, 0.0, getBookPosRange());

    fragStart = bookPosChar = rows[ceil(bookPos)].begin;
    fragEnd = nextFragment(fragStart, fragSize);
    Settings::setBookPosChar(bookHash, bookPosChar);

    update();
    emit updateBookPos(getBookPos());
}

void BookWidget::setBookWidth(int newBookWidth) {
    Settings::setBookWidth(newBookWidth);
    bookWidth = newBookWidth;
    breakdown();
    update();
    emit updateBookWidth(getBookWidth());
}

void BookWidget::setFragSize(int newFragSize) {
    auto range = getFragSizeRange();
    fragSize = std::clamp(newFragSize, range.first, range.second);
    Settings::setFragSize(fragSize);

    fragStart = bookPosChar = rows[ceil(bookPos)].begin;
    fragEnd = nextFragment(fragStart, fragSize);

    update();
    emit updateFragSize(getFragSize());
}

void BookWidget::next() {
    fragStart = fragEnd;
    fragEnd = nextFragment(fragStart, fragSize);

    int height = std::max(this->height(), minHeight);
    int num = height / fm_cache.lineSpacing();
    int screen_begin = rows[ceil(bookPos)].begin;
    int screen_end =
        rows[std::min((uint)floor(bookPos) + num - 1, (uint)rows.size() - 1)]
            .end;

    if (fragEnd > screen_end) {
        auto it = std::lower_bound(
            rows.begin(), rows.end(), fragStart,
            [](const book_row &row, int value) { return row.begin <= value; });
        bookPos = std::distance(rows.begin(), it) - 1;
        bookPosChar = rows[ceil(bookPos)].begin;
        Settings::setBookPosChar(bookHash, bookPosChar);
    }

    emit updateBookPos(bookPos);
    // qDebug() << book.mid(fragStart, fragEnd-fragStart) << bookPos <<
    // rows[bookPos].begin << fragStart;
    update();
}

void BookWidget::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    if (markupType == MarkupType::NoMarkup)
        breakdown();

    int height = std::max(this->height(), minHeight);
    int str_begin = std::max(this->width(), minWidth) / 2 - getBookWidth() / 2;

    painter.setFont(font);

    int curr_line = floor(bookPos);
    double top_rel_y = floor(bookPos) - bookPos;
    double curr_rel_y = 0;

    const uint spaceWidth = fm_cache.horizontalAdvance(' ');
    const uint lineSpacing = fm_cache.lineSpacing();
    const uint ascent = fm_cache.ascent();
    // qDebug() << "------------";
    while (curr_rel_y <= height + lineSpacing && curr_line < rows.size()) {
        QString str = book.mid(rows[curr_line].begin,
                               rows[curr_line].end - rows[curr_line].begin);
        QVector<int> str_width_cache = width_cache.mid(
            rows[curr_line].begin, rows[curr_line].end - rows[curr_line].begin);

        uint wideSpaceWidth = 0;
        if (curr_line + 1 == rows.size() ||
            rows[curr_line + 1].begin == rows[curr_line + 1].end)
            wideSpaceWidth = spaceWidth;
        else {
            uint spaceNum = str.count(' ');
            uint strWidth = std::accumulate(str_width_cache.begin(),
                                            str_width_cache.end(), 0);
            if (spaceNum)
                wideSpaceWidth =
                    (getBookWidth() - (strWidth - spaceWidth * spaceNum)) /
                    spaceNum;
        }
        int str_advance = 0;
        for (int i = 0; i < str.length(); i++) {
            if (rows[curr_line].begin + i >= fragStart &&
                rows[curr_line].begin + i < fragEnd)
                painter.setPen(QColor(255, 255, 255));
            else
                painter.setPen(QColor(127, 127, 127));

            if (str[i] == ' ')
                str_advance += wideSpaceWidth;
            else {
                painter.drawText(str_begin + str_advance,
                                 top_rel_y * lineSpacing + ascent + curr_rel_y,
                                 str[i]);
                str_advance += str_width_cache[i];
            }
        }
        // painter.drawText(x,
        // y+top_rel_y*fm.lineSpacing()+fm.ascent()+curr_rel_y,
        // book.mid(rows[curr_line].begin,
        // rows[curr_line].end-rows[curr_line].begin)); qDebug() <<
        // book.mid(rows[curr_line].begin,
        // rows[curr_line].end-rows[curr_line].begin); qDebug() <<
        // bookMap.mid(rows[curr_line].begin,
        // rows[curr_line].end-rows[curr_line].begin);

        curr_rel_y += lineSpacing;
        curr_line++;
    }
}

void BookWidget::resizeEvent(QResizeEvent *event) {
    markupType = MarkupType::NoMarkup;
    emit updateBookWidthRange(getBookWidthRange());
    emit updateBookWidth(getBookWidth());
}

void BookWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->position();
        bookPosStartPosition = bookPos;
        emit clicked();
    }
}

void BookWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        double newLineNumber =
            bookPosStartPosition +
            ((double)(dragStartPosition - event->position()).ry()) /
                fm_cache.height();
        setBookPos(newLineNumber);
    }
}