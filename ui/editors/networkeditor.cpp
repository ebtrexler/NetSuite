#include "networkeditor.h"
#include "RT_ModelCell.h"
#include "RT_PlaybackCell.h"
#include "RT_HHCurrent.h"
#include "RT_HH2Current.h"
#include "RT_VoltageClampPIDCurrent.h"
#include "RT_PlaybackCurrent.h"
#include "RT_InjectionElectrode.h"
#include "RT_PlaybackElectrode.h"
#include "RT_GapJunctionSynapse.h"
#include "RT_GenBiDirSynapse.h"
#include "RT_GJCurrent.h"
#include "RT_BiologicalCell.h"
#include "biologicalcelldialog.h"
#include "modelcelldialog.h"
#include "hhcurrentdialog.h"
#include "hh2currentdialog.h"
#include "voltageclamppiddialog.h"
#include "playbackcurrentdialog.h"
#include "electrodedialog.h"
#include "playbackelectrodedialog.h"
#include "playbackcelldialog.h"
#include "synapsedialog.h"
#include <QHeaderView>

NetworkEditor::NetworkEditor(QWidget *parent)
    : QTreeWidget(parent), network(nullptr)
{
    setHeaderLabel("Network Hierarchy");
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &NetworkEditor::onContextMenu);
    connect(this, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem *item) {
        if (!item) return;
        switch (getItemType(item)) {
        case CellItem: editCell(); break;
        case CurrentItem: editCurrent(); break;
        case ElectrodeItem: editElectrode(); break;
        case SynapseItem: editSynapse(); break;
        default: break;
        }
    });
}

void NetworkEditor::setNetwork(TNetwork *net)
{
    network = net;
    refreshTree();
}

void NetworkEditor::setItemData(QTreeWidgetItem *item, ItemType type, const std::wstring &name)
{
    item->setData(0, Qt::UserRole, (int)type);
    item->setData(0, Qt::UserRole + 1, QString::fromStdWString(name));
}

NetworkEditor::ItemType NetworkEditor::getItemType(QTreeWidgetItem *item)
{
    return (ItemType)item->data(0, Qt::UserRole).toInt();
}

std::wstring NetworkEditor::getItemName(QTreeWidgetItem *item)
{
    return item->data(0, Qt::UserRole + 1).toString().toStdWString();
}

std::wstring NetworkEditor::getParentCellName(QTreeWidgetItem *item)
{
    QTreeWidgetItem *p = item->parent();
    while (p) {
        if (getItemType(p) == CellItem) return getItemName(p);
        p = p->parent();
    }
    return L"";
}

void NetworkEditor::refreshTree()
{
    clear();
    if (!network) return;
    
    QTreeWidgetItem *root = new QTreeWidgetItem(this);
    root->setText(0, QString::fromStdWString(network->Name()));
    setItemData(root, Root);
    root->setExpanded(true);
    
    // Cells
    QTreeWidgetItem *cellsFolder = new QTreeWidgetItem(root);
    const TCellsMap &cells = network->GetCells();
    cellsFolder->setText(0, QString("Cells (%1)").arg(cells.size()));
    setItemData(cellsFolder, CellsFolder);
    cellsFolder->setExpanded(true);
    
    for (auto it = cells.begin(); it != cells.end(); ++it) {
        QTreeWidgetItem *cellItem = new QTreeWidgetItem(cellsFolder);
        cellItem->setText(0, QString::fromStdWString(it->first));
        setItemData(cellItem, CellItem, it->first);
        
        TCell *cell = it->second.get();
        
        // Currents subfolder
        TCurrentsArray currents = cell->GetCurrents();
        if (!currents.empty()) {
            QTreeWidgetItem *curFolder = new QTreeWidgetItem(cellItem);
            curFolder->setText(0, QString("Currents (%1)").arg(currents.size()));
            setItemData(curFolder, CurrentsFolder);
            for (auto *c : currents) {
                QTreeWidgetItem *ci = new QTreeWidgetItem(curFolder);
                ci->setText(0, QString::fromStdWString(c->Name()));
                setItemData(ci, CurrentItem, c->Name());
            }
        }
        
        // Electrodes subfolder
        TElectrodesArray electrodes = cell->GetElectrodes();
        if (!electrodes.empty()) {
            QTreeWidgetItem *elFolder = new QTreeWidgetItem(cellItem);
            elFolder->setText(0, QString("Electrodes (%1)").arg(electrodes.size()));
            setItemData(elFolder, ElectrodesFolder);
            for (auto *e : electrodes) {
                QTreeWidgetItem *ei = new QTreeWidgetItem(elFolder);
                ei->setText(0, QString::fromStdWString(e->Name()));
                setItemData(ei, ElectrodeItem, e->Name());
            }
        }
    }
    
    // Synapses
    const TSynapsesMap &synapses = network->GetSynapses();
    if (!synapses.empty()) {
        QTreeWidgetItem *synFolder = new QTreeWidgetItem(root);
        synFolder->setText(0, QString("Synapses (%1)").arg(synapses.size()));
        setItemData(synFolder, SynapsesFolder);
        for (auto it = synapses.begin(); it != synapses.end(); ++it) {
            QTreeWidgetItem *si = new QTreeWidgetItem(synFolder);
            TSynapse *syn = it->second.get();
            QString label = QString::fromStdWString(it->first) + " (" +
                QString::fromStdWString(syn->Pre()->Name()) + " → " +
                QString::fromStdWString(syn->Post()->Name()) + ")";
            si->setText(0, label);
            setItemData(si, SynapseItem, it->first);
        }
    }
}

void NetworkEditor::onContextMenu(const QPoint &pos)
{
    if (!network) return;
    
    QTreeWidgetItem *item = itemAt(pos);
    QMenu menu(this);
    
    if (!item || getItemType(item) == Root) {
        menu.addAction("Add Cell", this, &NetworkEditor::addCell);
        menu.addAction("Add Synapse", this, &NetworkEditor::addSynapse);
    } else {
        switch (getItemType(item)) {
        case CellsFolder:
            menu.addAction("Add Cell", this, &NetworkEditor::addCell);
            break;
        case CellItem:
            menu.addAction("Edit Cell", this, &NetworkEditor::editCell);
            menu.addAction("Add Current", this, &NetworkEditor::addCurrentToCell);
            menu.addAction("Add Electrode", this, &NetworkEditor::addElectrodeToCell);
            menu.addSeparator();
            menu.addAction("Remove Cell", this, &NetworkEditor::removeCell);
            break;
        case CurrentsFolder:
            menu.addAction("Add Current", this, &NetworkEditor::addCurrentToCell);
            break;
        case CurrentItem:
            menu.addAction("Edit Current", this, &NetworkEditor::editCurrent);
            menu.addAction("Remove Current", this, &NetworkEditor::removeCurrentFromCell);
            break;
        case ElectrodesFolder:
            menu.addAction("Add Electrode", this, &NetworkEditor::addElectrodeToCell);
            break;
        case ElectrodeItem:
            menu.addAction("Edit Electrode", this, &NetworkEditor::editElectrode);
            menu.addAction("Remove Electrode", this, &NetworkEditor::removeElectrodeFromCell);
            break;
        case SynapsesFolder:
            menu.addAction("Add Synapse", this, &NetworkEditor::addSynapse);
            break;
        case SynapseItem:
            menu.addAction("Edit Synapse", this, &NetworkEditor::editSynapse);
            menu.addAction("Remove Synapse", this, &NetworkEditor::removeSynapse);
            break;
        default: break;
        }
    }
    
    if (!menu.isEmpty()) menu.exec(mapToGlobal(pos));
}

static void registerAllCellFactories() {
    static bool done = false;
    if (done) return;
    done = true;
    try { GetCellFactory().registerBuilder(
        TModelCell_KEY, TypeID<TModelCell>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCellFactory().registerBuilder(
        TPlaybackCell_KEY, TypeID<TPlaybackCell>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCellFactory().registerBuilder(
        TBiologicalCell_KEY, TypeID<TBiologicalCell>(), TypeID<const std::wstring>()); } catch (...) {}
}

static void registerAllCurrentFactories() {
    static bool done = false;
    if (done) return;
    done = true;
    try { GetCurrentFactory().registerBuilder(
        THHCurrent_KEY, TypeID<THHCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        THH2Current_KEY, TypeID<THH2Current>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        TVoltageClamp_PID_Current_KEY, TypeID<TVoltageClampPIDCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        TPLAYBACKCURRENT_KEY, TypeID<TPlaybackCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetCurrentFactory().registerBuilder(
        TGAPJUNCTIONCURRENT_KEY, TypeID<TGapJunctionCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
}

static void registerAllElectrodeFactories() {
    static bool done = false;
    if (done) return;
    done = true;
    try { GetElectrodeFactory().registerBuilder(
        TInjectionElectrode_KEY, TypeID<TInjectionElectrode>(),
        TypeID<TCell*const>(), TypeID<const std::wstring>()); } catch (...) {}
    try { GetElectrodeFactory().registerBuilder(
        TPlaybackElectrode_KEY, TypeID<TPlaybackElectrode>(),
        TypeID<TCell*const>(), TypeID<const std::wstring>()); } catch (...) {}
}

static void registerAllSynapseFactories() {
    static bool done = false;
    if (done) return;
    done = true;
    registerAllCurrentFactories();
    try { GetSynapseFactory().registerBuilder(
        TGAPJUNCTIONSYNAPSE_KEY, TypeID<TGapJunctionSynapse>(),
        TypeID<const std::wstring>(), TypeID<TCell*const>(), TypeID<TCell*const>()); } catch (...) {}
    try { GetSynapseFactory().registerBuilder(
        TGENBIDIRSYNAPSE_KEY, TypeID<TGenBiDirSynapse>(),
        TypeID<const std::wstring>(), TypeID<TCell*const>(), TypeID<TCell*const>()); } catch (...) {}
}

static bool showCellDialog(TCell *cell, QWidget *parent) {
    if (auto *mc = dynamic_cast<TModelCell*>(cell)) {
        ModelCellDialog dlg(mc, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    if (auto *pc = dynamic_cast<TPlaybackCell*>(cell)) {
        PlaybackCellDialog dlg(pc, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    if (auto *bc = dynamic_cast<TBiologicalCell*>(cell)) {
        BiologicalCellDialog dlg(bc, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    return true; // unknown type, just accept
}

static bool showCurrentDialog(TCurrent *cur, QWidget *parent) {
    if (auto *hh = dynamic_cast<THHCurrent*>(cur)) {
        HHCurrentDialog dlg(hh, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    if (auto *hh2 = dynamic_cast<THH2Current*>(cur)) {
        HH2CurrentDialog dlg(hh2, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    if (auto *pid = dynamic_cast<TVoltageClampPIDCurrent*>(cur)) {
        VoltageClampPIDDialog dlg(pid, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    if (auto *pb = dynamic_cast<TPlaybackCurrent*>(cur)) {
        PlaybackCurrentDialog dlg(pb, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    return true;
}

static bool showElectrodeDialog(TElectrode *el, QWidget *parent) {
    if (auto *inj = dynamic_cast<TInjectionElectrode*>(el)) {
        ElectrodeDialog dlg(inj, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    if (auto *pb = dynamic_cast<TPlaybackElectrode*>(el)) {
        PlaybackElectrodeDialog dlg(pb, parent);
        return dlg.exec() == QDialog::Accepted;
    }
    return true;
}

void NetworkEditor::addCell()
{
    QStringList types;
    types << "Model Cell" << "Vm Playback Cell";
    types << "Biological Cell";
    bool ok;
    QString type = QInputDialog::getItem(this, "Add Cell", "Cell type:", types, 0, false, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "Add Cell", "Cell name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    registerAllCellFactories();

    try {
        TCell *cell = network->AddCellToNet(type.toStdWString(), name.toStdWString());
        if (!showCellDialog(cell, this)) {
            network->RemoveCellFromNet(name.toStdWString());
            return;
        }
        network->DescribeNetwork();
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::removeCell()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != CellItem) return;
    
    std::wstring name = getItemName(item);
    if (QMessageBox::question(this, "Remove Cell", 
        QString("Remove %1?").arg(QString::fromStdWString(name))) == QMessageBox::Yes) {
        try {
            network->RemoveCellFromNet(name);
            network->DescribeNetwork();
            refreshTree();
            emit networkModified();
        } catch (std::exception &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }
}

void NetworkEditor::editCell()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != CellItem) return;
    
    std::wstring name = getItemName(item);
    const TCellsMap &cells = network->GetCells();
    auto it = cells.find(name);
    if (it == cells.end()) return;
    
    if (showCellDialog(it->second.get(), this)) emit networkModified();
}

void NetworkEditor::addCurrentToCell()
{
    QTreeWidgetItem *item = currentItem();
    std::wstring cellName;
    if (getItemType(item) == CellItem) cellName = getItemName(item);
    else cellName = getParentCellName(item);
    if (cellName.empty()) return;
    
    QStringList types;
    types << "HH Current" << "HH 2 Currents Convolved" << "Voltage Clamp (PID) Current" << "Playback Current";
    bool ok;
    QString type = QInputDialog::getItem(this, "Add Current", "Current type:", types, 0, false, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "Add Current", "Current name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    
    registerAllCurrentFactories();
    
    try {
        TCurrent *cur = network->AddCurrentToCell(type.toStdWString(), name.toStdWString(), cellName);
        if (!showCurrentDialog(cur, this)) {
            network->RemoveCurrentFromCell(name.toStdWString(), cellName);
            return;
        }
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::removeCurrentFromCell()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != CurrentItem) return;
    
    std::wstring curName = getItemName(item);
    std::wstring cellName = getParentCellName(item);
    
    try {
        network->RemoveCurrentFromCell(curName, cellName);
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::editCurrent()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != CurrentItem) return;
    
    std::wstring curName = getItemName(item);
    std::wstring cellName = getParentCellName(item);
    
    const TCellsMap &cells = network->GetCells();
    auto cit = cells.find(cellName);
    if (cit == cells.end()) return;
    
    TCurrentsArray currents = cit->second->GetCurrents();
    for (auto *c : currents) {
        if (c->Name() == curName) {
            if (showCurrentDialog(c, this)) emit networkModified();
            return;
        }
    }
}

void NetworkEditor::addElectrodeToCell()
{
    QTreeWidgetItem *item = currentItem();
    std::wstring cellName;
    if (getItemType(item) == CellItem) cellName = getItemName(item);
    else cellName = getParentCellName(item);
    if (cellName.empty()) return;
    
    QStringList types;
    types << "Square Pulse Iinj Electrode" << "Playback Electrode";
    bool ok;
    QString type = QInputDialog::getItem(this, "Add Electrode", "Electrode type:", types, 0, false, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "Add Electrode", "Electrode name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    
    registerAllElectrodeFactories();
    
    try {
        TElectrode *el = network->AddElectrodeToCell(
            type.toStdWString(), name.toStdWString(), cellName);
        if (!showElectrodeDialog(el, this)) {
            network->RemoveElectrodeFromNet(name.toStdWString());
            return;
        }
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::removeElectrodeFromCell()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != ElectrodeItem) return;
    
    std::wstring elName = getItemName(item);
    
    try {
        network->RemoveElectrodeFromNet(elName);
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::editElectrode()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != ElectrodeItem) return;
    
    std::wstring elName = getItemName(item);
    std::wstring cellName = getParentCellName(item);
    
    const TCellsMap &cells = network->GetCells();
    auto cit = cells.find(cellName);
    if (cit == cells.end()) return;
    
    TElectrodesArray electrodes = cit->second->GetElectrodes();
    for (auto *e : electrodes) {
        if (e->Name() == elName) {
            if (showElectrodeDialog(e, this)) emit networkModified();
            return;
        }
    }
}

void NetworkEditor::addSynapse()
{
    if (!network) return;

    const TCellsMap &cells = network->GetCells();
    if (cells.size() < 2) {
        QMessageBox::information(this, "Add Synapse", "Need at least 2 cells.");
        return;
    }

    QStringList cellNames;
    for (auto it = cells.begin(); it != cells.end(); ++it)
        cellNames << QString::fromStdWString(it->first);

    bool ok;
    QString pre = QInputDialog::getItem(this, "Add Synapse", "Pre-synaptic cell:", cellNames, 0, false, &ok);
    if (!ok) return;
    QString post = QInputDialog::getItem(this, "Add Synapse", "Post-synaptic cell:", cellNames, 0, false, &ok);
    if (!ok) return;

    QStringList types;
    types << "Gap Junction Synapse" << "Generic Bi-Directional Synapse";
    QString type = QInputDialog::getItem(this, "Add Synapse", "Synapse type:", types, 0, false, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "Add Synapse", "Synapse name:",
        QLineEdit::Normal, pre + " → " + post, &ok);
    if (!ok || name.isEmpty()) return;

    // Register factories
    registerAllSynapseFactories();
    try {
        TSynapse *syn = network->AddSynapseBetweenCells(
            type.toStdWString(), name.toStdWString(),
            pre.toStdWString(), post.toStdWString());

        // Open edit dialog for gap junctions
        TGapJunctionSynapse *gj = dynamic_cast<TGapJunctionSynapse*>(syn);
        if (gj) {
            GapJunctionSynapseDialog dlg(gj, this);
            if (dlg.exec() != QDialog::Accepted) {
                network->RemoveSynapseFromNet(name.toStdWString());
                return;
            }
        }

        network->DescribeNetwork();
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::editSynapse()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != SynapseItem) return;

    std::wstring name = getItemName(item);
    const TSynapsesMap &synapses = network->GetSynapses();
    auto it = synapses.find(name);
    if (it == synapses.end()) return;

    TGapJunctionSynapse *gj = dynamic_cast<TGapJunctionSynapse*>(it->second.get());
    if (gj) {
        GapJunctionSynapseDialog dlg(gj, this);
        if (dlg.exec() == QDialog::Accepted) emit networkModified();
    }
}

void NetworkEditor::removeSynapse()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != SynapseItem) return;
    
    std::wstring name = getItemName(item);
    if (QMessageBox::question(this, "Remove Synapse", 
        QString("Remove %1?").arg(QString::fromStdWString(name))) == QMessageBox::Yes) {
        try {
            network->RemoveSynapseFromNet(name);
            refreshTree();
            emit networkModified();
        } catch (std::exception &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }
}

void NetworkEditor::addCellAt(int x, int y)
{
    if (!network) return;
    QStringList types;
    types << "Model Cell" << "Vm Playback Cell";
    types << "Biological Cell";
    bool ok;
    QString type = QInputDialog::getItem(this, "Add Cell", "Cell type:", types, 0, false, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "Add Cell", "Cell name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    registerAllCellFactories();

    try {
        TCell *cell = network->AddCellToNet(type.toStdWString(), name.toStdWString(), x, y);
        if (!showCellDialog(cell, this)) {
            network->RemoveCellFromNet(name.toStdWString());
            return;
        }
        network->DescribeNetwork();
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::addElectrodeToCellByName(const std::wstring &cellName)
{
    if (!network) return;
    QStringList types;
    types << "Square Pulse Iinj Electrode" << "Playback Electrode";
    bool ok;
    QString type = QInputDialog::getItem(this, "Add Electrode", "Electrode type:", types, 0, false, &ok);
    if (!ok) return;

    QString name = QInputDialog::getText(this, "Add Electrode", "Electrode name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    registerAllElectrodeFactories();

    try {
        TElectrode *el = network->AddElectrodeToCell(
            type.toStdWString(), name.toStdWString(), cellName);
        if (!showElectrodeDialog(el, this)) {
            network->RemoveElectrodeFromNet(name.toStdWString());
            return;
        }
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void NetworkEditor::addSynapseBetween(const std::wstring &preName, const std::wstring &postName)
{
    if (!network) return;
    bool ok;
    QStringList types;
    types << "Gap Junction Synapse" << "Generic Bi-Directional Synapse";
    QString type = QInputDialog::getItem(this, "Add Synapse", "Synapse type:", types, 0, false, &ok);
    if (!ok) return;

    QString defaultName = QString::fromStdWString(preName) + " → " + QString::fromStdWString(postName);
    QString name = QInputDialog::getText(this, "Add Synapse", "Synapse name:",
        QLineEdit::Normal, defaultName, &ok);
    if (!ok || name.isEmpty()) return;

    registerAllSynapseFactories();

    try {
        TSynapse *syn = network->AddSynapseBetweenCells(
            type.toStdWString(), name.toStdWString(), preName, postName);

        TGapJunctionSynapse *gj = dynamic_cast<TGapJunctionSynapse*>(syn);
        if (gj) {
            GapJunctionSynapseDialog dlg(gj, this);
            if (dlg.exec() != QDialog::Accepted) {
                network->RemoveSynapseFromNet(name.toStdWString());
                return;
            }
        }
        network->DescribeNetwork();
        refreshTree();
        emit networkModified();
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}
