#ifndef VOLTAGECLAMPPIDDIALOG_H
#define VOLTAGECLAMPPIDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_VoltageClampPIDCurrent.h"

class VoltageClampPIDDialog : public QDialog
{
    Q_OBJECT
public:
    VoltageClampPIDDialog(TVoltageClampPIDCurrent *cur, QWidget *parent = nullptr)
        : QDialog(parent), m_cur(cur)
    {
        setWindowTitle(QString("Voltage Clamp PID: %1").arg(QString::fromStdWString(cur->Name())));
        auto *layout = new QFormLayout(this);

        vCmdEdit = new QLineEdit(QString::number(cur->VCommand()));
        pEdit = new QLineEdit(QString::number(cur->pGain()));
        iEdit = new QLineEdit(QString::number(cur->iGain()));
        dEdit = new QLineEdit(QString::number(cur->dGain()));
        tauEdit = new QLineEdit(QString::number(cur->tau()));
        imaxEdit = new QLineEdit(QString::number(cur->iMax()));
        iminEdit = new QLineEdit(QString::number(cur->iMin()));

        layout->addRow("V command (mV):", vCmdEdit);
        layout->addRow("P gain:", pEdit);
        layout->addRow("I gain:", iEdit);
        layout->addRow("D gain:", dEdit);
        layout->addRow("Tau (ms):", tauEdit);
        layout->addRow("I max:", imaxEdit);
        layout->addRow("I min:", iminEdit);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &VoltageClampPIDDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addRow(buttons);
    }

private slots:
    void tryAccept() {
        bool ok;
        double vc = vCmdEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid V command"); return; }
        double pg = pEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid P gain"); return; }
        double ig = iEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid I gain"); return; }
        double dg = dEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid D gain"); return; }
        double tau = tauEdit->text().toDouble(&ok); if (!ok || tau <= 0) { QMessageBox::warning(this, "Error", "Tau must be > 0"); return; }
        double imax = imaxEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid I max"); return; }
        double imin = iminEdit->text().toDouble(&ok); if (!ok) { QMessageBox::warning(this, "Error", "Invalid I min"); return; }
        m_cur->VCommand(vc); m_cur->pGain(pg); m_cur->iGain(ig); m_cur->dGain(dg);
        m_cur->tau(tau); m_cur->iMax(imax); m_cur->iMin(imin);
        accept();
    }

private:
    TVoltageClampPIDCurrent *m_cur;
    QLineEdit *vCmdEdit, *pEdit, *iEdit, *dEdit, *tauEdit, *imaxEdit, *iminEdit;
};

#endif
