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
    LastBooksWidget(QWidget *wgt = nullptr) : QWidget(wgt) {
        // clang-format off
        vLayout = new QVBoxLayout();
            booksListWidget = new QListWidget();
            hLayout = new QHBoxLayout();
                openBookBtn = new QPushButton("open");
                cancelBtn = new QPushButton("cancel");


        this->setLayout(vLayout);
            vLayout->addWidget(booksListWidget);
            vLayout->addLayout(hLayout);
                hLayout->addWidget(openBookBtn);
                hLayout->addWidget(cancelBtn);
        // clang-format on

        booksListWidget->setWordWrap(true);

        connect(openBookBtn, SIGNAL(clicked()), this,
                SLOT(bookSelectionDialog()));
        connect(cancelBtn, SIGNAL(clicked()), this, SIGNAL(showBookWidget()));
        connect(booksListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,
                SLOT(itemSelected(QListWidgetItem *)));
    }

  public slots:
    void itemSelected(QListWidgetItem *item) {
        QWidget *wgt =
            dynamic_cast<QWidget *>(booksListWidget->itemWidget(item));
        if (wgt)
            emit loadBook(wgt->property("fileName").value<QString>());
        emit showBookWidget();
    }

    void bookSelectionDialog() {
        QString fileName = QFileDialog::getOpenFileName(
            this, tr("Open Book"), "/storage/emulated/0/",
            tr("Books (*.fb2 *.xpm *.txt)"));
        if (!fileName.isEmpty()) {
            emit loadBook(fileName);
            emit showBookWidget();
        }
    }

  protected:
    void resizeEvent(QResizeEvent *event) override {
        for (auto wgt :
             std::initializer_list<QPushButton *>{openBookBtn, cancelBtn}) {
            wgt->setContentsMargins(0, 0, 0, 0);
            wgt->setMaximumWidth(
                std::min(QApplication::primaryScreen()->size().width(),
                         width()) /
                2);
            wgt->setMinimumHeight(
                QApplication::primaryScreen()->physicalDotsPerInch() * 0.3);
        }

        QWidget::resizeEvent(event);
    }

    void showEvent(QShowEvent *event) override {
        // clang-format off
        QListWidgetItem* listWidgetItem;
            QWidget *itemWgt;
                QVBoxLayout *itemLayout;
                    QLabel *topLabel;
                    QLabel *bottomLabel;
        // clang-format on

        booksListWidget->clear();
        auto fileList = Settings::getLastBooksList();
        while (!fileList.isEmpty()) {
            auto fileName = fileList.last();
            fileList.pop_back();

            listWidgetItem = new QListWidgetItem(booksListWidget);
            itemWgt = new QWidget();
            itemLayout = new QVBoxLayout();
            topLabel = new QLabel();
            bottomLabel = new QLabel();

            itemWgt->setLayout(itemLayout);
            itemLayout->addWidget(topLabel);
            itemLayout->addWidget(bottomLabel);

            itemWgt->setContentsMargins(0, 10, 0, 10);

            QFileInfo fileInfo(fileName);
            topLabel->setWordWrap(true);
            bottomLabel->setWordWrap(true);

            // hack, to force QLabel to wrap strings without delimiters
            auto hyphenString = [](const QString &str) {
                QString str_result;
                for (auto ch : str) {
                    str_result += ch;
                    str_result += QChar(0x00AD);
                }
                if (!str_result.isEmpty())
                    str_result.removeLast();

                return str_result;
            };
            topLabel->setText(
                hyphenString(fileInfo.baseName() + "." + fileInfo.suffix()));
            bottomLabel->setText(hyphenString(fileInfo.absoluteFilePath()));

            QFont font = QApplication::font();
            font.setPixelSize(
                QApplication::primaryScreen()->physicalDotsPerInch() * 0.13);
            font.setBold(true);
            topLabel->setFont(font);

            itemWgt->setStyleSheet(
                "background: qlineargradient(x1: 0.5, y1: 0, x2: 0.5, y2: 1, "
                "stop: 0 rgba(63, 77, 99, 0), "
                "stop: 0.2 rgba(63, 77, 99, 255), "
                "stop: 0.8 rgba(63, 77, 99, 255), "
                "stop: 1 rgba(63, 77, 99, 0));");
            topLabel->setAttribute(Qt::WA_TranslucentBackground);
            bottomLabel->setAttribute(Qt::WA_TranslucentBackground);

            itemWgt->setProperty("fileName", QVariant::fromValue(fileName));
            listWidgetItem->setSizeHint(itemWgt->sizeHint());
            booksListWidget->addItem(listWidgetItem);
            booksListWidget->setItemWidget(listWidgetItem, itemWgt);
        }

        QWidget::showEvent(event);
    }

  signals:
    void loadBook(const QString &filename);
    void showBookWidget();
};

#endif