#ifndef RT_HH2CURRENT_H
#define RT_HH2CURRENT_H

#include "RT_Current.h"
#include "RT_HHKineticsFactor.h"
#include "RT_Utilities.h"
#include <cmath>
#include <cstdlib>

#define THH2Current_KEY L"HH 2 Currents Convolved"

/// HH current with separate pre/post kinetics and optional add/multiply mode
class THH2Current : public TCurrent
{
private:
    double F_p_pre, F_q_pre, F_r_pre;
    double F_p_post, F_q_post, F_r_post;
    double F_E, F_Gmax, F_Gnoise;
    int    F_Add_Dont_Multiply; // 1 = add conductances, 0 = multiply gates
    double F_E2, F_Gmax2, F_Gnoise2;
    long   idum;

protected:
    void WriteToStream(std::ostream &) const override {}
    void ReadFromStream(std::istream &) override {}

public:
    THHKineticsFactor m_pre, h_pre, n_pre;
    THHKineticsFactor m_post, h_post, n_post;

    // Pre kinetics exponents
    double p_pre() const { return F_p_pre; }
    void p_pre(double v) { F_p_pre = v; }
    double q_pre() const { return F_q_pre; }
    void q_pre(double v) { F_q_pre = v; }
    double r_pre() const { return F_r_pre; }
    void r_pre(double v) { F_r_pre = v; }

    // Post kinetics exponents
    double p_post() const { return F_p_post; }
    void p_post(double v) { F_p_post = v; }
    double q_post() const { return F_q_post; }
    void q_post(double v) { F_q_post = v; }
    double r_post() const { return F_r_post; }
    void r_post(double v) { F_r_post = v; }

    // Conductance params (primary)
    double E() const { return F_E; }
    void E(double v) { F_E = v; }
    double Gmax() const { return F_Gmax; }
    void Gmax(double v) { F_Gmax = v; }
    double Gnoise() const { return F_Gnoise; }
    void Gnoise(double v) { F_Gnoise = v; }

    // Dual conductance mode
    int Add_Dont_Multiply() const { return F_Add_Dont_Multiply; }
    void Add_Dont_Multiply(int v) { F_Add_Dont_Multiply = v; }

    // Secondary conductance params (used when Add_Dont_Multiply == 1)
    double E2() const { return F_Add_Dont_Multiply ? F_E2 : F_E; }
    void E2(double v) { F_E2 = v; }
    double Gmax2() const { return F_Add_Dont_Multiply ? F_Gmax2 : F_Gmax; }
    void Gmax2(double v) { F_Gmax2 = v; }
    double Gnoise2() const { return F_Add_Dont_Multiply ? F_Gnoise2 : F_Gnoise; }
    void Gnoise2(double v) { F_Gnoise2 = v; }

    bool Initialize(bool Reset) override {
        if (Reset) return Restart(-60);
        return true;
    }

    bool Restart(double V) {
        m_pre.Restart(V); h_pre.Restart(V); n_pre.Restart(V);
        m_post.Restart(V); h_post.Restart(V); n_post.Restart(V);
        idum = -(std::rand() % 10000 + 1);
        return true;
    }

    const std::wstring & ClassKey() const override {
        static std::wstring Key(THH2Current_KEY);
        return Key;
    }

    const void GetParamLogHeader(std::vector<std::string> &params) const override {
        params.clear();
    }

    /// Pre gates use Vkin, post gates use Vdrv; multiply or add depending on mode
    double DoUpdate(double step, double Vkin, double Vdrv,
                    std::vector<double> &params) override
    {
        params.clear();
        double mp_pre = 1, hq_pre = 1, nr_pre = 1;
        if (F_p_pre != 0) mp_pre = pow(m_pre.Update(Vkin, step), F_p_pre);
        if (F_q_pre != 0) hq_pre = pow(h_pre.Update(Vkin, step), F_q_pre);
        if (F_r_pre != 0) nr_pre = pow(n_pre.Update(Vkin, step), F_r_pre);

        double mp_post = 1, hq_post = 1, nr_post = 1;
        if (F_p_post != 0) mp_post = pow(m_post.Update(Vdrv, step), F_p_post);
        if (F_q_post != 0) hq_post = pow(h_post.Update(Vdrv, step), F_q_post);
        if (F_r_post != 0) nr_post = pow(n_post.Update(Vdrv, step), F_r_post);

        double pre_gates = mp_pre * hq_pre * nr_pre;
        double post_gates = mp_post * hq_post * nr_post;

        double I = 0, G = 0;
        if (F_Add_Dont_Multiply) {
            G = (F_Gnoise == 0.0) ? F_Gmax * pre_gates
                : (F_Gmax + 0.01 * F_Gnoise * F_Gmax * gasdev(&idum)) * pre_gates;
            I = G * (Vdrv - F_E);
            double G2 = (F_Gnoise2 == 0.0) ? F_Gmax2 * post_gates
                : (F_Gmax2 + 0.01 * F_Gnoise2 * F_Gmax2 * gasdev(&idum)) * post_gates;
            I += G2 * (Vdrv - F_E2);
        } else {
            double open_prob = pre_gates * post_gates;
            G = (F_Gnoise == 0.0) ? F_Gmax * open_prob
                : (F_Gmax + 0.01 * F_Gnoise * F_Gmax * gasdev(&idum)) * open_prob;
            I = G * (Vdrv - F_E);
        }
        return I;
    }

    void CopyParamsFrom(const TCurrent * const) override {}

    THH2Current() : TCurrent(nullptr, L""),        F_p_pre(0), F_q_pre(0), F_r_pre(0),
        F_p_post(0), F_q_post(0), F_r_post(0),
        F_E(0), F_Gmax(5), F_Gnoise(0),
        F_Add_Dont_Multiply(0), F_E2(0), F_Gmax2(5), F_Gnoise2(0), idum(-1) {}

    THH2Current(TCurrentUser *owner, const std::wstring &name) : TCurrent(owner, name),
        F_p_pre(0), F_q_pre(0), F_r_pre(0),
        F_p_post(0), F_q_post(0), F_r_post(0),
        F_E(0), F_Gmax(5), F_Gnoise(0),
        F_Add_Dont_Multiply(0), F_E2(0), F_Gmax2(5), F_Gnoise2(0), idum(-1) {}
};

#endif
