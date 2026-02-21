#ifndef ELECTRODEDIALOG_H
#define ELECTRODEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_InjectionElectrode.h"

class ElectrodeDialog : public QDialog
{
    Q_OBJECT
public:
    ElectrodeDialog(TInjectionElectrode *el, QWidget *parent = nullptr)
        : QDialog(parent), m_el(el)
    {
        setWindowTitle(QString("Electrode: %1").arg(QString::fromStdWString(el->Name())));
        auto *layout = new QFormLayout(this);
        
        ampEdit = new QLineEdit(QString::number(el->Amplitude()));
        durEdit = new QLineEdit(QString::number(el->Duration()));
        delayEdit = new QLineEdit(QString::number(el->Delay()));
        initDelayEdit = new QLineEdit(QString::number(el->InitDelay()));
        repeatsSpin = new QSpinBox();
        repeatsSpin->setRange(-1, 100000);
        repeatsSpin->setValue(el->NumRepeats());
        repeatsSpin->setSpecialValueText("forever");
        
        layout->addRow("Amplitude (nA):", ampEdit);
        layout->addRow("Pulse duration (ms):", durEdit);
        layout->addRow("Interpulse interval (ms):", delayEdit);
        layout->addRow("Initial delay (ms):", initDelayEdit);
        layout->addRow("Repeats (-1 = forever):", repeatsSpin);
        
        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &ElectrodeDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addRow(buttons);
    }

private slots:
    void tryAccept() {
        bool ok;
        double amp = ampEdit->text().toDouble(&ok);
        if (!ok) { QMessageBox::warning(this, "Error", "Invalid amplitude"); return; }
        double dur = durEdit->text().toDouble(&ok);
        if (!ok || dur < 0) { QMessageBox::warning(this, "Error", "Duration must be >= 0"); return; }
        double delay = delayEdit->text().toDouble(&ok);
        if (!ok || delay < 0) { QMessageBox::warning(this, "Error", "Delay must be >= 0"); return; }
        double initDelay = initDelayEdit->text().toDouble(&ok);
        if (!ok || initDelay < 0) { QMessageBox::warning(this, "Error", "Initial delay must be >= 0"); return; }
        
        m_el->SetAmplitude(amp);
        m_el->SetDuration(dur);
        m_el->SetDelay(delay);
        m_el->SetInitDelay(initDelay);
        m_el->SetNumRepeats(repeatsSpin->value());
        accept();
    }

private:
    TInjectionElectrode *m_el;
    QLineEdit *ampEdit, *durEdit, *delayEdit, *initDelayEdit;
    QSpinBox *repeatsSpin;
};

#endif
