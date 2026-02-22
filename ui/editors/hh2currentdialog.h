#ifndef HH2CURRENTDIALOG_H
#define HH2CURRENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include "RT_HH2Current.h"
#include "hhcurrentdialog.h" // reuse KineticsPlotWidget

class HH2CurrentDialog : public QDialog
{
    Q_OBJECT
public:
    HH2CurrentDialog(THH2Current *cur, QWidget *parent = nullptr)
        : QDialog(parent), m_cur(cur)
    {
        setWindowTitle(QString("HH2 Current: %1").arg(QString::fromStdWString(cur->Name())));
        resize(700, 600);
        auto *mainLayout = new QVBoxLayout(this);

        // Conductance params
        auto *condGroup = new QGroupBox("Conductance");
        auto *condForm = new QFormLayout(condGroup);
        gmaxEdit = new QLineEdit(QString::number(cur->Gmax()));
        eEdit = new QLineEdit(QString::number(cur->E()));
        gnoiseEdit = new QLineEdit(QString::number(cur->Gnoise()));
        modeCombo = new QComboBox();
        modeCombo->addItems({"Multiply gates", "Add conductances"});
        modeCombo->setCurrentIndex(cur->Add_Dont_Multiply());
        condForm->addRow("Gmax (µS):", gmaxEdit);
        condForm->addRow("E (mV):", eEdit);
        condForm->addRow("Gnoise (%):", gnoiseEdit);
        condForm->addRow("Mode:", modeCombo);
        // Secondary conductance (shown when Add mode)
        gmax2Edit = new QLineEdit(QString::number(cur->Gmax2()));
        e2Edit = new QLineEdit(QString::number(cur->E2()));
        gnoise2Edit = new QLineEdit(QString::number(cur->Gnoise2()));
        gmax2Label = new QLabel("Gmax2 (µS):");
        e2Label = new QLabel("E2 (mV):");
        gnoise2Label = new QLabel("Gnoise2 (%):");
        condForm->addRow(gmax2Label, gmax2Edit);
        condForm->addRow(e2Label, e2Edit);
        condForm->addRow(gnoise2Label, gnoise2Edit);
        mainLayout->addWidget(condGroup);
        updateSecondaryVisibility();
        connect(modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this](int) { updateSecondaryVisibility(); });

        // Tabs for pre/post kinetics
        tabs = new QTabWidget();
        tabs->addTab(createKineticsTab(cur->m_pre, cur->h_pre, cur->n_pre,
                                        cur->p_pre(), cur->q_pre(), cur->r_pre(),
                                        preEdits), "Pre-synaptic");
        tabs->addTab(createKineticsTab(cur->m_post, cur->h_post, cur->n_post,
                                        cur->p_post(), cur->q_post(), cur->r_post(),
                                        postEdits), "Post-synaptic");
        mainLayout->addWidget(tabs);

        // Plots
        ivPlot = new KineticsPlotWidget();
        ivPlot->setTitle("I-V"); ivPlot->setXLabel("V (mV)"); ivPlot->setYLabel("I (nA)");
        ivPlot->setXRange(-150, 150);
        infPlot = new KineticsPlotWidget();
        infPlot->setTitle("Steady-state"); infPlot->setXLabel("V (mV)"); infPlot->setYLabel("inf");
        infPlot->setXRange(-150, 150);
        tauPlot = new KineticsPlotWidget();
        tauPlot->setTitle("Time constants"); tauPlot->setXLabel("V (mV)"); tauPlot->setYLabel("tau (ms)");
        tauPlot->setXRange(-150, 150);
        auto *plotLayout = new QHBoxLayout();
        plotLayout->addWidget(ivPlot);
        plotLayout->addWidget(infPlot);
        plotLayout->addWidget(tauPlot);
        mainLayout->addLayout(plotLayout);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        buttons->button(QDialogButtonBox::Ok)->setAutoDefault(false);
        buttons->button(QDialogButtonBox::Cancel)->setAutoDefault(false);
        connect(buttons, &QDialogButtonBox::accepted, this, &HH2CurrentDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        mainLayout->addWidget(buttons);

        connect(tabs, &QTabWidget::currentChanged, this, [this](int) { updatePlots(); });
        updatePlots();
    }

protected:
    void keyPressEvent(QKeyEvent *e) override {
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
            updatePlots();
            e->accept();
            return;
        }
        QDialog::keyPressEvent(e);
    }

private:
    struct KineticsEdits {
        QLineEdit *V0m, *km, *tlom, *thim, *infMinm, *pm;
        QLineEdit *V0h, *kh, *tloh, *thih, *infMinh, *qe;
        QLineEdit *V0n, *kn, *tlon, *thin, *infMinn, *re;
    };

    QWidget *createKineticsTab(THHKineticsFactor &m, THHKineticsFactor &h, THHKineticsFactor &n,
                                double p, double q, double r, KineticsEdits &ed)
    {
        auto *w = new QWidget();
        auto *layout = new QHBoxLayout(w);
        auto makeGroup = [](const QString &title, THHKineticsFactor &f, double exp,
                            const QString &expLabel, QLineEdit *&V0, QLineEdit *&k,
                            QLineEdit *&tlo, QLineEdit *&thi, QLineEdit *&infMin, QLineEdit *&expEdit) {
            auto *g = new QGroupBox(title);
            auto *fl = new QFormLayout(g);
            V0 = new QLineEdit(QString::number(f.V0()));
            k = new QLineEdit(QString::number(f.k()));
            tlo = new QLineEdit(QString::number(f.t_lo()));
            thi = new QLineEdit(QString::number(f.t_hi()));
            infMin = new QLineEdit(QString::number(f.infMin()));
            expEdit = new QLineEdit(QString::number(exp));
            fl->addRow("V½ (mV):", V0);
            fl->addRow("k:", k);
            fl->addRow("τ_lo (ms):", tlo);
            fl->addRow("τ_hi (ms):", thi);
            fl->addRow("infMin:", infMin);
            fl->addRow(expLabel + ":", expEdit);
            return g;
        };
        layout->addWidget(makeGroup("m", m, p, "p", ed.V0m, ed.km, ed.tlom, ed.thim, ed.infMinm, ed.pm));
        layout->addWidget(makeGroup("h", h, q, "q", ed.V0h, ed.kh, ed.tloh, ed.thih, ed.infMinh, ed.qe));
        layout->addWidget(makeGroup("n", n, r, "r", ed.V0n, ed.kn, ed.tlon, ed.thin, ed.infMinn, ed.re));
        return w;
    }

    void readKinetics(KineticsEdits &ed, THHKineticsFactor &m, THHKineticsFactor &h, THHKineticsFactor &n,
                      double &p, double &q, double &r) {
        m.V0(ed.V0m->text().toDouble()); m.k(ed.km->text().toDouble());
        m.t_lo(ed.tlom->text().toDouble()); m.t_hi(ed.thim->text().toDouble());
        m.infMin(ed.infMinm->text().toDouble());
        p = ed.pm->text().toDouble();

        h.V0(ed.V0h->text().toDouble()); h.k(ed.kh->text().toDouble());
        h.t_lo(ed.tloh->text().toDouble()); h.t_hi(ed.thih->text().toDouble());
        h.infMin(ed.infMinh->text().toDouble());
        q = ed.qe->text().toDouble();

        n.V0(ed.V0n->text().toDouble()); n.k(ed.kn->text().toDouble());
        n.t_lo(ed.tlon->text().toDouble()); n.t_hi(ed.thin->text().toDouble());
        n.infMin(ed.infMinn->text().toDouble());
        r = ed.re->text().toDouble();
    }

    void updateSecondaryVisibility() {
        bool show = modeCombo->currentIndex() == 1;
        gmax2Edit->setVisible(show); gmax2Label->setVisible(show);
        e2Edit->setVisible(show); e2Label->setVisible(show);
        gnoise2Edit->setVisible(show); gnoise2Label->setVisible(show);
    }

    void updatePlots() {
        // Temporarily apply UI values to compute curves
        double pp, qp, rp, ppo, qpo, rpo;
        THHKineticsFactor tm_pre = m_cur->m_pre, th_pre = m_cur->h_pre, tn_pre = m_cur->n_pre;
        THHKineticsFactor tm_post = m_cur->m_post, th_post = m_cur->h_post, tn_post = m_cur->n_post;
        readKinetics(preEdits, tm_pre, th_pre, tn_pre, pp, qp, rp);
        readKinetics(postEdits, tm_post, th_post, tn_post, ppo, qpo, rpo);

        // Pick which tab is active for plotting
        THHKineticsFactor *m, *h, *n;
        double p, q, r;
        if (tabs->currentIndex() == 0) { m = &tm_pre; h = &th_pre; n = &tn_pre; p = pp; q = qp; r = rp; }
        else { m = &tm_post; h = &th_post; n = &tn_post; p = ppo; q = qpo; r = rpo; }

        double gmax = gmaxEdit->text().toDouble();
        double erev = eEdit->text().toDouble();

        ivPlot->clearTraces(); infPlot->clearTraces(); tauPlot->clearTraces();
        std::vector<double> iv, mi, hi, ni, mt, ht, nt;
        for (int i = 0; i <= 300; i++) {
            double V = -150.0 + i;
            double minf = m->inf(V), hinf = h->inf(V), ninf = n->inf(V);
            double mp = p == 0 ? 1 : pow(minf, p);
            double hq = q == 0 ? 1 : pow(hinf, q);
            double nr = r == 0 ? 1 : pow(ninf, r);
            iv.push_back(gmax * mp * hq * nr * (V - erev));
            mi.push_back(minf); hi.push_back(hinf); ni.push_back(ninf);
            mt.push_back(m->tau(V)); ht.push_back(h->tau(V)); nt.push_back(n->tau(V));
        }
        ivPlot->addTrace(iv, Qt::black, "I-V");
        infPlot->addTrace(mi, Qt::red, "m"); infPlot->addTrace(hi, Qt::blue, "h"); infPlot->addTrace(ni, Qt::darkGreen, "n");
        tauPlot->addTrace(mt, Qt::red, "m"); tauPlot->addTrace(ht, Qt::blue, "h"); tauPlot->addTrace(nt, Qt::darkGreen, "n");
        ivPlot->replot(); infPlot->replot(); tauPlot->replot();
    }

    void tryAccept() {
        double pp, qp, rp, ppo, qpo, rpo;
        readKinetics(preEdits, m_cur->m_pre, m_cur->h_pre, m_cur->n_pre, pp, qp, rp);
        readKinetics(postEdits, m_cur->m_post, m_cur->h_post, m_cur->n_post, ppo, qpo, rpo);
        m_cur->p_pre(pp); m_cur->q_pre(qp); m_cur->r_pre(rp);
        m_cur->p_post(ppo); m_cur->q_post(qpo); m_cur->r_post(rpo);
        m_cur->Gmax(gmaxEdit->text().toDouble());
        m_cur->E(eEdit->text().toDouble());
        m_cur->Gnoise(gnoiseEdit->text().toDouble());
        m_cur->Add_Dont_Multiply(modeCombo->currentIndex());
        if (modeCombo->currentIndex() == 1) {
            m_cur->Gmax2(gmax2Edit->text().toDouble());
            m_cur->E2(e2Edit->text().toDouble());
            m_cur->Gnoise2(gnoise2Edit->text().toDouble());
        }
        accept();
    }

    THH2Current *m_cur;
    QLineEdit *gmaxEdit, *eEdit, *gnoiseEdit;
    QLineEdit *gmax2Edit, *e2Edit, *gnoise2Edit;
    QLabel *gmax2Label, *e2Label, *gnoise2Label;
    QComboBox *modeCombo;
    QTabWidget *tabs;
    KineticsEdits preEdits, postEdits;
    KineticsPlotWidget *ivPlot, *infPlot, *tauPlot;
};

#endif
