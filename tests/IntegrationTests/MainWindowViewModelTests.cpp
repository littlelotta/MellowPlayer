#include <catch.hpp>
#include <QtTest/QSignalSpy>
#include <MellowPlayer/Presentation/ViewModels/MainWindowViewModel.hpp>
#include "Utils/DependencyPool.hpp"

using namespace MellowPlayer;
using namespace MellowPlayer::Application;
using namespace MellowPlayer::Presentation;

TEST_CASE("MainWindowViewModel") {
    Tests::DependencyPool pool;
    MainWindowViewModel& mainWindow = pool.getMainWindowViewModel();
    QSignalSpy visibleChangedSpy(&mainWindow, SIGNAL(visibleChanged()));

    REQUIRE(mainWindow.load());

    SECTION("show emit visibleChanged") {
        REQUIRE(!mainWindow.isVisible());
        REQUIRE(visibleChangedSpy.count() == 0);
        mainWindow.show();
        REQUIRE(mainWindow.isVisible());
        REQUIRE(visibleChangedSpy.count() == 1);

        SECTION("hide") {
            mainWindow.hide();
            REQUIRE(!mainWindow.isVisible());
            REQUIRE(visibleChangedSpy.count() == 2);
        }
    }
}
