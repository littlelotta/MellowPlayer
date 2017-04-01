#pragma once

#include <MellowPlayer/UseCases.hpp>
#include "ListeningHistoryEntryModel.hpp"

BEGIN_MELLOWPLAYER_NAMESPACE(Presentation)

class ListeningHistoryViewModel: public QObject {
    Q_PROPERTY(QAbstractListModel* model READ getModel CONSTANT)
    Q_OBJECT
public:
    ListeningHistoryViewModel(UseCases::ListeningHistory& listeningHistory);

    ListeningHistoryEntryListModel* getModel();

private slots:
    void onEntryAdded(const UseCases::ListeningHistoryEntry& entry);
    void onEntryRemoved(int index);
    void onEntriesCleared();

private:
    ListeningHistoryEntryListModel model;
};

END_MELLOWPLAYER_NAMESPACE
