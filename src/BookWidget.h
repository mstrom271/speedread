#ifndef __BOOKWIDGET_H__
#define __BOOKWIDGET_H__

#include "Settings.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <QVector>
#include <QWidget>

struct book_row {
    uint begin;
    uint end;
    bool isHyphen;
};

enum class MarkupType {
    NoMarkup,
    NoHyphen,
    Hyphen,
};

bool loadBookTXT(const QString &filename, QString &book);
bool loadBookFB2(const QString &filename, QString &book);

class BookWidget : public QWidget {
    Q_OBJECT

    QString book;
    uint bookHash;
    QVector<unsigned char> bookMap;
    QFont font = Settings::getFont();
    QFontMetrics fm_cache = QFontMetrics(font);
    QVector<int> width_cache;
    QVector<book_row> rows;
    MarkupType markupType = MarkupType::NoMarkup;

    double bookPos;
    const double bookPosScale = 10.0;

    // drag
    uint bookPosChar;
    QPointF dragStartPosition;
    double bookPosStartPosition;

    int bookWidth;
    const int minWidth = 30;
    const int minHeight = 30;
    uint fragSize = 50;
    uint fragStart = 0;
    uint fragEnd = 0;

    void onThemeChange();
    void onLanguageChange();

  public:
    BookWidget(QWidget *wgt = nullptr);
    void init();
    void breakdown();

    void mapBook(unsigned char level, uint start, uint length);
    uint nextFragment(uint start, uint length);
    uint fragmentSize(uint start, uint end);
    uint getCurrentFragSize();

    double getBookPos();
    double getBookPosRange();

    int getBookWidth();
    std::pair<int, int> getBookWidthRange();

    int getFragSize();
    std::pair<int, int> getFragSizeRange();

  public slots:
    void setFont(QFont newFont);
    bool loadBook(const QString &filename);
    void setBookPos(double newBookPos);
    void setBookWidth(int newBookWidth);
    void setFragSize(int newFragSize);
    void next();

  protected:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool event(QEvent *event) override;

  signals:
    void updateBookPosRange(int max);
    void updateBookPos(double pos);
    void updateBookWidthRange(std::pair<int, int> range);
    void updateBookWidth(int newBookWidth);
    void updateFragSizeRange(std::pair<int, int> range);
    void updateFragSize(int newBookWidth);
    void clicked();
};

#endif