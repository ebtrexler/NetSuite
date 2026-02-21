#include "hhcurrentdialog.h"

static const int NumPts = 301;
static const double VStart = -150.0;
static const double VStep = 1.0;

HHCurrentDialog::HHCurrentDialog(THHCurrent *current, QWidget *parent)
    : QDialog(parent), m_current(current)
{
    setWindowTitle(QString("HH Current: %1").arg(QString::fromStdWString(current->Name())));
    resize(750, 600);

    auto *topLayout = new QHBoxLayout(this);

    // === Left side: parameters ===
    auto *leftWidget = new QWidget;
    leftWidget->setMaximumWidth(300);
    auto *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    auto *formula = new QLabel("I = Gmax · m<sup>p</sup> · h<sup>q</sup> · n<sup>r</sup> · (V - E)");
    formula->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(formula);

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
    leftLayout->addWidget(basicGroup);

    leftLayout->addWidget(makeKineticsGroup("m — Activation", current->get_m(), mEdits));
    leftLayout->addWidget(makeKineticsGroup("h — Inactivation", current->get_h(), hEdits));
    leftLayout->addWidget(makeKineticsGroup("n — Third factor", current->get_n(), nEdits));

    auto *updateBtn = new QPushButton("Update Plots");
    connect(updateBtn, &QPushButton::clicked, this, &HHCurrentDialog::updatePlots);
    leftLayout->addWidget(updateBtn);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &HHCurrentDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    leftLayout->addWidget(buttons);

    topLayout->addWidget(leftWidget);

    // === Right side: plots ===
    auto *plotLayout = new QVBoxLayout;

    plotIV = new KineticsPlotWidget;
    plotIV->setTitle("Steady-State I-V");
    plotIV->setXLabel("mV");
    plotIV->setYLabel("I (nA)");
    plotIV->setXRange(VStart, VStart + (NumPts - 1) * VStep);

    plotInf = new KineticsPlotWidget;
    plotInf->setTitle("Steady-State Activation");
    plotInf->setXLabel("mV");
    plotInf->setYLabel("inf");
    plotInf->setXRange(VStart, VStart + (NumPts - 1) * VStep);

    plotTau = new KineticsPlotWidget;
    plotTau->setTitle("Time Constants");
    plotTau->setXLabel("mV");
    plotTau->setYLabel("τ (ms)");
    plotTau->setXRange(VStart, VStart + (NumPts - 1) * VStep);

    plotLayout->addWidget(plotIV, 1);
    plotLayout->addWidget(plotInf, 1);
    plotLayout->addWidget(plotTau, 1);
    topLayout->addLayout(plotLayout, 1);

    // Connect Enter key in all line edits to update plots
    auto allEdits = findChildren<QLineEdit*>();
    for (auto *edit : allEdits) {
        connect(edit, &QLineEdit::returnPressed, this, &HHCurrentDialog::updatePlots);
    }

    updatePlots();
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

void HHCurrentDialog::readKinetics(KineticsEdits &edits, THHKineticsFactor &kf)
{
    kf.V0(edits.V0->text().toDouble());
    kf.k(edits.k->text().toDouble());
    kf.t_lo(edits.t_lo->text().toDouble());
    kf.t_hi(edits.t_hi->text().toDouble());
    kf.infMin(edits.infMin->text().toDouble());
}

void HHCurrentDialog::updatePlots()
{
    // Temporarily apply current params to compute curves
    double origGmax = m_current->Gmax(), origE = m_current->E();
    double origP = m_current->p(), origQ = m_current->q(), origR = m_current->r();

    // Save original kinetics
    auto saveKF = [](THHKineticsFactor &kf) {
        return std::make_tuple(kf.V0(), kf.k(), kf.t_lo(), kf.t_hi(), kf.infMin());
    };
    auto restoreKF = [](THHKineticsFactor &kf, std::tuple<double,double,double,double,double> &s) {
        kf.V0(std::get<0>(s)); kf.k(std::get<1>(s)); kf.t_lo(std::get<2>(s));
        kf.t_hi(std::get<3>(s)); kf.infMin(std::get<4>(s));
    };
    auto sm = saveKF(m_current->get_m());
    auto sh = saveKF(m_current->get_h());
    auto sn = saveKF(m_current->get_n());

    // Apply UI values
    m_current->Gmax(gmaxEdit->text().toDouble());
    m_current->E(eEdit->text().toDouble());
    m_current->p(pEdit->text().toDouble());
    m_current->q(qEdit->text().toDouble());
    m_current->r(rEdit->text().toDouble());
    readKinetics(mEdits, m_current->get_m());
    readKinetics(hEdits, m_current->get_h());
    readKinetics(nEdits, m_current->get_n());

    double Gmax = m_current->Gmax(), E = m_current->E();
    double p = m_current->p(), q = m_current->q(), r = m_current->r();

    std::vector<double> ivData(NumPts), mInf(NumPts), hInf(NumPts), nInf(NumPts);
    std::vector<double> mTau(NumPts), hTau(NumPts), nTau(NumPts);

    for (int i = 0; i < NumPts; i++) {
        double V = VStart + i * VStep;
        mInf[i] = m_current->get_m().inf(V);
        hInf[i] = m_current->get_h().inf(V);
        nInf[i] = m_current->get_n().inf(V);
        mTau[i] = m_current->get_m().tau(V);
        hTau[i] = m_current->get_h().tau(V);
        nTau[i] = m_current->get_n().tau(V);

        double mp = p <= 0 ? 1.0 : std::pow(mInf[i], p);
        double hq = q <= 0 ? 1.0 : std::pow(hInf[i], q);
        double nr = r <= 0 ? 1.0 : std::pow(nInf[i], r);
        ivData[i] = Gmax * mp * hq * nr * (V - E);
    }

    // Restore original values (user hasn't clicked OK yet)
    m_current->Gmax(origGmax); m_current->E(origE);
    m_current->p(origP); m_current->q(origQ); m_current->r(origR);
    restoreKF(m_current->get_m(), sm);
    restoreKF(m_current->get_h(), sh);
    restoreKF(m_current->get_n(), sn);

    // Update plot widgets
    plotIV->clearTraces();
    plotIV->addTrace(ivData, Qt::black, "I-V");
    plotIV->replot();

    plotInf->clearTraces();
    plotInf->addTrace(mInf, Qt::red, "m∞");
    plotInf->addTrace(hInf, Qt::blue, "h∞");
    plotInf->addTrace(nInf, QColor(0, 160, 0), "n∞");
    plotInf->replot();

    plotTau->clearTraces();
    plotTau->addTrace(mTau, Qt::red, "τm");
    plotTau->addTrace(hTau, Qt::blue, "τh");
    plotTau->addTrace(nTau, QColor(0, 160, 0), "τn");
    plotTau->replot();
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

    readKinetics(mEdits, m_current->get_m());
    readKinetics(hEdits, m_current->get_h());
    readKinetics(nEdits, m_current->get_n());

    QDialog::accept();
}
