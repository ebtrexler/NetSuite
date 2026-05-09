// ui/editors/genbidirsynapsedialog.h
//
// Editor for TGenBiDirSynapse (generic bidirectional chemical synapse).
// Lets you add / edit / remove the currents on each direction:
//   pre → post   (e.g. AMPA, NMDA, GABA_A — the usual synaptic currents)
//   post → pre   (retrograde modulators — rarely used but supported)
//
// Currents on a synapse use the same classes as cell currents
// (THHCurrent, THH2Current, etc.) — only the V_kin vs V_drv wiring
// differs (pre drives kinetics, post drives driving force). So the
// existing HHCurrentDialog etc. work unchanged.

#ifndef GENBIDIRSYNAPSEDIALOG_H
#define GENBIDIRSYNAPSEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialogButtonBox>

#include "RT_Network.h"
#include "RT_GenBiDirSynapse.h"
#include "RT_HHCurrent.h"
#include "RT_HH2Current.h"
#include "RT_VoltageClampPIDCurrent.h"
#include "RT_PlaybackCurrent.h"

#include "hhcurrentdialog.h"
#include "hh2currentdialog.h"
#include "voltageclamppiddialog.h"
#include "playbackcurrentdialog.h"

class GenBiDirSynapseDialog : public QDialog
{
    Q_OBJECT
public:
    GenBiDirSynapseDialog(TNetwork *net, TGenBiDirSynapse *syn,
                          QWidget *parent = nullptr)
        : QDialog(parent), m_net(net), m_syn(syn)
    {
        setWindowTitle(QString("Synapse: %1").arg(
            QString::fromStdWString(syn->Name())));
        resize(480, 400);

        auto *layout = new QVBoxLayout(this);

        auto *info = new QLabel(QString("Pre: %1   →   Post: %2")
            .arg(QString::fromStdWString(syn->Pre()->Name()))
            .arg(QString::fromStdWString(syn->Post()->Name())));
        info->setAlignment(Qt::AlignCenter);
        layout->addWidget(info);

        layout->addWidget(makeDirectionGroup(
            QString("Pre → Post currents (kinetics follow %1, drive %2)")
                .arg(QString::fromStdWString(syn->Pre()->Name()))
                .arg(QString::fromStdWString(syn->Post()->Name())),
            /*preToPost=*/true, m_p2pList));

        layout->addWidget(makeDirectionGroup(
            QString("Post → Pre currents (kinetics follow %1, drive %2)")
                .arg(QString::fromStdWString(syn->Post()->Name()))
                .arg(QString::fromStdWString(syn->Pre()->Name())),
            /*preToPost=*/false, m_p2rList));

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::accept);
        layout->addWidget(buttons);

        refreshLists();
    }

private slots:
    void onAdd(bool preToPost) {
        QStringList types;
        types << "HH Current" << "HH 2 Currents Convolved"
              << "Voltage Clamp (PID) Current" << "Playback Current";
        bool ok = false;
        QString type = QInputDialog::getItem(this, "Add Synaptic Current",
            "Type:", types, 0, false, &ok);
        if (!ok) return;
        QString name = QInputDialog::getText(this, "Add Synaptic Current",
            "Name (unique):", QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;

        // AddCurrentToSynapse takes the target cell name to indicate
        // direction. pre→post means "the current affects the post cell."
        std::wstring targetCell =
            preToPost ? m_syn->Post()->Name() : m_syn->Pre()->Name();

        TCurrent *cur = nullptr;
        try {
            cur = m_net->AddCurrentToSynapse(
                m_syn->Name(), type.toStdWString(),
                name.toStdWString(), targetCell);
        } catch (std::exception &e) {
            QMessageBox::warning(this, "Add Current", e.what());
            return;
        }
        if (!cur) return;

        // Open the type-specific editor. If the user cancels, remove
        // the current — same pattern as addCurrentToCell.
        if (!showCurrentDialog(cur)) {
            try {
                m_net->RemoveCurrentFromSynapse(
                    name.toStdWString(), m_syn->Name());
            } catch (...) {}
        }
        refreshLists();
    }

    void onEdit(bool preToPost) {
        QListWidget *list = preToPost ? m_p2pList : m_p2rList;
        QListWidgetItem *it = list->currentItem();
        if (!it) return;
        std::wstring name = it->text().toStdWString();
        TCurrent *cur = findCurrent(preToPost, name);
        if (cur) showCurrentDialog(cur);
        refreshLists();
    }

    void onRemove(bool preToPost) {
        QListWidget *list = preToPost ? m_p2pList : m_p2rList;
        QListWidgetItem *it = list->currentItem();
        if (!it) return;
        QString name = it->text();
        if (QMessageBox::question(this, "Remove Current",
                QString("Remove \"%1\"?").arg(name)) != QMessageBox::Yes)
            return;
        try {
            m_net->RemoveCurrentFromSynapse(
                name.toStdWString(), m_syn->Name());
        } catch (std::exception &e) {
            QMessageBox::warning(this, "Remove Current", e.what());
        }
        refreshLists();
    }

private:
    QGroupBox* makeDirectionGroup(const QString &title, bool preToPost,
                                  QListWidget *&listRef) {
        auto *group = new QGroupBox(title);
        auto *v = new QVBoxLayout(group);

        auto *list = new QListWidget;
        v->addWidget(list);
        listRef = list;

        auto *row = new QHBoxLayout;
        auto *addBtn = new QPushButton("Add…");
        auto *editBtn = new QPushButton("Edit…");
        auto *remBtn  = new QPushButton("Remove");
        row->addWidget(addBtn);
        row->addWidget(editBtn);
        row->addWidget(remBtn);
        row->addStretch();
        v->addLayout(row);

        connect(addBtn,  &QPushButton::clicked, this,
                [this, preToPost]{ onAdd(preToPost); });
        connect(editBtn, &QPushButton::clicked, this,
                [this, preToPost]{ onEdit(preToPost); });
        connect(remBtn,  &QPushButton::clicked, this,
                [this, preToPost]{ onRemove(preToPost); });
        // Double-click on a current name opens the editor too.
        connect(list, &QListWidget::itemDoubleClicked, this,
                [this, preToPost](QListWidgetItem*) { onEdit(preToPost); });

        return group;
    }

    void refreshLists() {
        m_p2pList->clear();
        m_p2rList->clear();
        for (auto *c : m_syn->PreToPostCurrents())
            m_p2pList->addItem(QString::fromStdWString(c->Name()));
        for (auto *c : m_syn->PostToPreCurrents())
            m_p2rList->addItem(QString::fromStdWString(c->Name()));
    }

    TCurrent* findCurrent(bool preToPost, const std::wstring &name) {
        // PreToPostCurrents()/PostToPreCurrents() return the vector by
        // value in this codebase, so we take a copy; the TCurrent* it
        // contains is still the live object.
        auto arr = preToPost ? m_syn->PreToPostCurrents()
                             : m_syn->PostToPreCurrents();
        for (auto *c : arr) if (c->Name() == name) return c;
        return nullptr;
    }

    // Dispatch to the right per-type dialog. Duplicates the small
    // switch from networkeditor.cpp rather than introducing a cross-
    // file dependency — three-case switch is cheaper than refactor.
    bool showCurrentDialog(TCurrent *cur) {
        if (!cur) return false;
        if (auto *hh = dynamic_cast<THHCurrent*>(cur)) {
            HHCurrentDialog dlg(hh, this);
            return dlg.exec() == QDialog::Accepted;
        }
        if (auto *hh2 = dynamic_cast<THH2Current*>(cur)) {
            HH2CurrentDialog dlg(hh2, this);
            return dlg.exec() == QDialog::Accepted;
        }
        if (auto *pid = dynamic_cast<TVoltageClampPIDCurrent*>(cur)) {
            VoltageClampPIDDialog dlg(pid, this);
            return dlg.exec() == QDialog::Accepted;
        }
        if (auto *pb = dynamic_cast<TPlaybackCurrent*>(cur)) {
            PlaybackCurrentDialog dlg(pb, this);
            return dlg.exec() == QDialog::Accepted;
        }
        return true;
    }

    TNetwork *m_net;
    TGenBiDirSynapse *m_syn;
    QListWidget *m_p2pList = nullptr;
    QListWidget *m_p2rList = nullptr;
};

#endif  // GENBIDIRSYNAPSEDIALOG_H
