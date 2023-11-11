#include "Language.h"
#include "MainWidget.h"
#include "Settings.h"
#include "Theme.h"
#include <QApplication>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Language::applyLanguage(Settings::getLanguage());
    Theme::applyTheme(Settings::getTheme());

    MainWidget mainWidget;
    mainWidget.show();

    return app.exec();
}
