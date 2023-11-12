#include "Theme.h"
#include "Settings.h"
#include <QApplication>
#include <QWidget>

Theme::Theme() {}

Theme &Theme::getInstance() {
    static Theme instance;
    return instance;
}

// notify all widgets to change a theme
void Theme::notifyAll() {
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (auto widget : allWidgets) {
        QApplication::instance()->postEvent(widget, new ThemeChangeEvent);
    }
}

// load theme
void Theme::applyTheme(QString theme) {
    if (Settings::getTheme() == "dark") {
        Theme::setColor(Background, Qt::black);
        Theme::setColor(Text, Qt::white);
        Theme::setColor(TransparentText, QColor(Qt::white).darker(200));
        Theme::setColor(Control, QColor("#393939"));
        Theme::setColor(ControlStart, QColor("#b4b4b4"));
        Theme::setColor(ControlStop, QColor("#8f8f8f"));
    } else if (Settings::getTheme() == "grey") {
        Theme::setColor(Background, Qt::gray);
        Theme::setColor(Text, Qt::black);
        Theme::setColor(TransparentText, Qt::darkGray);
        Theme::setColor(Control, QColor("#303030"));
        Theme::setColor(ControlStart, QColor("#222222"));
        Theme::setColor(ControlStop, QColor("#555555"));
    } else if (Settings::getTheme() == "light") {
        Theme::setColor(Background, Qt::lightGray);
        Theme::setColor(Text, Qt::black);
        Theme::setColor(TransparentText, QColor(Qt::lightGray).darker(200));
        Theme::setColor(Control, QColor("#393939"));
        Theme::setColor(ControlStart, QColor("#222222"));
        Theme::setColor(ControlStop, QColor("#8f8f8f"));
    }

    QPalette palette = QApplication::palette();
    palette.setBrush(QPalette::Window, Theme::getColor(Background));
    palette.setBrush(QPalette::Base, Theme::getColor(Background));
    palette.setBrush(QPalette::Text, Theme::getColor(Text));
    palette.setBrush(QPalette::Button, Theme::getColor(Background));
    palette.setBrush(QPalette::ButtonText, Theme::getColor(Text));
    QApplication::setPalette(palette);

    QString style;
    // clang-format off
    style =
        "QPushButton {\
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                stop:0 " + Theme::getColor(Theme::ControlStart).name() + ",\
                stop:1 " + Theme::getColor(Theme::ControlStop).name() + ");\
            border-style: solid;\
            border-width: 1px;\
            border-radius: 5px;\
        }\
        QSlider::groove:vertical {\
            background: " + Theme::getColor(Theme::Control).name() + ";\
            position: absolute;\
            left: 4px; right: 4px;\
        }\
        QSlider::handle:vertical {\
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\
                stop:0 " + Theme::getColor(Theme::ControlStart).name() + ",\
                stop:1 " + Theme::getColor(Theme::ControlStop).name() + ");\
            border: 1px solid " + Theme::getColor(Theme::ControlStop).name() + ";\
        }\
        QSlider::add-page:vertical {\
            background: " + Theme::getColor(Theme::Control).name() + ";\
        }\
        QSlider::sub-page:vertical {\
            background: " + Theme::getColor(Theme::Control).name() + ";\
        }";
    // clang-format on
    static_cast<QApplication *>(QApplication::instance())->setStyleSheet(style);
}

// TODO:
QString Theme::getSystemTheme() { return correct("dark"); }

// check if theme is allowed. Return default, if theme is incorrect
QString Theme::correct(QString theme) {
    QStringList themeAllowed{"dark", "grey", "light"};
    if (!themeAllowed.contains(theme))
        return themeAllowed.front();
    return theme;
}

void Theme::setColor(ColorRole colorRole, QColor color) {
    Theme::getInstance().colorMap.insert(colorRole, color);
}

QColor Theme::getColor(ColorRole colorRole) {
    if (Theme::getInstance().colorMap.contains(colorRole))
        return Theme::getInstance().colorMap.value(colorRole);
    else
        return Qt::black;
}
