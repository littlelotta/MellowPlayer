#include "DependencyPool.hpp"

#include <MellowPlayer/Domain/ListeningHistory/ListeningHistory.hpp>
#include <MellowPlayer/Domain/Player/CurrentPlayer.hpp>
#include <MellowPlayer/Domain/Player/Players.hpp>
#include <MellowPlayer/Domain/Settings/Settings.hpp>
#include <MellowPlayer/Domain/StreamingServices/StreamingServicesController.hpp>
#include <MellowPlayer/Domain/Updater/Github/LatestGithubReleaseQuerier.hpp>
#include <MellowPlayer/Domain/Updater/Updater.hpp>
#include <MellowPlayer/Domain/ICommandLineParser.hpp>
#include <MellowPlayer/Domain/IQtApplication.hpp>
#include <MellowPlayer/Domain/Notifications/INotificationPresenter.hpp>
#include <MellowPlayer/Domain/Settings/ISettingsStore.hpp>
#include <MellowPlayer/Domain/StreamingServices/IStreamingServiceCreator.hpp>
#include <MellowPlayer/Domain/Updater/AbstractPlatformUpdater.hpp>
#include <MellowPlayer/Domain/UserScripts/IUserScriptFactory.hpp>

#include <MellowPlayer/Infrastructure/Services/LocalAlbumArt.hpp>
#include <MellowPlayer/Infrastructure/Settings/SettingsSchemaLoader.hpp>

#include <MellowPlayer/Presentation/Notifications/Notifier.hpp>
#include <MellowPlayer/Presentation/ViewModels/ListeningHistory/ListeningHistoryViewModel.hpp>
#include <MellowPlayer/Presentation/ViewModels/MainWindowViewModel.hpp>
#include <MellowPlayer/Presentation/ViewModels/StreamingServices/StreamingServicesControllerViewModel.hpp>
#include <MellowPlayer/Presentation/ViewModels/ThemeViewModel.hpp>
#include <MellowPlayer/Presentation/ViewModels/UpdaterViewModel.hpp>

#include <Mocks/AlbumArtDownloaderMock.hpp>
#include <Mocks/CommnandLineParserMock.hpp>
#include <Mocks/FakeFileDownloader.hpp>
#include <Mocks/FakeHttpClient.hpp>
#include <Mocks/FakePlatformUpdater.hpp>
#include <Mocks/FakeWorkDispatcher.hpp>
#include <Mocks/InMemoryListeningHistoryDataProvider.hpp>
#include <Mocks/NotificationPresenterMock.hpp>
#include <Mocks/QtApplicationMock.hpp>
#include <Mocks/SettingsStoreMock.hpp>
#include <Mocks/StreamingServiceCreatorMock.hpp>
#include <Mocks/StreamingServiceLoaderMock.hpp>
#include <Mocks/StreamingServiceWatcherMock.hpp>
#include <Mocks/ThemeLoaderMock.hpp>
#include <UnitTests/Domain/UserScripts/FakeUserScript.hpp>

using namespace std;
using namespace fakeit;
using namespace MellowPlayer::Domain;
using namespace MellowPlayer::Domain::Tests;
using namespace MellowPlayer::Presentation;
using namespace MellowPlayer::Infrastructure;
using namespace MellowPlayer::Tests;

DependencyPool::DependencyPool()
        : mICommandLineParser(CommandLineParserMock::get()),
          mIQtApplication(QtApplicationMock::get()),
          mISettingsStore(SettingsStoreMock::get()),
          mIStreamingServiceCreator(StreamingServiceCreatorMock::get()),
          mINotificationPresenter(NotificationPresenterMock::get()),
          dataProvider(make_unique<InMemoryListeningHistoryDataProvider>())
{
    When(Method(mUserScriptsFactoryMock, create)).AlwaysDo([]() -> IUserScript* {
        return new FakeUserScript;
    });
}

DependencyPool::~DependencyPool() = default;

StreamingServicesController& DependencyPool::getStreamingServicesController()
{
    static auto streamingServiceLoaderMock = StreamingServiceLoaderMock::get();
    static auto streamingServiceWatcherMock = StreamingServiceWatcherMock::get();
    if (pStreamingServicesController == nullptr) {
        pStreamingServicesController = make_unique<StreamingServicesController>(streamingServiceLoaderMock.get(), streamingServiceWatcherMock.get());
        pStreamingServicesController->load();
    }
    return *pStreamingServicesController;
}

StreamingServicesControllerViewModel& DependencyPool::getStreamingServicesControllerViewModel()
{
    if (pStreamingServicesControllerViewModel == nullptr)
        pStreamingServicesControllerViewModel = make_unique<StreamingServicesControllerViewModel>(
        getStreamingServicesController(),
        getPlayers(),
        getSettings(),
        getWorkDispatcher(),
        getStreamingServicesCreator(),
        getCommandLineParser(),
        getUserScriptFactory());
    return *pStreamingServicesControllerViewModel;
}

Players& DependencyPool::getPlayers()
{
    if (pPlayers == nullptr)
        pPlayers = make_unique<Players>(getStreamingServicesController());
    return *pPlayers;
}

IStreamingServiceCreator& DependencyPool::getStreamingServicesCreator()
{
    return mIStreamingServiceCreator.get();
}

ISettingsStore& DependencyPool::getSettingsStore()
{
    return mISettingsStore.get();
}

Settings& DependencyPool::getSettings()
{
    static SettingsSchemaLoader loader;
    if (pSettings == nullptr)
        pSettings = make_unique<Settings>(loader, getSettingsStore());
    return *pSettings;
}

ICommandLineParser& DependencyPool::getCommandLineParser()
{
    return mICommandLineParser.get();
}

IWorkDispatcher& DependencyPool::getWorkDispatcher()
{
    static FakeWorkDispatcher instance;
    return instance;
}

ListeningHistoryViewModel& DependencyPool::getListeningHistoryViewModel()
{
    if (pListeningHistoryViewModel == nullptr)
        pListeningHistoryViewModel = make_unique<ListeningHistoryViewModel>(getListeningHistory());
    return *pListeningHistoryViewModel;
}

ListeningHistory& DependencyPool::getListeningHistory()
{
    if (pListeningHistory == nullptr)
        pListeningHistory = make_unique<ListeningHistory>(*dataProvider, getCurrentPlayer(), getWorkDispatcher(), getSettings());
    return *pListeningHistory;
}

IPlayer& DependencyPool::getCurrentPlayer()
{
    if (pCurrentPlayer == nullptr)
        pCurrentPlayer = make_unique<CurrentPlayer>(getPlayers(), getStreamingServicesController());
    return *pCurrentPlayer;
}

ThemeViewModel& DependencyPool::getThemeViewModel()
{
    static auto mock = ThemeLoaderMock::get();
    if (pThemeViewModel == nullptr)
        pThemeViewModel = make_unique<ThemeViewModel>(getStreamingServicesController(), getSettings(), mock.get());
    return *pThemeViewModel;
}

UpdaterViewModel& DependencyPool::getUpdaterViewModel()
{
    if (pUpdaterViewModel == nullptr)
        pUpdaterViewModel = make_unique<UpdaterViewModel>(getUpdater());
    return *pUpdaterViewModel;
}

Updater& DependencyPool::getUpdater()
{
    static FakeHttpClient httpClient;
    static LatestGithubReleaseQuerier querier(httpClient);
    if (pUpdater == nullptr)
        pUpdater = make_unique<Updater>(querier, getSettings(), getPlatformUpdater());
    return *pUpdater;
}

IQtApplication& DependencyPool::getQtApplication()
{
    return mIQtApplication.get();
}

MainWindowViewModel& DependencyPool::getMainWindowViewModel()
{
    if (pMainWindowViewModel == nullptr) {
        auto& streamingServices = getStreamingServicesControllerViewModel();
        auto& listeningHistory = getListeningHistoryViewModel();
        auto& theme = getThemeViewModel();
        auto& updater = getUpdaterViewModel();
        auto& qtApp = getQtApplication();
        auto& currentPlayer = getCurrentPlayer();
        auto& settings = getSettings();

        pMainWindowViewModel = make_unique<MainWindowViewModel>(streamingServices, listeningHistory, theme, updater, qtApp, currentPlayer, settings);
    }

    return *pMainWindowViewModel;
}

Notifier& DependencyPool::getNotifier()
{
    if (pNotifier == nullptr)
        pNotifier =
        make_unique<Notifier>(getCurrentPlayer(), getLocalAlbumArt(), getNotificationPresenter(), getStreamingServicesController(), getSettings());
    return *pNotifier;
}

INotificationPresenter& DependencyPool::getNotificationPresenter()
{
    return mINotificationPresenter.get();
}

LocalAlbumArt& DependencyPool::getLocalAlbumArt()
{
    static AlbumArtDownloaderMock downloader;

    if (pLocalAlbumArt == nullptr)
        pLocalAlbumArt = make_unique<LocalAlbumArt>(getCurrentPlayer(), downloader);
    return *pLocalAlbumArt;
}

Mock<INotificationPresenter>& DependencyPool::getNotificationPresenterMock()
{
    return mINotificationPresenter;
}

AbstractPlatformUpdater& DependencyPool::getPlatformUpdater()
{
    static FakeFileDownloader fakeFileDownloader;
    if (pPlatformUpdater == nullptr)
        pPlatformUpdater = make_unique<FakePlatformUpdater>(fakeFileDownloader);
    return *pPlatformUpdater;
}

IUserScriptFactory& DependencyPool::getUserScriptFactory()
{
    return mUserScriptsFactoryMock.get();
}
