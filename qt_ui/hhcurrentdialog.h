#ifndef HHCURRENTDIALOG_H
#define HHCURRENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
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
    
    // Parameter inputs
    QLineEdit *gmaxEdit;
    QLineEdit *eEdit;
    QLineEdit *gnoiseEdit;
    QLineEdit *pEdit;
    QLineEdit *qEdit;
    QLineEdit *rEdit;
    
    // Kinetic factor inputs (m)
    QLineEdit *m_V0Edit;
    QLineEdit *m_kEdit;
    QLineEdit *m_tloEdit;
    QLineEdit *m_thiEdit;
    QLineEdit *m_infMinEdit;
    
    // Kinetic factor inputs (h)
    QLineEdit *h_V0Edit;
    QLineEdit *h_kEdit;
    QLineEdit *h_tloEdit;
    QLineEdit *h_thiEdit;
    QLineEdit *h_infMinEdit;
};

#endif // HHCURRENTDIALOG_H
