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

  public:
    LastBooksWidget(QWidget *wgt = nullptr);

  public slots:
    void itemSelected(QListWidgetItem *item);
    void bookSelectionDialog();

  protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

  signals:
    void loadBook(const QString &filename);
    void showBookWidget();
};

#endif