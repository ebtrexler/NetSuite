#ifndef BIOLOGICALCELLDIALOG_H
#define BIOLOGICALCELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_BiologicalCell.h"

class BiologicalCellDialog : public QDialog
{
    Q_OBJECT
public:
    BiologicalCellDialog(TBiologicalCell *cell, QWidget *parent = nullptr)
        : QDialog(parent), m_cell(cell), m_showingAmpGains(false)
    {
        setWindowTitle(QString("Biological Cell: %1").arg(QString::fromStdWString(cell->Name())));
        auto *mainLayout = new QVBoxLayout(this);

        // --- AI/AO channel lists ---
        auto *chanLayout = new QHBoxLayout;

        auto *aiBox = new QVBoxLayout;
        aiBox->addWidget(new QLabel("Voltage Sampling"));
        aiBox->addWidget(new QLabel("Analog Input Channels"));
        aiChannelList = new QListWidget;
        aiBox->addWidget(aiChannelList);
        chanLayout->addLayout(aiBox);

        auto *aoBox = new QVBoxLayout;
        aoBox->addWidget(new QLabel("Current Injection"));
        aoBox->addWidget(new QLabel("Analog Output Channels"));
        aoChannelList = new QListWidget;
        aoBox->addWidget(aoChannelList);
        chanLayout->addLayout(aoBox);

        mainLayout->addLayout(chanLayout);

        // --- Gain group ---
        auto *gainGroup = new QGroupBox("Gain Settings");
        auto *gainLayout = new QFormLayout(gainGroup);

        toggleBtn = new QPushButton("Switch to Amplifier Gains");
        gainLayout->addRow(toggleBtn);

        gainLabel1 = new QLabel;
        gainLabel2 = new QLabel;
        gainEdit1 = new QLineEdit;
        gainEdit2 = new QLineEdit;
        gainLayout->addRow(gainLabel1, gainEdit1);
        gainLayout->addRow(gainLabel2, gainEdit2);

        connect(toggleBtn, &QPushButton::clicked, this, &BiologicalCellDialog::toggleGainMode);

        mainLayout->addWidget(gainGroup);

        // --- Current limits group ---
        auto *limitGroup = new QGroupBox("Current Injection Limits");
        auto *limitLayout = new QFormLayout(limitGroup);
        posLimitEdit = new QLineEdit;
        negLimitEdit = new QLineEdit;
        limitLayout->addRow("Positive limit (nA):", posLimitEdit);
        limitLayout->addRow("Negative limit (nA):", negLimitEdit);
        mainLayout->addWidget(limitGroup);

        // --- Buttons ---
        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &BiologicalCellDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        mainLayout->addWidget(buttons);

        populateChannels();
        selectChannel(aiChannelList, QString::fromStdWString(cell->AIChannel()));
        selectChannel(aoChannelList, QString::fromStdWString(cell->AOChannel()));

        // Start in network gain mode
        setNetworkGainLabels();
        gainEdit1->setText(QString::number(cell->AIGain()));
        gainEdit2->setText(QString::number(cell->AOGain()));

        posLimitEdit->setText(QString::number(cell->PosCurrentLimit()));
        negLimitEdit->setText(QString::number(cell->NegCurrentLimit()));

        resize(400, 520);
    }

private slots:
    void toggleGainMode() {
        bool ok1, ok2;
        double v1 = gainEdit1->text().toDouble(&ok1);
        double v2 = gainEdit2->text().toDouble(&ok2);

        if (m_showingAmpGains) {
            // Amp → Network: AIGain = 1000/ampGain, AOGain = 1/extCmd
            if (ok1 && v1 != 0) gainEdit1->setText(QString::number(1000.0 / v1, 'g', 5));
            if (ok2 && v2 != 0) gainEdit2->setText(QString::number(1.0 / v2, 'g', 5));
            setNetworkGainLabels();
        } else {
            // Network → Amp: ampGain = 1000/AIGain, extCmd = 1/AOGain
            if (ok1 && v1 != 0) gainEdit1->setText(QString::number(1000.0 / v1, 'g', 5));
            if (ok2 && v2 != 0) gainEdit2->setText(QString::number(1.0 / v2, 'g', 5));
            setAmpGainLabels();
        }
        m_showingAmpGains = !m_showingAmpGains;
    }

    void tryAccept() {
        // Convert to network gains if currently showing amp gains
        double aiGain, aoGain;
        bool ok1, ok2;
        double v1 = gainEdit1->text().toDouble(&ok1);
        double v2 = gainEdit2->text().toDouble(&ok2);
        if (!ok1 || !ok2) { QMessageBox::warning(this, "Error", "Invalid gain value"); return; }

        if (m_showingAmpGains) {
            if (v1 == 0 || v2 == 0) { QMessageBox::warning(this, "Error", "Gain cannot be zero"); return; }
            aiGain = 1000.0 / v1;
            aoGain = 1.0 / v2;
        } else {
            aiGain = v1;
            aoGain = v2;
        }
        if (aiGain < 0) { QMessageBox::warning(this, "Error", "Input Gain must be >= 0"); return; }
        if (aoGain < 0) { QMessageBox::warning(this, "Error", "Output Gain must be >= 0"); return; }

        bool ok;
        double pos = posLimitEdit->text().toDouble(&ok);
        if (!ok || pos < 0) { QMessageBox::warning(this, "Error", "Positive limit must be >= 0"); return; }
        double neg = negLimitEdit->text().toDouble(&ok);
        if (!ok || neg > 0) { QMessageBox::warning(this, "Error", "Negative limit must be <= 0"); return; }

        if (aiChannelList->currentItem())
            m_cell->SetAIChannel(aiChannelList->currentItem()->text().toStdWString());
        if (aoChannelList->currentItem())
            m_cell->SetAOChannel(aoChannelList->currentItem()->text().toStdWString());

        m_cell->SetAIGain(aiGain);
        m_cell->SetAOGain(aoGain);
        m_cell->SetPosCurrentLimit(pos);
        m_cell->SetNegCurrentLimit(neg);
        accept();
    }

private:
    void setNetworkGainLabels() {
        gainLabel1->setText("Gain (cell mV / ADC V):");
        gainLabel2->setText("Gain (DAC V / cell nA):");
        toggleBtn->setText("Switch to Amplifier Gains");
    }
    void setAmpGainLabels() {
        gainLabel1->setText("Amp Gain (cell mV / Amp mV):");
        gainLabel2->setText("Ext. Command Sens. (nA/V):");
        toggleBtn->setText("Switch to Network Gains");
    }

    void populateChannels() {
        for (int i = 0; i < 8; ++i) {
            aiChannelList->addItem(QString("Dev1/ai%1").arg(i));
            aoChannelList->addItem(QString("Dev1/ao%1").arg(i));
        }
    }
    void selectChannel(QListWidget *list, const QString &name) {
        auto items = list->findItems(name, Qt::MatchExactly);
        if (!items.isEmpty()) list->setCurrentItem(items.first());
    }

    TBiologicalCell *m_cell;
    bool m_showingAmpGains;
    QListWidget *aiChannelList, *aoChannelList;
    QPushButton *toggleBtn;
    QLabel *gainLabel1, *gainLabel2;
    QLineEdit *gainEdit1, *gainEdit2;
    QLineEdit *posLimitEdit, *negLimitEdit;
};

#endif
