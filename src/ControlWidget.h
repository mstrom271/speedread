#ifndef __CONTROLWIDGET_H__
#define __CONTROLWIDGET_H__

#include "Settings.h"
#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QSpinBox>
#include <QStackedLayout>
#include <QStyle>

class ControlWidget : public QWidget {
    Q_OBJECT
    // clang-format off
    QVBoxLayout *vLayout;
        QHBoxLayout *hLayout1;
            QPushButton *playBtn;
        QGridLayout *gridLayout;
            QLabel *bookPosLabel;
            QSlider *bookPosSlider;
            QLabel *bookWidthLabel;
            QSlider *bookWidthSlider;
            QLabel *fragSizeLabel;
            QSlider *fragSizeSlider;
            QLabel *speedLabel;
            QSlider *speedSlider;

            QPushButton *openBtn;
            QPushButton *bookPosBtn;
            QPushButton *bookWidthBtn;
            QPushButton *fragSizeBtn;
            QPushButton *speedBtn;
            QPushButton *settingsBtn;
    // clang-format on

    bool bookWidthSliderPressed = false;

  public:
    ControlWidget(QWidget *wgt = nullptr);
    int getSpeed();

  public slots:
    void setBookPosRangeSilent(int max);
    void setBookPosSilent(double newBookPos);
    void setBookPos(int newBookPos);

    void setBookWidthRangeSilent(std::pair<int, int> range);
    void setBookWidthSilent(int newBookWidth);
    void setBookWidthPress();
    void setBookWidthMove(int newBookWidth);
    void setBookWidthRelease();
    void setBookWidth(int newBookWidth);

    void setFragSizeRangeSilent(std::pair<int, int> range);
    void setFragSizeSilent(int newFragSize);
    void setFragSize(int newFragSize);

    void setSpeed(int newSpeed);

  protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

  signals:
    void updateBookPos(double);
    void updateBookWidth(int);
    void updateFragSize(int);
    void play();
    void showLastBooksWidget();
    void showSettingsWidget();
};

#endif
