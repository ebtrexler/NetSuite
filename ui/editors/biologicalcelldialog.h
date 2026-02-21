#ifndef BIOLOGICALCELLDIALOG_H
#define BIOLOGICALCELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "RT_BiologicalCell.h"

class BiologicalCellDialog : public QDialog
{
    Q_OBJECT
public:
    BiologicalCellDialog(TBiologicalCell *cell, QWidget *parent = nullptr)
        : QDialog(parent), m_cell(cell)
    {
        setWindowTitle(QString("Biological Cell: %1").arg(QString::fromStdWString(cell->Name())));
        auto *layout = new QFormLayout(this);

        posLimitEdit = new QLineEdit(QString::number(cell->PosCurrentLimit()));
        negLimitEdit = new QLineEdit(QString::number(cell->NegCurrentLimit()));

        layout->addRow("Positive current limit (nA):", posLimitEdit);
        layout->addRow("Negative current limit (nA):", negLimitEdit);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttons, &QDialogButtonBox::accepted, this, &BiologicalCellDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addRow(buttons);
    }

private slots:
    void tryAccept() {
        bool ok1, ok2;
        double pos = posLimitEdit->text().toDouble(&ok1);
        double neg = negLimitEdit->text().toDouble(&ok2);
        if (!ok1 || pos < 0) { QMessageBox::warning(this, "Error", "Positive limit must be >= 0"); return; }
        if (!ok2 || neg > 0) { QMessageBox::warning(this, "Error", "Negative limit must be <= 0"); return; }
        m_cell->SetPosCurrentLimit(pos);
        m_cell->SetNegCurrentLimit(neg);
        accept();
    }

private:
    TBiologicalCell *m_cell;
    QLineEdit *posLimitEdit, *negLimitEdit;
};

#endif
