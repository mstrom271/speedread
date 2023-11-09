#include "SettingsWidget.h"
#include "Settings.h"
#include <QFontDatabase>

SettingsWidget::SettingsWidget(QWidget *wgt) {
    // clang-format off
    vLayout = new QVBoxLayout;
        scrollArea = new QScrollArea;
            scrollWidget = new QWidget;
                scrollLayout = new QVBoxLayout;
                    vLayout1 = new QVBoxLayout;
                        lbl_theme = new QLabel;
                        lbl_theme_detailed = new QLabel;
                        cmb_theme = new QComboBox();
                    vLayout2 = new QVBoxLayout;
                        lbl_language = new QLabel;
                        lbl_language_detailed = new QLabel;
                        cmb_language = new QComboBox;
                    vLayout3 = new QVBoxLayout;
                        lbl_font = new QLabel;
                        lbl_font_detailed = new QLabel;
                        cmb_font_family = new QComboBox;
                        cmb_font_size = new QComboBox;
                    vLayout4 = new QVBoxLayout;
                        lbl_keepAwake = new QLabel;
                        lbl_keepAwake_detailed = new QLabel;
                        ckb_keepAwake = new QCheckBox;
        okBtn = new QPushButton("ok");

    this->setLayout(vLayout);
        vLayout->addWidget(scrollArea);
            scrollArea->setWidget(scrollWidget);
                scrollWidget->setLayout(scrollLayout);
                    scrollLayout->addLayout(vLayout1);
                        vLayout1->addWidget(lbl_theme);
                        vLayout1->addWidget(lbl_theme_detailed);
                        vLayout1->addWidget(cmb_theme);
                    scrollLayout->addLayout(vLayout2);
                        vLayout2->addWidget(lbl_language);
                        vLayout2->addWidget(lbl_language_detailed);
                        vLayout2->addWidget(cmb_language);
                    scrollLayout->addLayout(vLayout3);
                        vLayout3->addWidget(lbl_font);
                        vLayout3->addWidget(lbl_font_detailed);
                        vLayout3->addWidget(cmb_font_family);
                        vLayout3->addWidget(cmb_font_size);
                    scrollLayout->addLayout(vLayout4);
                        vLayout4->addWidget(lbl_keepAwake);
                        vLayout4->addWidget(lbl_keepAwake_detailed);
                        vLayout4->addWidget(ckb_keepAwake);
        vLayout->addWidget(okBtn);
    // clang-format on

    scrollArea->setWidgetResizable(true);

    lbl_theme->setText("theme");
    lbl_theme_detailed->setText("theme detailed");
    lbl_language->setText("language");
    lbl_language_detailed->setText("language detailed");
    lbl_font->setText("font");
    lbl_font_detailed->setText("font detailed");
    lbl_keepAwake->setText("keep awake");
    lbl_keepAwake_detailed->setText("keep awake detailed");
    ckb_keepAwake->setText("keep awake");

    cmb_font_family->addItems(QFontDatabase::families());
    cmb_font_family->setCurrentText(Settings::getFont().family());
    updateFontFamilySilent();

    cmb_theme->addItems({"Dark", "Grey", "Light"});
    cmb_language->addItems({"English", "Russian"});

    connect(okBtn, SIGNAL(clicked()), this, SIGNAL(showBookWidget()));
    connect(cmb_font_family, SIGNAL(textActivated(QString)),
            SLOT(updateFontFamily()));
    connect(cmb_font_size, SIGNAL(textActivated(QString)),
            SLOT(updateFontSize()));
}

void SettingsWidget::updateFontFamilySilent() {
    auto sizesInt = QFontDatabase::pointSizes(cmb_font_family->currentText());
    QStringList sizesString;
    for (auto size : sizesInt)
        sizesString.push_back(QString::number(size));
    cmb_font_size->clear();
    cmb_font_size->addItems(sizesString);
    cmb_font_size->setCurrentText(
        QString::number(Settings::getFont().pointSize()));
}

void SettingsWidget::updateFontFamily() {
    updateFontFamilySilent();
    QFont font = Settings::getFont();
    font.setFamily(cmb_font_family->currentText());
    font.setPointSize(cmb_font_size->currentText().toInt());
    emit setFont(font);
}

void SettingsWidget::updateFontSize() {
    QFont font = Settings::getFont();
    font.setFamily(cmb_font_family->currentText());
    font.setPointSize(cmb_font_size->currentText().toInt());
    emit setFont(font);
}