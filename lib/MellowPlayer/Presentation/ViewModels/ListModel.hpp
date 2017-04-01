#pragma once

#include <QAbstractListModel>
#include <MellowPlayer/Macros.hpp>

BEGIN_MELLOWPLAYER_NAMESPACE(Presentation)

template<typename T>
class ListModel: public QAbstractListModel {
public:
    enum Roles { Object };

    ListModel(): QAbstractListModel() { }

    void prepend(T* item) {
        beginInsertRows(QModelIndex(), 0, 0);
        items.prepend(item);
        endInsertRows();
    }

    void append(T* item) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        items.append(item);
        endInsertRows();
    }

    void removeAt(int index) {
        beginRemoveRows(QModelIndex(), index, index);
        items.removeAt(index);
        endRemoveRows();
    }

    void remove(T* item) {
        auto index = items.indexOf(item);
        beginRemoveRows(QModelIndex(), index, index);
        items.removeAt(index);
        endRemoveRows();
    }

    void clear() {
        beginRemoveRows(QModelIndex(), 0, rowCount());
        items.clear();
        endRemoveRows();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return items.count();
    }

    QVariant data(const QModelIndex& index, int) const override {
        if (index.row() < 0 || index.row() >= rowCount())
            return QVariant();
        QVariant data;
        data.setValue(items[index.row()]);
        return data;
    }

    QList<T*> getItems() { return items; }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[Object] = "object";
        return roles;
    };

private:
    QList<T*> items;
};

END_MELLOWPLAYER_NAMESPACE
