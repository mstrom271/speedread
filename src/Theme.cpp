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

// TODO:
QString Theme::getSystemTheme() { return correct("dark"); }

// check if theme is allowed. Return default, if theme is incorrect
QString Theme::correct(QString theme) {
    QStringList themeAllowed{"dark", "grey", "light"};
    if (!themeAllowed.contains(theme))
        return themeAllowed.front();
    return theme;
}
