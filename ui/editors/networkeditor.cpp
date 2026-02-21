#include "networkeditor.h"
#include "RT_ModelCell.h"
#include "RT_HHCurrent.h"
#include "RT_InjectionElectrode.h"
#include <QHeaderView>

NetworkEditor::NetworkEditor(QWidget *parent)
    : QTreeWidget(parent), network(nullptr)
{
    setHeaderLabel("Network Hierarchy");
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &NetworkEditor::onContextMenu);
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
            menu.addAction("Remove Synapse", this, &NetworkEditor::removeSynapse);
            break;
        default: break;
        }
    }
    
    if (!menu.isEmpty()) menu.exec(mapToGlobal(pos));
}

void NetworkEditor::addCell()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Cell", "Cell name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    
    // Register factory if needed
    static bool reg = false;
    if (!reg) {
        try { GetCellFactory().registerBuilder(
            TModelCell_KEY, TypeID<TModelCell>(), TypeID<const std::wstring>()); } catch (...) {}
        reg = true;
    }
    
    try {
        TCell *cell = network->AddCellToNet(TModelCell_KEY, name.toStdWString());
        
        // Edit capacitance and initial Vm
        TModelCell *mc = dynamic_cast<TModelCell*>(cell);
        if (mc) {
            double cap = QInputDialog::getDouble(this, "Cell Parameters", "Capacitance (nF):", 
                mc->Capacitance(), 0.001, 10000, 3, &ok);
            if (ok) mc->SetCapacitance(cap);
            
            double vm = QInputDialog::getDouble(this, "Cell Parameters", "Initial Vm (mV):", 
                mc->InitialVm(), -200, 200, 1, &ok);
            if (ok) mc->SetInitialVm(vm);
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
    
    TModelCell *mc = dynamic_cast<TModelCell*>(it->second.get());
    if (!mc) return;
    
    bool ok;
    double cap = QInputDialog::getDouble(this, "Edit Cell", "Capacitance (nF):", 
        mc->Capacitance(), 0.001, 10000, 3, &ok);
    if (ok) mc->SetCapacitance(cap);
    
    double vm = QInputDialog::getDouble(this, "Edit Cell", "Initial Vm (mV):", 
        mc->InitialVm(), -200, 200, 1, &ok);
    if (ok) mc->SetInitialVm(vm);
    
    emit networkModified();
}

void NetworkEditor::addCurrentToCell()
{
    QTreeWidgetItem *item = currentItem();
    std::wstring cellName;
    
    if (getItemType(item) == CellItem) cellName = getItemName(item);
    else cellName = getParentCellName(item);
    if (cellName.empty()) return;
    
    bool ok;
    QString name = QInputDialog::getText(this, "Add Current", "Current name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    
    // Register HH current factory if needed
    try { GetCurrentFactory().registerBuilder(
        THHCurrent_KEY, TypeID<THHCurrent>(),
        TypeID<TCurrentUser*const>(), TypeID<const std::wstring>()); } catch (...) {}
    
    try {
        TCurrent *cur = network->AddCurrentToCell(THHCurrent_KEY, name.toStdWString(), cellName);
        
        THHCurrent *hh = dynamic_cast<THHCurrent*>(cur);
        if (hh) {
            double gmax = QInputDialog::getDouble(this, "HH Current", "Gmax (µS):", 
                hh->Gmax(), 0, 100000, 3, &ok);
            if (ok) hh->Gmax(gmax);
            
            double e = QInputDialog::getDouble(this, "HH Current", "Reversal potential E (mV):", 
                hh->E(), -200, 200, 1, &ok);
            if (ok) hh->E(e);
            
            double p = QInputDialog::getDouble(this, "HH Current", "Activation exponent p:", 
                hh->p(), 0, 10, 0, &ok);
            if (ok) hh->p(p);
            
            double q = QInputDialog::getDouble(this, "HH Current", "Inactivation exponent q:", 
                hh->q(), 0, 10, 0, &ok);
            if (ok) hh->q(q);
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
    
    // Find the current
    const TCellsMap &cells = network->GetCells();
    auto cit = cells.find(cellName);
    if (cit == cells.end()) return;
    
    TCurrentsArray currents = cit->second->GetCurrents();
    for (auto *c : currents) {
        if (c->Name() == curName) {
            THHCurrent *hh = dynamic_cast<THHCurrent*>(c);
            if (hh) {
                bool ok;
                double gmax = QInputDialog::getDouble(this, "Edit HH Current", "Gmax (µS):", 
                    hh->Gmax(), 0, 100000, 3, &ok);
                if (ok) hh->Gmax(gmax);
                
                double e = QInputDialog::getDouble(this, "Edit HH Current", "E (mV):", 
                    hh->E(), -200, 200, 1, &ok);
                if (ok) hh->E(e);
                
                double p = QInputDialog::getDouble(this, "Edit HH Current", "p:", 
                    hh->p(), 0, 10, 0, &ok);
                if (ok) hh->p(p);
                
                double q = QInputDialog::getDouble(this, "Edit HH Current", "q:", 
                    hh->q(), 0, 10, 0, &ok);
                if (ok) hh->q(q);
            }
            emit networkModified();
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
    
    bool ok;
    QString name = QInputDialog::getText(this, "Add Electrode", "Electrode name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    
    // Register electrode factory if needed
    try { GetElectrodeFactory().registerBuilder(
        TInjectionElectrode_KEY, TypeID<TInjectionElectrode>(),
        TypeID<TCell*const>(), TypeID<const std::wstring>()); } catch (...) {}
    
    try {
        TElectrode *el = network->AddElectrodeToCell(
            TInjectionElectrode_KEY, name.toStdWString(), cellName);
        
        TInjectionElectrode *inj = dynamic_cast<TInjectionElectrode*>(el);
        if (inj) {
            double amp = QInputDialog::getDouble(this, "Injection Electrode", "Amplitude (nA):", 
                inj->Amplitude(), -1000, 1000, 3, &ok);
            if (ok) inj->SetAmplitude(amp);
            
            double dur = QInputDialog::getDouble(this, "Injection Electrode", "Duration (ms):", 
                inj->Duration(), 0, 100000, 1, &ok);
            if (ok) inj->SetDuration(dur);
            
            double delay = QInputDialog::getDouble(this, "Injection Electrode", "Interpulse interval (ms):", 
                inj->Delay(), 0, 100000, 1, &ok);
            if (ok) inj->SetDelay(delay);
            
            int reps = QInputDialog::getInt(this, "Injection Electrode", "Repeats (-1 = forever):", 
                inj->NumRepeats(), -1, 100000, 1, &ok);
            if (ok) inj->SetNumRepeats(reps);
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
            TInjectionElectrode *inj = dynamic_cast<TInjectionElectrode*>(e);
            if (inj) {
                bool ok;
                double amp = QInputDialog::getDouble(this, "Edit Electrode", "Amplitude (nA):", 
                    inj->Amplitude(), -1000, 1000, 3, &ok);
                if (ok) inj->SetAmplitude(amp);
                
                double dur = QInputDialog::getDouble(this, "Edit Electrode", "Duration (ms):", 
                    inj->Duration(), 0, 100000, 1, &ok);
                if (ok) inj->SetDuration(dur);
                
                double delay = QInputDialog::getDouble(this, "Edit Electrode", "Interval (ms):", 
                    inj->Delay(), 0, 100000, 1, &ok);
                if (ok) inj->SetDelay(delay);
                
                int reps = QInputDialog::getInt(this, "Edit Electrode", "Repeats (-1 = forever):", 
                    inj->NumRepeats(), -1, 100000, 1, &ok);
                if (ok) inj->SetNumRepeats(reps);
            }
            emit networkModified();
            return;
        }
    }
}

void NetworkEditor::addSynapse()
{
    if (!network) return;
    
    const TCellsMap &cells = network->GetCells();
    if (cells.size() < 2) {
        QMessageBox::information(this, "Add Synapse", "Need at least 2 cells to create a synapse.");
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
    
    QString name = QInputDialog::getText(this, "Add Synapse", "Synapse name:", QLineEdit::Normal, 
        pre + " → " + post, &ok);
    if (!ok || name.isEmpty()) return;
    
    // For now, just add a simple synapse entry - we'd need a synapse type registered
    // Gap junction synapses would need to be extracted from GUI code like TModelCell was
    QMessageBox::information(this, "Add Synapse", 
        "Synapse types (Gap Junction, etc.) need to be extracted from legacy code.\n"
        "This will be implemented next.");
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
