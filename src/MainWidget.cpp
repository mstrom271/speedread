#include "MainWidget.h"

MainWidget::MainWidget(QWidget *wgt) : QWidget(wgt) {
    // clang-format off
    stackedLayout = new QStackedLayout();
        bookWidget = new BookWidget();
        controlWidget = new ControlWidget();
        lastBooksWidget = new LastBooksWidget();


    this->setLayout(stackedLayout);
        stackedLayout->addWidget(bookWidget);
        stackedLayout->addWidget(controlWidget);
        stackedLayout->addWidget(lastBooksWidget);
    // clang-format on

    connect(controlWidget, SIGNAL(play()), this, SLOT(play()));
    connect(bookWidget, SIGNAL(clicked()), this, SLOT(pause()));

    connect(bookWidget, SIGNAL(updateBookPosRange(int)), controlWidget,
            SLOT(setBookPosRangeSilent(int)));
    connect(bookWidget, SIGNAL(updateBookPos(double)), controlWidget,
            SLOT(setBookPosSilent(double)));
    connect(controlWidget, SIGNAL(updateBookPos(double)), bookWidget,
            SLOT(setBookPos(double)));

    connect(bookWidget, SIGNAL(updateBookWidthRange(std::pair<int, int>)),
            controlWidget, SLOT(setBookWidthRangeSilent(std::pair<int, int>)));
    connect(bookWidget, SIGNAL(updateBookWidth(int)), controlWidget,
            SLOT(setBookWidthSilent(int)));
    connect(controlWidget, SIGNAL(updateBookWidth(int)), bookWidget,
            SLOT(setBookWidth(int)));

    connect(bookWidget, SIGNAL(updateFragSizeRange(std::pair<int, int>)),
            controlWidget, SLOT(setFragSizeRangeSilent(std::pair<int, int>)));
    connect(bookWidget, SIGNAL(updateFragSize(int)), controlWidget,
            SLOT(setFragSizeSilent(int)));
    connect(controlWidget, SIGNAL(updateFragSize(int)), bookWidget,
            SLOT(setFragSize(int)));

    connect(controlWidget, SIGNAL(showLastBooksWidget()), this,
            SLOT(showLastBooksWidget()));
    connect(lastBooksWidget, SIGNAL(loadBook(const QString &)), bookWidget,
            SLOT(loadBook(const QString &)));
    connect(lastBooksWidget, SIGNAL(showBookWidget()), this,
            SLOT(showBookWidget()));

    stackedLayout->setContentsMargins(0, 0, 0, 0);
    stackedLayout->setStackingMode(QStackedLayout::StackingMode::StackAll);
    stackedLayout->setCurrentWidget(controlWidget);
    lastBooksWidget->hide();
    bookWidget->init();
    resize(Settings::getWindowWidth(), Settings::getWindowHeight());

    // hack, to get proper initial DPI on android
    QTimer *oneShotTimer = new QTimer(this);
    oneShotTimer->singleShot(500, []() {
        for (auto wgt : QApplication::allWidgets()) {
            wgt->resize(wgt->size() + QSize(1, 1));
        }
    });
}

void MainWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QCoreApplication::sendEvent(bookWidget, event);
        dragStart = true;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragStart == true)
        QCoreApplication::sendEvent(bookWidget, event);
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event) { dragStart = false; }

void MainWidget::resizeEvent(QResizeEvent *event) {
    Settings::setWindowWidth(event->size().width());
    Settings::setWindowHeight(event->size().height());

    QWidget::resizeEvent(event);
}

void MainWidget::play() {
    if (!playInProcess) {
        playInProcess = true;
        auto fragSize = bookWidget->getCurrentFragSize();

        if (!fragSize) {
            bookWidget->next();
            fragSize = bookWidget->getCurrentFragSize();
        }

        if (fragSize) {
            timer.singleShot(fragSize * 1000 * 60 / controlWidget->getSpeed(),
                             this, SLOT(next()));
            controlWidget->hide();
        } else
            pause();
    }
}

void MainWidget::next() {
    if (playInProcess) {
        bookWidget->next();
        auto fragSize = bookWidget->getCurrentFragSize();
        if (fragSize)
            timer.singleShot(fragSize * 1000 * 60 / controlWidget->getSpeed(),
                             this, SLOT(next()));
        else
            pause();
    }
}

void MainWidget::pause() {
    playInProcess = false;
    controlWidget->show();
}

void MainWidget::showLastBooksWidget() {
    bookWidget->hide();
    controlWidget->hide();
    lastBooksWidget->show();
}

void MainWidget::showBookWidget() {
    lastBooksWidget->hide();
    bookWidget->show();
    controlWidget->show();
}