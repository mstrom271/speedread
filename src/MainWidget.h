#ifndef __MAINWIDGET_H__
#define __MAINWIDGET_H__

#include "BookWidget.h"
#include "ControlWidget.h"
#include "LastBooksWidget.h"
#include "Settings.h"
#include "SettingsWidget.h"
#include <QCoreApplication>
#include <QLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QTimer>

class MainWidget : public QWidget {
    Q_OBJECT
    // clang-format off
    QStackedLayout *stackedLayout;
        BookWidget *bookWidget;
        ControlWidget *controlWidget;
        LastBooksWidget *lastBooksWidget;
        SettingsWidget *settingsWidget;
    // clang-format on

    QTimer timer;
    bool playInProcess = false;
    bool dragStart = false;

    void onThemeChange();
    void onLanguageChange();

  public:
    MainWidget(QWidget *wgt = nullptr);

  public slots:
    void play();
    void next();
    void pause();
    void showLastBooksWidget();
    void showSettingsWidget();
    void showBookWidget();

  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool event(QEvent *event) override;

  signals:
};

#endif