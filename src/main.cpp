#include "Language.h"
#include "MainWidget.h"
#include "Settings.h"
#include <QApplication>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Language::loadTranslation(Settings::getLanguage());

    MainWidget mainWidget;
    mainWidget.show();

    return app.exec();
}
