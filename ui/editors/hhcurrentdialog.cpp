#include "hhcurrentdialog.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QMessageBox>

HHCurrentDialog::HHCurrentDialog(THHCurrent *current, QWidget *parent)
    : QDialog(parent), m_current(current)
{
    setWindowTitle("HH Current Parameters");
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Basic parameters group
    QGroupBox *basicGroup = new QGroupBox("Basic Parameters");
    QFormLayout *basicLayout = new QFormLayout(basicGroup);
    
    gmaxEdit = new QLineEdit(QString::number(current->Gmax()));
    eEdit = new QLineEdit(QString::number(current->E()));
    gnoiseEdit = new QLineEdit(QString::number(current->Gnoise()));
    pEdit = new QLineEdit(QString::number(current->p()));
    qEdit = new QLineEdit(QString::number(current->q()));
    rEdit = new QLineEdit(QString::number(current->r()));
    
    basicLayout->addRow("Gmax (μS):", gmaxEdit);
    basicLayout->addRow("E (mV):", eEdit);
    basicLayout->addRow("Gnoise (%):", gnoiseEdit);
    basicLayout->addRow("p (exponent):", pEdit);
    basicLayout->addRow("q (exponent):", qEdit);
    basicLayout->addRow("r (exponent):", rEdit);
    
    mainLayout->addWidget(basicGroup);
    
    // m kinetic factor group
    QGroupBox *mGroup = new QGroupBox("m Kinetic Factor");
    QFormLayout *mLayout = new QFormLayout(mGroup);
    
    THHKineticsFactor &m = current->get_m();
    m_V0Edit = new QLineEdit(QString::number(m.V0()));
    m_kEdit = new QLineEdit(QString::number(m.k()));
    m_tloEdit = new QLineEdit(QString::number(m.t_lo()));
    m_thiEdit = new QLineEdit(QString::number(m.t_hi()));
    m_infMinEdit = new QLineEdit(QString::number(m.infMin()));
    
    mLayout->addRow("V₀ (mV):", m_V0Edit);
    mLayout->addRow("k (steepness):", m_kEdit);
    mLayout->addRow("τ_lo (ms):", m_tloEdit);
    mLayout->addRow("τ_hi (ms):", m_thiEdit);
    mLayout->addRow("inf_min:", m_infMinEdit);
    
    mainLayout->addWidget(mGroup);
    
    // h kinetic factor group
    QGroupBox *hGroup = new QGroupBox("h Kinetic Factor");
    QFormLayout *hLayout = new QFormLayout(hGroup);
    
    THHKineticsFactor &h = current->get_h();
    h_V0Edit = new QLineEdit(QString::number(h.V0()));
    h_kEdit = new QLineEdit(QString::number(h.k()));
    h_tloEdit = new QLineEdit(QString::number(h.t_lo()));
    h_thiEdit = new QLineEdit(QString::number(h.t_hi()));
    h_infMinEdit = new QLineEdit(QString::number(h.infMin()));
    
    hLayout->addRow("V₀ (mV):", h_V0Edit);
    hLayout->addRow("k (steepness):", h_kEdit);
    hLayout->addRow("τ_lo (ms):", h_tloEdit);
    hLayout->addRow("τ_hi (ms):", h_thiEdit);
    hLayout->addRow("inf_min:", h_infMinEdit);
    
    mainLayout->addWidget(hGroup);
    
    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &HHCurrentDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    mainLayout->addWidget(buttonBox);
}

void HHCurrentDialog::accept()
{
    bool ok;
    
    // Validate and set basic parameters
    double gmax = gmaxEdit->text().toDouble(&ok);
    if (!ok || gmax < 0) {
        QMessageBox::warning(this, "Invalid Input", "Gmax must be a non-negative number");
        return;
    }
    
    double e = eEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "E must be a valid number");
        return;
    }
    
    double gnoise = gnoiseEdit->text().toDouble(&ok);
    if (!ok || gnoise < 0) {
        QMessageBox::warning(this, "Invalid Input", "Gnoise must be a non-negative number");
        return;
    }
    
    double p = pEdit->text().toDouble(&ok);
    if (!ok || p < 0) {
        QMessageBox::warning(this, "Invalid Input", "p must be a non-negative number");
        return;
    }
    
    double q = qEdit->text().toDouble(&ok);
    if (!ok || q < 0) {
        QMessageBox::warning(this, "Invalid Input", "q must be a non-negative number");
        return;
    }
    
    double r = rEdit->text().toDouble(&ok);
    if (!ok || r < 0) {
        QMessageBox::warning(this, "Invalid Input", "r must be a non-negative number");
        return;
    }
    
    // Set basic parameters
    m_current->Gmax(gmax);
    m_current->E(e);
    m_current->Gnoise(gnoise);
    m_current->p(p);
    m_current->q(q);
    m_current->r(r);
    
    // Set m kinetic factor
    THHKineticsFactor &m = m_current->get_m();
    m.V0(m_V0Edit->text().toDouble());
    m.k(m_kEdit->text().toDouble());
    m.t_lo(m_tloEdit->text().toDouble());
    m.t_hi(m_thiEdit->text().toDouble());
    m.infMin(m_infMinEdit->text().toDouble());
    
    // Set h kinetic factor
    THHKineticsFactor &h = m_current->get_h();
    h.V0(h_V0Edit->text().toDouble());
    h.k(h_kEdit->text().toDouble());
    h.t_lo(h_tloEdit->text().toDouble());
    h.t_hi(h_thiEdit->text().toDouble());
    h.infMin(h_infMinEdit->text().toDouble());
    
    QDialog::accept();
}
