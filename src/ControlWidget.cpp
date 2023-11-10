#include "ControlWidget.h"
#include "Language.h"

void ControlWidget::onThemeChange() {}

void ControlWidget::onLanguageChange() {
    playBtn->setText(tr("PlayBtn"));
    openBtn->setText(tr("Open"));
    bookPosBtn->setText(tr("Pos"));
    bookWidthBtn->setText(tr("Width"));
    fragSizeBtn->setText(tr("Frag"));
    speedBtn->setText(tr("Speed"));
    settingsBtn->setText(tr("Settings"));
}

ControlWidget::ControlWidget(QWidget *wgt) : QWidget(wgt) {
    // clang-format off
    vLayout = new QVBoxLayout;
        hLayout1 = new QHBoxLayout;
            playBtn = new QPushButton;
        gridLayout = new QGridLayout;
            bookPosLabel = new QLabel;
            bookWidthLabel = new QLabel;
            fragSizeLabel = new QLabel;
            speedLabel = new QLabel;

            bookPosSlider = new QSlider(Qt::Vertical);
            bookWidthSlider = new QSlider(Qt::Vertical);
            fragSizeSlider = new QSlider(Qt::Vertical);
            speedSlider = new QSlider(Qt::Vertical);

            openBtn = new QPushButton;
            bookPosBtn = new QPushButton;
            bookWidthBtn = new QPushButton;
            fragSizeBtn = new QPushButton;
            speedBtn = new QPushButton;
            settingsBtn = new QPushButton;

    this->setLayout(vLayout);
        vLayout->addStretch();
        vLayout->addLayout(hLayout1);
            hLayout1->addStretch();
            hLayout1->addWidget(playBtn);
            hLayout1->addStretch();
        vLayout->addStretch();
        vLayout->addLayout(gridLayout);
            gridLayout->addWidget(bookPosLabel, 0, 1, Qt::AlignHCenter);
            gridLayout->addWidget(bookWidthLabel, 0, 2, Qt::AlignHCenter);
            gridLayout->addWidget(fragSizeLabel, 0, 3, Qt::AlignHCenter);
            gridLayout->addWidget(speedLabel, 0, 4, Qt::AlignHCenter);

            gridLayout->addWidget(bookPosSlider, 1, 1, 1, 1, Qt::AlignHCenter);
            gridLayout->addWidget(bookWidthSlider, 1, 2, 1, 1, Qt::AlignHCenter);
            gridLayout->addWidget(fragSizeSlider, 1, 3, 1, 1, Qt::AlignHCenter);
            gridLayout->addWidget(speedSlider, 1, 4, 1, 1, Qt::AlignHCenter);

            gridLayout->addWidget(openBtn, 2, 0);
            gridLayout->addWidget(bookPosBtn, 2, 1);
            gridLayout->addWidget(bookWidthBtn, 2, 2);
            gridLayout->addWidget(fragSizeBtn, 2, 3);
            gridLayout->addWidget(speedBtn, 2, 4);
            gridLayout->addWidget(settingsBtn, 2, 5);

    vLayout->setContentsMargins(0, 0, 0, 0);
        //hLayout1
            connect(playBtn, SIGNAL(clicked()), this, SIGNAL(play()));
        gridLayout->setContentsMargins(0, 0, 0, 0);
            connect(bookPosSlider, SIGNAL(valueChanged(int)), this, SLOT(setBookPos(int)));
                bookPosSlider->setInvertedAppearance(true);
            connect(bookWidthSlider, SIGNAL(sliderPressed()), this, SLOT(setBookWidthPress()));
            connect(bookWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(setBookWidthMove(int)));
            connect(bookWidthSlider, SIGNAL(sliderReleased()), this, SLOT(setBookWidthRelease()));
            connect(bookWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(setBookWidth(int)));
            connect(fragSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setFragSize(int)));
            speedSlider->setRange(400, 5000);
                connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));
                speedSlider->setSliderPosition(Settings::getSpeed());

            connect(openBtn, SIGNAL(clicked()), this, SIGNAL(showLastBooksWidget()));
            connect(settingsBtn, SIGNAL(clicked()), this, SIGNAL(showSettingsWidget()));
    // clang-format on
    onLanguageChange();
}

int ControlWidget::getSpeed() { return speedSlider->sliderPosition(); }

// BookPos
void ControlWidget::setBookPosRangeSilent(int max) {
    bookPosSlider->blockSignals(true);
    bookPosSlider->setMinimum(0);
    bookPosSlider->setMaximum(max);
    bookPosSlider->blockSignals(false);
}

void ControlWidget::setBookPosSilent(double newBookPos) {
    bookPosSlider->blockSignals(true);
    bookPosSlider->setSliderPosition((int)newBookPos);
    bookPosLabel->setText(
        QString::number((int)bookPosSlider->sliderPosition()));
    bookPosSlider->blockSignals(false);
}

void ControlWidget::setBookPos(int newBookPos) {
    bookPosLabel->setText(QString::number(bookPosSlider->sliderPosition()));
    emit updateBookPos(newBookPos);
}

// BookWidth
void ControlWidget::setBookWidthRangeSilent(std::pair<int, int> range) {
    bookWidthSlider->blockSignals(true);
    bookWidthSlider->setMinimum(range.first);
    bookWidthSlider->setMaximum(range.second);
    bookWidthSlider->blockSignals(false);
}

void ControlWidget::setBookWidthSilent(int newBookWidth) {
    bookWidthSlider->blockSignals(true);
    bookWidthSlider->setSliderPosition(newBookWidth);
    bookWidthLabel->setText(QString::number(bookWidthSlider->sliderPosition()));
    bookWidthSlider->blockSignals(false);
}

void ControlWidget::setBookWidthPress() {
    bookWidthSliderPressed = true;
    bookWidthLabel->setText(QString::number(bookWidthSlider->sliderPosition()));
    update();
}

void ControlWidget::setBookWidthMove(int newBookWidth) {
    bookWidthLabel->setText(QString::number(bookWidthSlider->sliderPosition()));
    update();
}

void ControlWidget::setBookWidthRelease() {
    bookWidthLabel->setText(QString::number(bookWidthSlider->sliderPosition()));
    bookWidthSliderPressed = false;
    update();
    emit updateBookWidth(bookWidthSlider->sliderPosition());
}

void ControlWidget::setBookWidth(int newBookWidth) {
    bookWidthLabel->setText(QString::number(bookWidthSlider->sliderPosition()));
    if (!bookWidthSliderPressed) {
        update();
        emit updateBookWidth(bookWidthSlider->sliderPosition());
    }
}

// FragSize
void ControlWidget::setFragSizeRangeSilent(std::pair<int, int> range) {
    fragSizeSlider->blockSignals(true);
    fragSizeSlider->setMinimum(range.first);
    fragSizeSlider->setMaximum(range.second);
    fragSizeSlider->blockSignals(false);
}

void ControlWidget::setFragSizeSilent(int newFragSize) {
    fragSizeSlider->blockSignals(true);
    fragSizeSlider->setSliderPosition(newFragSize);
    fragSizeLabel->setText(QString::number(fragSizeSlider->sliderPosition()));
    fragSizeSlider->blockSignals(false);
}

void ControlWidget::setFragSize(int newFragSize) {
    fragSizeLabel->setText(QString::number(fragSizeSlider->sliderPosition()));
    emit updateFragSize(newFragSize);
}

// Speed
void ControlWidget::setSpeed(int newSpeed) {
    Settings::setSpeed(newSpeed);
    speedLabel->setText(QString::number(getSpeed()));
}

void ControlWidget::resizeEvent(QResizeEvent *event) {
    for (auto wgt : std::initializer_list<QSlider *>{
             bookPosSlider, bookWidthSlider, fragSizeSlider, speedSlider}) {

        wgt->setMinimumWidth(
            QApplication::primaryScreen()->physicalDotsPerInch() * 0.2);
        wgt->setMinimumHeight(
            std::min(QApplication::primaryScreen()->size().height(), height()) /
            3);

        double wgt_size = 1;
        QString styleQSlider;
        QString slider_fullsize =
            QString::number(static_cast<int>(wgt->width() * wgt_size));
        QString slider_halfsize =
            QString::number(static_cast<int>(wgt->width() * (wgt_size / 2)));
        QString slider_quartsize =
            QString::number(static_cast<int>(wgt->width() * (wgt_size / 4)));

        styleQSlider =
            "\
            QSlider::groove:vertical {\
                background: #393939;\
                position: absolute; /* absolutely position 4px from the left and right of the widget. setting margins on the widget should work too... */\
                left: 4px; right: 4px;\
            }\
            \
            QSlider::handle:vertical {\
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);;\
                border: 1px solid #8f8f8f;\
                height: " +
            slider_fullsize + "px;\
                border-radius: " +
            slider_halfsize + "px;\
                margin: 0px -" +
            slider_quartsize +
            "px; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\
            }\
            \
            QSlider::add-page:vertical {\
                background: #393939;\
            }\
            \
            QSlider::sub-page:vertical {\
                background: #393939;\
            }\
            ";

        wgt->setStyleSheet(styleQSlider);
    }

    playBtn->setFixedHeight(200);

    for (auto wgt : std::initializer_list<QPushButton *>{
             playBtn, openBtn, bookPosBtn, bookWidthBtn, fragSizeBtn, speedBtn,
             settingsBtn}) {
        wgt->setContentsMargins(0, 0, 0, 0);
        wgt->setMaximumWidth(
            std::min(QApplication::primaryScreen()->size().width(), width()) /
            7);
        wgt->setMinimumHeight(
            QApplication::primaryScreen()->physicalDotsPerInch() * 0.3);
    }
    QWidget::resizeEvent(event);
}

void ControlWidget::paintEvent(QPaintEvent *event) {
    if (bookWidthSliderPressed) {
        QPainter painter(this);
        int indent = (width() - bookWidthSlider->sliderPosition()) / 2;
        painter.setPen(Qt::white);
        painter.drawLine(indent, 0, indent, height());
        painter.drawLine(width() - indent - 1, 0, width() - indent - 1,
                         height());
    }
    QWidget::paintEvent(event);
}

bool ControlWidget::event(QEvent *event) {
    /* if (event->type() == ThemeChangeEvent::type){
        onThemeChange();
        return true;
    }else  */
    if (event->type() == LanguageChangeEvent::type) {
        onLanguageChange();
        return true;
    }

    return QWidget::event(event);
}