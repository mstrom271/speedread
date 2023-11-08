#ifndef __SETTINGSWIDGET_H__
#define __SETTINGSWIDGET_H__

#include "Settings.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

class SettingsWidget : public QWidget {
    Q_OBJECT
    // clang-format off
    QVBoxLayout *vLayout;
        QScrollArea *scrollArea;
            QWidget *scrollWidget;
                QVBoxLayout *scrollLayout;
                    QHBoxLayout *hLayout1;
                        QLabel *lbl_theme;
                        QLabel *lbl_theme_detailed;
                        QComboBox *cmb_theme;
                    QHBoxLayout *hLayout2;
                        QLabel *lbl_language;
                        QLabel *lbl_language_detailed;
                        QComboBox *cmb_language;
                    QHBoxLayout *hLayout3;
                        QLabel *lbl_font;
                        QLabel *lbl_font_detailed;
                        QComboBox *cmb_font;
                    QHBoxLayout *hLayout4;
                        QLabel *lbl_keepAwake;
                        QLabel *lbl_keepAwake_detailed;
                        QCheckBox *ckb_keepAwake;
        QPushButton *okBtn;
    // clang-format on

  public:
    SettingsWidget(QWidget *wgt = nullptr);

  public slots:
  protected:
  signals:
    void showBookWidget();
};

#endif
