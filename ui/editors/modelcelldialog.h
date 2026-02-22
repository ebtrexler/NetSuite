#ifndef MODELCELLDIALOG_H
#define MODELCELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_ModelCell.h"

class ModelCellDialog : public QDialog
{
    Q_OBJECT
public:
    ModelCellDialog(TModelCell *cell, QWidget *parent = nullptr)
        : QDialog(parent), m_cell(cell)
    {
        setWindowTitle(QString("Model Cell: %1").arg(QString::fromStdWString(cell->Name())));
        auto *layout = new QFormLayout(this);
        
        capEdit = new QLineEdit(QString::number(cell->Capacitance()));
        vmEdit = new QLineEdit(QString::number(cell->InitialVm()));
        
        layout->addRow("Capacitance (nF):", capEdit);
        layout->addRow("Initial Vm (mV):", vmEdit);
        
        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &ModelCellDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addRow(buttons);
    }

private slots:
    void tryAccept() {
        bool ok1, ok2;
        double cap = capEdit->text().toDouble(&ok1);
        double vm = vmEdit->text().toDouble(&ok2);
        if (!ok1 || cap <= 0) { QMessageBox::warning(this, "Error", "Capacitance must be positive"); return; }
        if (!ok2) { QMessageBox::warning(this, "Error", "Invalid Vm"); return; }
        m_cell->SetCapacitance(cap);
        m_cell->SetInitialVm(vm);
        accept();
    }

private:
    TModelCell *m_cell;
    QLineEdit *capEdit, *vmEdit;
};

#endif
