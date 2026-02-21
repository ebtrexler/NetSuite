#ifndef NETWORKEDITOR_H
#define NETWORKEDITOR_H

#include <QTreeWidget>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include "RT_Network.h"

class NetworkEditor : public QTreeWidget
{
    Q_OBJECT

public:
    explicit NetworkEditor(QWidget *parent = nullptr);
    void setNetwork(TNetwork *net);
    void refreshTree();

signals:
    void networkModified();

private slots:
    void onContextMenu(const QPoint &pos);
    
    void addCell();
    void removeCell();
    void editCell();
    
    void addCurrentToCell();
    void removeCurrentFromCell();
    void editCurrent();
    
    void addElectrodeToCell();
    void removeElectrodeFromCell();
    void editElectrode();
    
    void addSynapse();
    void removeSynapse();
    void editSynapse();

private:
    TNetwork *network;
    
    enum ItemType { Root, CellsFolder, CellItem, CurrentsFolder, CurrentItem,
                    ElectrodesFolder, ElectrodeItem, SynapsesFolder, SynapseItem };
    
    void setItemData(QTreeWidgetItem *item, ItemType type, const std::wstring &name = L"");
    ItemType getItemType(QTreeWidgetItem *item);
    std::wstring getItemName(QTreeWidgetItem *item);
    std::wstring getParentCellName(QTreeWidgetItem *item);
};

#endif
