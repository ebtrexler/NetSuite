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

    // Called from NetworkView context menu
    void addCellAt(int x, int y);
    void addElectrodeToCellByName(const std::wstring &cellName);
    void addSynapseBetween(const std::wstring &preName, const std::wstring &postName);

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
