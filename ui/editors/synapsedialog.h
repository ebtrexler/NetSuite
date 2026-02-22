#ifndef SYNAPSEDIALOG_H
#define SYNAPSEDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_Synapse.h"
#include "RT_GapJunctionSynapse.h"
#include "RT_GJCurrent.h"

class GapJunctionSynapseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GapJunctionSynapseDialog(TGapJunctionSynapse *syn, QWidget *parent = nullptr)
        : QDialog(parent), m_syn(syn)
    {
        setWindowTitle(QString("Gap Junction: %1").arg(QString::fromStdWString(syn->Name())));

        auto *layout = new QVBoxLayout(this);

        auto *info = new QLabel(QString("Pre: %1  →  Post: %2")
            .arg(QString::fromStdWString(syn->Pre()->Name()))
            .arg(QString::fromStdWString(syn->Post()->Name())));
        info->setAlignment(Qt::AlignCenter);
        layout->addWidget(info);

        // Pre→Post conductance
        auto *g1 = new QGroupBox(QString("Pre→Post (%1 → %2)")
            .arg(QString::fromStdWString(syn->Pre()->Name()))
            .arg(QString::fromStdWString(syn->Post()->Name())));
        auto *l1 = new QFormLayout(g1);
        m_pre2post = getGJCurrent(syn->PreToPostCurrents());
        m_gPre2Post = new QLineEdit(QString::number(m_pre2post ? m_pre2post->Gmax() : 0));
        l1->addRow("Gmax (µS):", m_gPre2Post);
        layout->addWidget(g1);

        // Post→Pre conductance
        auto *g2 = new QGroupBox(QString("Post→Pre (%1 → %2)")
            .arg(QString::fromStdWString(syn->Post()->Name()))
            .arg(QString::fromStdWString(syn->Pre()->Name())));
        auto *l2 = new QFormLayout(g2);
        m_post2pre = getGJCurrent(syn->PostToPreCurrents());
        m_gPost2Pre = new QLineEdit(QString::number(m_post2pre ? m_post2pre->Gmax() : 0));
        l2->addRow("Gmax (µS):", m_gPost2Pre);
        layout->addWidget(g2);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &GapJunctionSynapseDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

private slots:
    void accept() override {
        bool ok1, ok2;
        double g1 = m_gPre2Post->text().toDouble(&ok1);
        double g2 = m_gPost2Pre->text().toDouble(&ok2);
        if (!ok1 || !ok2 || g1 < 0 || g2 < 0) {
            QMessageBox::warning(this, "Error", "Conductance must be >= 0");
            return;
        }
        if (m_pre2post) m_pre2post->SetGmax(g1);
        if (m_post2pre) m_post2pre->SetGmax(g2);
        QDialog::accept();
    }

private:
    TGapJunctionSynapse *m_syn;
    TGapJunctionCurrent *m_pre2post = nullptr;
    TGapJunctionCurrent *m_post2pre = nullptr;
    QLineEdit *m_gPre2Post, *m_gPost2Pre;

    TGapJunctionCurrent* getGJCurrent(const TCurrentsArray &arr) {
        if (!arr.empty()) return dynamic_cast<TGapJunctionCurrent*>(arr[0]);
        return nullptr;
    }
};

#endif
