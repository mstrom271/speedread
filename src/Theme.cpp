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
    QPalette palette = QApplication::palette();

    if (Settings::getTheme() == "dark") {
        palette.setBrush(QPalette::Window, Qt::black);
        palette.setBrush(QPalette::Base, Qt::black);
        palette.setBrush(QPalette::Text, QColor(Qt::white).darker(200));
        palette.setBrush(QPalette::BrightText, Qt::white);
        palette.setBrush(QPalette::Button, Qt::black);
        palette.setBrush(QPalette::ButtonText, Qt::white);
    } else if (Settings::getTheme() == "grey") {
        palette.setBrush(QPalette::Window, Qt::gray);
        palette.setBrush(QPalette::Base, Qt::gray);
        palette.setBrush(QPalette::Text, QColor(Qt::white).darker(200));
        palette.setBrush(QPalette::BrightText, Qt::white);
        palette.setBrush(QPalette::Button, Qt::gray);
        palette.setBrush(QPalette::ButtonText, Qt::white);
    } else if (Settings::getTheme() == "light") {
        palette.setBrush(QPalette::Window, Qt::lightGray);
        palette.setBrush(QPalette::Base, Qt::lightGray);
        palette.setBrush(QPalette::Text, QColor(Qt::lightGray).darker(200));
        palette.setBrush(QPalette::BrightText, Qt::black);
        palette.setBrush(QPalette::Button, Qt::lightGray);
        palette.setBrush(QPalette::ButtonText, Qt::black);
    }

    QApplication::setPalette(palette);
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
