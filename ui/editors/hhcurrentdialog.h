#ifndef HHCURRENTDIALOG_H
#define HHCURRENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLabel>
#include "RT_HHCurrent.h"

class HHCurrentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HHCurrentDialog(THHCurrent *current, QWidget *parent = nullptr);
private slots:
    void accept() override;
private:
    THHCurrent *m_current;
    QLineEdit *gmaxEdit, *eEdit, *gnoiseEdit, *pEdit, *qEdit, *rEdit;
    
    struct KineticsEdits {
        QLineEdit *V0, *k, *t_lo, *t_hi, *infMin;
    };
    KineticsEdits mEdits, hEdits, nEdits;
    
    QGroupBox* makeKineticsGroup(const QString &title, THHKineticsFactor &kf, KineticsEdits &edits);
};

#endif
