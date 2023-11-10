#ifndef __LASTBOOKSWIDGET_H__
#define __LASTBOOKSWIDGET_H__

#include "Settings.h"
#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QWidget>

class LastBooksWidget : public QWidget {
    Q_OBJECT
    // clang-format off
    QVBoxLayout *vLayout;
        QListWidget *booksListWidget;
        QHBoxLayout *hLayout;
            QPushButton *openBookBtn;
            QPushButton *cancelBtn;
    // clang-format on
    void onThemeChange();
    void onLanguageChange();

  public:
    LastBooksWidget(QWidget *wgt = nullptr);

  public slots:
    void itemSelected(QListWidgetItem *item);
    void bookSelectionDialog();

  protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual bool event(QEvent *event) override;

  signals:
    void loadBook(const QString &filename);
    void showBookWidget();
};

#endif