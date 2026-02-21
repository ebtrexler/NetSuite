#include "hhcurrentdialog.h"

HHCurrentDialog::HHCurrentDialog(THHCurrent *current, QWidget *parent)
    : QDialog(parent), m_current(current)
{
    setWindowTitle(QString("HH Current: %1").arg(QString::fromStdWString(current->Name())));
    setMinimumWidth(400);
    
    auto *mainLayout = new QVBoxLayout(this);
    
    // Formula reminder
    auto *formula = new QLabel("I = Gmax · m<sup>p</sup> · h<sup>q</sup> · n<sup>r</sup> · (V - E)");
    formula->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(formula);
    
    // Basic parameters
    auto *basicGroup = new QGroupBox("Parameters");
    auto *basicLayout = new QFormLayout(basicGroup);
    
    gmaxEdit = new QLineEdit(QString::number(current->Gmax()));
    eEdit = new QLineEdit(QString::number(current->E()));
    gnoiseEdit = new QLineEdit(QString::number(current->Gnoise()));
    pEdit = new QLineEdit(QString::number(current->p()));
    qEdit = new QLineEdit(QString::number(current->q()));
    rEdit = new QLineEdit(QString::number(current->r()));
    
    basicLayout->addRow("Gmax (µS):", gmaxEdit);
    basicLayout->addRow("E reversal (mV):", eEdit);
    basicLayout->addRow("Gnoise (%):", gnoiseEdit);
    basicLayout->addRow("p (m exponent):", pEdit);
    basicLayout->addRow("q (h exponent):", qEdit);
    basicLayout->addRow("r (n exponent):", rEdit);
    mainLayout->addWidget(basicGroup);
    
    // Kinetics groups
    mainLayout->addWidget(makeKineticsGroup("m — Activation", current->get_m(), mEdits));
    mainLayout->addWidget(makeKineticsGroup("h — Inactivation", current->get_h(), hEdits));
    mainLayout->addWidget(makeKineticsGroup("n — Third factor", current->get_n(), nEdits));
    
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &HHCurrentDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);
}

QGroupBox* HHCurrentDialog::makeKineticsGroup(const QString &title, THHKineticsFactor &kf, KineticsEdits &edits)
{
    auto *group = new QGroupBox(title);
    auto *layout = new QFormLayout(group);
    
    edits.V0 = new QLineEdit(QString::number(kf.V0()));
    edits.k = new QLineEdit(QString::number(kf.k()));
    edits.t_lo = new QLineEdit(QString::number(kf.t_lo()));
    edits.t_hi = new QLineEdit(QString::number(kf.t_hi()));
    edits.infMin = new QLineEdit(QString::number(kf.infMin()));
    
    layout->addRow("V₀ half-act (mV):", edits.V0);
    layout->addRow("k slope:", edits.k);
    layout->addRow("τ min (ms):", edits.t_lo);
    layout->addRow("τ max (ms):", edits.t_hi);
    layout->addRow("inf min:", edits.infMin);
    
    return group;
}

void HHCurrentDialog::accept()
{
    bool ok;
    double gmax = gmaxEdit->text().toDouble(&ok);
    if (!ok || gmax < 0) { QMessageBox::warning(this, "Error", "Gmax must be >= 0"); return; }
    
    m_current->Gmax(gmax);
    m_current->E(eEdit->text().toDouble());
    m_current->Gnoise(gnoiseEdit->text().toDouble());
    m_current->p(pEdit->text().toDouble());
    m_current->q(qEdit->text().toDouble());
    m_current->r(rEdit->text().toDouble());
    
    auto applyKinetics = [](KineticsEdits &edits, THHKineticsFactor &kf) {
        kf.V0(edits.V0->text().toDouble());
        kf.k(edits.k->text().toDouble());
        kf.t_lo(edits.t_lo->text().toDouble());
        kf.t_hi(edits.t_hi->text().toDouble());
        kf.infMin(edits.infMin->text().toDouble());
    };
    
    applyKinetics(mEdits, m_current->get_m());
    applyKinetics(hEdits, m_current->get_h());
    applyKinetics(nEdits, m_current->get_n());
    
    QDialog::accept();
}
