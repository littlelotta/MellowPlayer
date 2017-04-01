#include <QtWebEngine>
#include "QtWebApplication.hpp"
#include "MellowPlayer/Presentation/Widgets/QmlMainWindow.hpp"
#include "IconProvider.hpp"

USE_MELLOWPLAYER_NAMESPACE(UseCases)
USE_MELLOWPLAYER_NAMESPACE(Presentation)

QtWebApplication::QtWebApplication(int &argc, char **argv) :
    QApplication(argc, argv) {
    setApplicationDisplayName("MellowPlayer");
    setApplicationName("MellowPlayer3");
    setApplicationVersion(MELLOWPLAYER_VERSION);
    setOrganizationDomain("org.mellowplayer");
    setOrganizationName("MellowPlayer");
    setWindowIcon(IconProvider::windowIcon());

    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Black.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Italic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Thin.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-ThinItalic.ttf");

    qRegisterMetaType<Player*>("UseCases::Player*");
    qRegisterMetaType<Song*>("UseCases::Song*");
    qRegisterMetaType<Song*>("UseCases::Plugin*");
    qRegisterMetaType<ListeningHistoryEntry>("ListeningHistoryEntry");
}

int QtWebApplication::run() {
    return exec();
}

void QtWebApplication::quit() {
    exit(0);
}
