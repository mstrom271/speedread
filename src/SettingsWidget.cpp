#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget *wgt) {
    // clang-format off
    vLayout = new QVBoxLayout;
        scrollArea = new QScrollArea;
            scrollWidget = new QWidget;
                scrollLayout = new QVBoxLayout;
                    hLayout1 = new QHBoxLayout;
                        lbl_theme = new QLabel;
                        lbl_theme_detailed = new QLabel;
                        cmb_theme = new QComboBox;
                    hLayout2 = new QHBoxLayout;
                        lbl_language = new QLabel;
                        lbl_language_detailed = new QLabel;
                        cmb_language = new QComboBox;
                    hLayout3 = new QHBoxLayout;
                        lbl_font = new QLabel;
                        lbl_font_detailed = new QLabel;
                        cmb_font = new QComboBox;
                    hLayout4 = new QHBoxLayout;
                        lbl_keepAwake = new QLabel;
                        lbl_keepAwake_detailed = new QLabel;
                        ckb_keepAwake = new QCheckBox;
        okBtn = new QPushButton("ok");

    this->setLayout(vLayout);
        vLayout->addWidget(scrollArea);
            scrollArea->setWidget(scrollWidget);
                scrollWidget->setLayout(scrollLayout);
                    scrollLayout->addLayout(hLayout1);
                        hLayout1->addWidget(lbl_theme);
                        hLayout1->addWidget(lbl_theme_detailed);
                        hLayout1->addWidget(cmb_theme);
                    scrollLayout->addLayout(hLayout2);
                        hLayout2->addWidget(lbl_language);
                        hLayout2->addWidget(lbl_language_detailed);
                        hLayout2->addWidget(cmb_language);
                    scrollLayout->addLayout(hLayout3);
                        hLayout3->addWidget(lbl_font);
                        hLayout3->addWidget(lbl_font_detailed);
                        hLayout3->addWidget(cmb_font);
                    scrollLayout->addLayout(hLayout4);
                        hLayout4->addWidget(lbl_keepAwake);
                        hLayout4->addWidget(lbl_keepAwake_detailed);
                        hLayout4->addWidget(ckb_keepAwake);
        vLayout->addWidget(okBtn);
    // clang-format on

    connect(okBtn, SIGNAL(clicked()), this, SIGNAL(showBookWidget()));
}