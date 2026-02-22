/* /////////////////////////////////////////////////////////////////////////////
NetSuite: NetClamp/NetSim/NetFit Neural Network Modeling and Dynamic Clamp Software
	 Copyright (C) 2011-2014  E. Brady Trexler, Ph.D.

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

Please direct correspondence to ebtrexler _at_ hudsonvalleysci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GUI_RT_Edit_HH2Current.h"
#include "RT_Utilities.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MultiPLOTPanel"
#pragma resource "*.dfm"
THH2CurrentForm *HH2CurrentForm;


//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- THH2Current methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define THH2Current

#include "RT_HHKineticsFactor.h"

#define VERSION 1 //for freaky currents 5/29/14

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   THH2Current   ***************************
//
//
/// Implementation of Hodgkin-Huxley type current with Tohidi-Nadim shortcuts
/*!
<pre>
 Classes THH2Current gives ionic current as a function of several kinetic
 parameters and the input voltages V_kin and V_drv.  For intrinsic currents
 V_drv = V_kin, and for synaptic currents, V_kin is the voltage of the
 presynaptic cell.

   Computes:               p_pre      q_pre      r_pre
			 pre_gates = m_pre   *  h_pre   *  n_pre

									 p_post    q_post     r_post
			post_gates = m_post  *  h_post  *  n_post

			There are two methods to compute the current:

			Multiplication
			I = Gmax * pre_gates * post_gates * (V_drv - E)

			Addition
			I = Gmax * (pre_gates + post_gates) * (V_drv - E)
</pre>

@author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class THH2Current: public TCurrent
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TCurrent>(*this);
      ar & BOOST_SERIALIZATION_NVP(F_p_pre);
      ar & BOOST_SERIALIZATION_NVP(F_q_pre);
      ar & BOOST_SERIALIZATION_NVP(F_r_pre);
      ar & BOOST_SERIALIZATION_NVP(F_p_post);
      ar & BOOST_SERIALIZATION_NVP(F_q_post);
      ar & BOOST_SERIALIZATION_NVP(F_r_post);
      ar & BOOST_SERIALIZATION_NVP(F_E);
      ar & BOOST_SERIALIZATION_NVP(F_Gmax);
		ar & BOOST_SERIALIZATION_NVP(F_Gnoise);
      ar & BOOST_SERIALIZATION_NVP(m_pre);
      ar & BOOST_SERIALIZATION_NVP(h_pre);
      ar & BOOST_SERIALIZATION_NVP(n_pre);
      ar & BOOST_SERIALIZATION_NVP(m_post);
      ar & BOOST_SERIALIZATION_NVP(h_post);
		ar & BOOST_SERIALIZATION_NVP(n_post);

		// 2014/05/29 added for fitting wierd currents with sums of conductances
		if (version >= 1) {
			ar & BOOST_SERIALIZATION_NVP(F_Add_Dont_Multiply);
			ar & BOOST_SERIALIZATION_NVP(F_E2);
			ar & BOOST_SERIALIZATION_NVP(F_Gmax2);
			ar & BOOST_SERIALIZATION_NVP(F_Gnoise2);
		}

	}
#endif

private:
	double								F_p_pre;
	double 								F_q_pre;
   double                        F_r_pre;
   double                        F_p_post;
   double                        F_q_post;
	double                        F_r_post;
	double 								F_E;
	double 								F_Gmax;
	double                        F_Gnoise;

	// 2014/05/29 added for fitting wierd currents with sums of conductances
	int									F_Add_Dont_Multiply; // 1 = add, 0 = mult
	double 								F_E2;
	double 								F_Gmax2;
	double                        F_Gnoise2;

	UnicodeString                 V0txt, ktxt, tlotxt,
                                 thitxt,infMintxt, wtxt,
                                 ptext, qtext, rtext;

   long                          idum; // for random variables

protected:
	/// Writes data members to a stream
	virtual void const __fastcall WriteToStream(ostream &stream) const
	{
		stream.write((char *)&F_p_pre, sizeof(double));
		stream.write((char *)&F_q_pre, sizeof(double));
		stream.write((char *)&F_r_pre, sizeof(double));
		stream.write((char *)&F_p_post, sizeof(double));
		stream.write((char *)&F_q_post, sizeof(double));
		stream.write((char *)&F_r_post, sizeof(double));
		stream.write((char *)&F_E, sizeof(double));
		stream.write((char *)&F_Gmax, sizeof(double));
		stream.write((char *)&F_Gnoise, sizeof(double));
		stream.write((char *)&F_Add_Dont_Multiply, sizeof(int));
		stream.write((char *)&F_E2, sizeof(int));
		stream.write((char *)&F_Gmax2, sizeof(int));
		stream.write((char *)&F_Gnoise2, sizeof(double));
		stream << m_pre;
		stream << h_pre;
		stream << n_pre;
		stream << m_post;
		stream << h_post;
		stream << n_post;
		TCurrent::WriteToStream(stream); //Write base class members
	}

	/// Reads data members from a stream
	virtual void const __fastcall ReadFromStream(istream &stream)
	{
		stream.read((char *)&F_p_pre, sizeof(double));
		stream.read((char *)&F_q_pre, sizeof(double));
		stream.read((char *)&F_r_pre, sizeof(double));
		stream.read((char *)&F_p_post, sizeof(double));
		stream.read((char *)&F_q_post, sizeof(double));
		stream.read((char *)&F_r_post, sizeof(double));
		stream.read((char *)&F_E, sizeof(double));
		stream.read((char *)&F_Gmax, sizeof(double));
		stream.read((char *)&F_Gnoise, sizeof(double));
#ifdef VERSION
		if (VERSION >= 1) {
			stream.read((char *)&F_Add_Dont_Multiply, sizeof(int));
			stream.read((char *)&F_E2, sizeof(int));
			stream.read((char *)&F_Gmax2, sizeof(int));
			stream.read((char *)&F_Gnoise2, sizeof(double));
		}
#endif
		stream >> m_pre;
		stream >> h_pre;
		stream >> n_pre;
		stream >> m_post;
		stream >> h_post;
		stream >> n_post;
		TCurrent::ReadFromStream(stream); //Read base class members
	}

public:
	/// presynaptic activation exponent
	__property double 				p_pre = {read = F_p_pre, write = F_p_pre};
	/// presynaptic inactivation exponent
	__property double 				q_pre = {read = F_q_pre, write = F_q_pre};
	/// presynaptic third param exponent
	__property double             r_pre = {read = F_r_pre, write = F_r_pre};
	/// postsynaptic activation exponent
	__property double 				p_post = {read = F_p_post, write = F_p_post};
	/// postsynaptic inactivation exponent
	__property double 				q_post = {read = F_q_post, write = F_q_post};
	/// postsynaptic third param exponent
	__property double             r_post = {read = F_r_post, write = F_r_post};
	///reversal potential (mV)
	__property double 				E     = {read = GetE, write = SetE};
	/// mean maximum conductance (uS)
	__property double 				Gmax  = {read = GetGmax, write = SetGmax};
	/// Variation about mean conductance (%)
	__property double 				Gnoise= {read = GetGnoise, write = SetGnoise};
	/// Add conductances rather than multiply
	__property int						Add_Dont_Multiply = {read = F_Add_Dont_Multiply,
																	write = F_Add_Dont_Multiply};
	///reversal potential #2 (mV)
	__property double 				E2    = {read = GetE2, write = SetE2};
	/// mean maximum conductance #2 (uS)
	__property double 				Gmax2 = {read = GetGmax2, write = SetGmax2};
	/// Variation about mean conductance #2 (%)
	__property double 				Gnoise2= {read = GetGnoise2, write = SetGnoise2};

	/// presynaptic activation kinetic factor
	THHKineticsFactor             m_pre;
	/// presynaptic inactivation kinetic factor
	THHKineticsFactor             h_pre;
	/// presynaptic third kinetic factor
	THHKineticsFactor             n_pre;
	/// postsynaptic activation kinetic factor
	THHKineticsFactor             m_post;
	/// postsynaptic inactivation kinetic factor
	THHKineticsFactor             h_post;
	/// postsynaptic third kinetic factor
	THHKineticsFactor             n_post;

	void __fastcall 					SetGmax(double gmax)
	{
		F_Gmax = gmax;
		if (F_Add_Dont_Multiply) {
			F_Gmax2 = gmax;
		}
	}
	void __fastcall 					SetE(double erev)
	{
		F_E = erev;
		if (F_Add_Dont_Multiply) {
			F_E2 = erev;
		}
	}
	void __fastcall 					SetGnoise(double gnoise)
	{
		F_Gnoise = gnoise;
		if (F_Add_Dont_Multiply) {
			F_Gnoise2 = gnoise;
		}
	}
	double __fastcall 					GetGmax()
	{
		return F_Gmax;
	}
	double __fastcall 					GetE()
	{
		return F_E;
	}
	double __fastcall 					GetGnoise()
	{
		return F_Gnoise;
	}

	void __fastcall 					SetGmax2(double gmax)
	{
		F_Gmax2 = gmax;
	}
	void __fastcall 					SetE2(double erev)
	{
		F_E2 = erev;
	}
	void __fastcall 					SetGnoise2(double gnoise)
	{
		F_Gnoise2 = gnoise;
	}
	double __fastcall 					GetGmax2()
	{
		return F_Add_Dont_Multiply ? F_Gmax2 : F_Gmax;
	}
	double __fastcall 					GetE2()
	{
		return F_Add_Dont_Multiply ? F_E2 : F_E;
	}
	double __fastcall 					GetGnoise2()
	{
		return F_Add_Dont_Multiply ? F_Gnoise2 : F_Gnoise;
	}

	// implement pure virtual
	bool __fastcall               Initialize(bool Reset)
	{
		if (Reset) return Restart( -60 ) ; /* TODO : Need to find way to choose initial voltage for HHCurrent */
		return true;
	}

	/// Initializes kinetic parameters m_pre, h_pre, n_pre, m_post, h_post, and n_post
	bool __fastcall               Restart( double V )
	{
		m_pre.Restart(V);
		h_pre.Restart(V);
		n_pre.Restart(V);
		m_post.Restart(V);
		h_post.Restart(V);
		n_post.Restart(V);
		randomize();
		idum = -1 * rand();
		return true;
	}

	// implement pure virtual
	#define THH2Current_KEY L"HH 2 Currents Convolved"
	const std::wstring & __fastcall ClassKey() const
	{
		static std::wstring Key(THH2Current_KEY);
		return Key;
	}

	// implement pure virtual
	const void __fastcall			GetParamLogHeader(
												std::vector<std::string>& params) const
	{
		params.clear();
		if (p_pre != 0) {
			params.push_back("_mPre");
		}
		if (q_pre != 0) {
			params.push_back("_hPre");
		}
		if (r_pre != 0) {
			params.push_back("_nPre");
		}
		if (p_post != 0) {
			params.push_back("_mPost");
		}
		if (q_post != 0) {
			params.push_back("_hPost");
		}
		if (r_post != 0) {
			params.push_back("_nPost");
		}

		if (F_Add_Dont_Multiply) {
			params.push_back("_G1");
			params.push_back("_G2");
		}
		else {
			params.push_back("_G");
		}

	}

/// Calculation of HH type voltage dependent synaptic current - 3 gates pre and post
/*!
<pre>
*     Computes
*                               p_pre         q_pre         r_pre
*        pre_gates =  m_pre(Vkin) * h_pre(Vkin) * n_pre(Vkin)
*
*                                 p_post          q_post           r_post
*        post_gates = m_post(Vdrv)  *  h_post(Vdrv)  *  n_post(Vdrv)
*
*     and
*        I = Gmax * pre_gates * post_gates * (Vdrv - E)
</pre>
*/
	virtual double __fastcall DoUpdate(double step, double Vkin, double Vdrv,
													std::vector<double> & params)
	{
		// step is ms, V's are mV, G is in microsiemens, I is in nA

		double I = 0.0;
		double G = 0.0;
		double G2 = 0.0;
		double I2 = 0.0;

		params.clear();

		// Update presynaptic gates using Vkin
		double M_pre, mp_pre = 1;
		if (p_pre != 0) {
			M_pre = m_pre.Update(Vkin, step);
			mp_pre = pow(M_pre,p_pre);
			params.push_back(M_pre);
		}

		double H_pre, hq_pre = 1;
		if (q_pre != 0) {
			H_pre = h_pre.Update(Vkin, step);
			hq_pre = pow(H_pre,q_pre);
			params.push_back(H_pre);
		}

		double N_pre, nr_pre = 1;
		if (r_pre != 0) {
			N_pre = n_pre.Update(Vkin, step);
			nr_pre = pow(N_pre,r_pre);
			params.push_back(N_pre);
		}


		// update postsynaptic gates using Vdrv
		double M_post, mp_post = 1;
		if (p_post != 0) {
			M_post = m_post.Update(Vdrv, step);
			mp_post = pow(M_post,p_post);
			params.push_back(M_post);
		}

		double H_post, hq_post = 1;
		if (q_post != 0) {
			H_post = h_post.Update(Vdrv, step);
			hq_post = pow(H_post,q_post);
			params.push_back(H_post);
		}

		double N_post, nr_post = 1;
		if (r_post != 0) {
			N_post = n_post.Update(Vdrv, step);
			nr_post = pow(N_post,r_post);
			params.push_back(N_post);
		}

		double pre_gates = mp_pre*hq_pre*nr_pre;
		double post_gates = mp_post*hq_post*nr_post;

		// we're adding conductances here
		if (F_Add_Dont_Multiply) {
			// calc first gate's current
			if (F_Gnoise == 0.0){
				G = F_Gmax*pre_gates;
			}
			else {
				double noise = 0.01 * F_Gnoise * F_Gmax * gasdev(&idum);
				G = (F_Gmax + noise)*pre_gates;
			}

			I = G * (Vdrv - F_E);       // uS * mV = nA

			params.push_back(G);

			// calc second gate's current
			if (F_Gnoise2 == 0.0){
				G2 = F_Gmax2*post_gates;
			}
			else {
				double noise = 0.01 * F_Gnoise2 * F_Gmax2 * gasdev(&idum);
				G2= (F_Gmax2 + noise)*post_gates;
			}

			I2 = G2 * (Vdrv - F_E2);       // uS * mV = nA

			params.push_back(G2);

			I+=I2;  // add the currents together
		}
		else {
			double open_prob = pre_gates * post_gates;

			if (F_Gnoise == 0.0){
				G = F_Gmax*open_prob;
			}
			else {
				double noise = 0.01 * F_Gnoise * F_Gmax * gasdev(&idum);
				G = (F_Gmax + noise)*open_prob;
			}

			I = G * (Vdrv - F_E);       // uS * mV = nA

			params.push_back(G);
		}

		return I;
	}

	/// Called by GUI to synchronize edit form with current values of object params
	virtual void __fastcall       PopulateEditForm()
	{
		THH2CurrentForm *form = (THH2CurrentForm * )GetEditForm();

		V0txt =     L"V_1/2 (mV)";
		ktxt  =     L"k";
		tlotxt=     L"Tau low (ms)";
		thitxt=     L"Tau hi (ms)";
		infMintxt = L"SSmin";
		wtxt  =     L"den. exponent";
		ptext =     L"p (exponent)";
		qtext =     L"q (exponent)";
		rtext =     L"r (exponent)";

		double p, q, r;
		THHKineticsFactor *m, *h, *n;

		const int presynaptic = 0;
		const int postsynaptic = 1;

		try {
			if (dynamic_cast<TSynapse * const> (Owner())){
				form->TabControl1->Tabs->Strings[0] = (L"PreSynaptic");
				form->TabControl1->Tabs->Strings[1] = (L"PostSynaptic");
			}
			else {   // intracellular or intrinsic
				form->TabControl1->Tabs->Strings[0] = (L"Current #1");
				form->TabControl1->Tabs->Strings[1] = (L"Current #2");
			}
		} catch (EInvalidCast &E) {

		}

		form->GmaxEdit->Text = FloatToStr(F_Gmax);
		form->GnoiseEdit->Text = FloatToStr(F_Gnoise);
		form->ErevEdit->Text = FloatToStr(F_E);

		form->GCalcRadioGroup->ItemIndex = Add_Dont_Multiply;

		if (form->TabControl1->TabIndex == presynaptic) {
			p = p_pre;
			q = q_pre;
			r = r_pre;
			m = &m_pre;
			h = &h_pre;
			n = &n_pre;
		}

		else if (form->TabControl1->TabIndex == postsynaptic) {
			p = p_post;
			q = q_post;
			r = r_post;
			m = &m_post;
			h = &h_post;
			n = &n_post;

			if (F_Add_Dont_Multiply) {
         	form->GmaxEdit->Text = FloatToStr(F_Gmax2);
				form->GnoiseEdit->Text = FloatToStr(F_Gnoise2);
				form->ErevEdit->Text = FloatToStr(F_E2);
			}
		}

		form->ValueListEditor_m->Values[V0txt] = m->V0;
		form->ValueListEditor_m->Values[ktxt] = m->k;
		form->ValueListEditor_m->Values[tlotxt] = m->t_lo;
		form->ValueListEditor_m->Values[thitxt] = m->t_hi;
		form->ValueListEditor_m->Values[infMintxt] = m->infMin;
		form->ValueListEditor_m->Values[wtxt] = m->w;
		form->ValueListEditor_m->Values[ptext] = p;

		form->ValueListEditor_h->Values[V0txt] = h->V0;
		form->ValueListEditor_h->Values[ktxt] = h->k;
		form->ValueListEditor_h->Values[tlotxt] = h->t_lo;
		form->ValueListEditor_h->Values[thitxt] = h->t_hi;
		form->ValueListEditor_h->Values[infMintxt] = h->infMin;
		form->ValueListEditor_h->Values[wtxt] = h->w;
		form->ValueListEditor_h->Values[qtext] = q;

		form->ValueListEditor_n->Values[V0txt] = n->V0;
		form->ValueListEditor_n->Values[ktxt] = n->k;
		form->ValueListEditor_n->Values[tlotxt] = n->t_lo;
		form->ValueListEditor_n->Values[thitxt] = n->t_hi;
		form->ValueListEditor_n->Values[infMintxt] = n->infMin;
		form->ValueListEditor_n->Values[wtxt] = n->w;
		form->ValueListEditor_n->Values[rtext] = r;

		form->UpdatePlots();
	}

	/// called by ValidateEditForm
	bool KineticFactorsValidate(THHKineticsFactor &f, wchar_t *factorname,
											TValueListEditor *editor, double &the_exp,
											wchar_t *exptext)
	{
		double temp;
		bool ok = true;
		UnicodeString message, exponent;

//      if (V0txt == NULL) { // if == NULL, then PopulateEditForm hasn't been called yet
//         return false;
//      }

		try {
			if (editor->Values[V0txt] != L"") {
				temp = StrToFloat(editor->Values[V0txt]);
				f.V0 = temp;
			}

			if (editor->Values[ktxt] != L""){
				temp = StrToFloat(editor->Values[ktxt]);
				f.k = temp;
			}

			if (editor->Values[tlotxt] != L"") {
				temp = StrToFloat(editor->Values[tlotxt]);
				if (temp < 0.0){
					message = factorname;
					message += L"'s Tau_low must be greater than or equal to zero";
					Application->MessageBox(message.c_str(), L"Please correct input");
					ok = false;
				} else {
					f.t_lo = temp;
				}
			}

			if (editor->Values[thitxt] != L"") {
				temp = StrToFloat(editor->Values[thitxt]);
				if (temp < 0.0){
					message = factorname;
					message += L"'s Tau_hi must be greater than or equal to zero";
					Application->MessageBox(message.c_str(), L"Please correct input");
					ok = false;
				} else {
					f.t_hi = temp;
				}
			}


			if (editor->Values[infMintxt] != L"") {
				temp = StrToFloat(editor->Values[infMintxt]);
				if (temp < 0.0 || temp > 1.0){
					message = factorname;
					message += L"'s infMin must be greater than or equal to zero and less"
					L" than or equal to one.";
					Application->MessageBox(message.c_str(), L"Please correct input");
					ok = false;
				} else {
					f.infMin = temp;
				}
			}

			if (editor->Values[wtxt] != L"") {
				temp = StrToFloat(editor->Values[wtxt]);
				if (temp < 0.0){
					message = factorname;
					message += L"'s w must be greater than or equal to zero.";
					Application->MessageBox(message.c_str(), L"Please correct input");
					ok = false;
				} else {
					f.w = temp;
				}
			}


			exponent = exptext;
			exponent += L" (exponent)";
			if (editor->Values[exponent] != L"") {
				temp = StrToFloat(editor->Values[exponent]);
				if (temp < 0.0){
					message = exponent;
					message += L" must be greater than or equal to zero";
					Application->MessageBox(message.c_str(), L"Please correct input");
					ok = false;
				} else {
					the_exp = temp;
				}
			}

		} catch (EConvertError & e) {
			ok = false;
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
		}

		return ok;
	}

	/// Called by GUI to check if changed values are satisfactory
	virtual bool __fastcall       ValidateEditForm()
	{

		THH2CurrentForm *form = (THH2CurrentForm * )GetEditForm();
		bool ok = true;

		double *p, *q, *r;
		THHKineticsFactor *m, *h, *n;

		const int presynaptic = 0;
		const int postsynaptic = 1;

		if (form->TabControl1->TabIndex == presynaptic) {
			p = &p_pre;
			q = &q_pre;
			r = &r_pre;
			m = &m_pre;
			h = &h_pre;
			n = &n_pre;
		}

		else if (form->TabControl1->TabIndex == postsynaptic) {
			p = &p_post;
			q = &q_post;
			r = &r_post;
			m = &m_post;
			h = &h_post;
			n = &n_post;
		}

		Add_Dont_Multiply = form->GCalcRadioGroup->ItemIndex;

		ok = KineticFactorsValidate(*m, L"m", form->ValueListEditor_m, *p, L"p");
		if (ok == false) {
			return ok;
		}
		ok = KineticFactorsValidate(*h, L"h", form->ValueListEditor_h, *q, L"q");
		if (ok == false) {
			return ok;
		}
		ok = KineticFactorsValidate(*n, L"n", form->ValueListEditor_n, *r, L"r");
		if (ok == false) {
			return ok;
		}

		double temp;
		try {
			temp = StrToFloat(form->GmaxEdit->Text);
//         if (temp < 0.0){
//            Application->MessageBox(L"Gmax conductance must be greater than or equal to zero",
//                                    L"Please correct input");
//            ok = false;
//         } else {
//            F_Gmax = temp;
//         }

			// for subtracting conductance, Gmax has to be negative
			if (form->TabControl1->TabIndex == presynaptic) {
				F_Gmax = temp;
			}
			else {
				F_Gmax2 = temp;
			}

			temp = StrToFloat(form->ErevEdit->Text);
			if (form->TabControl1->TabIndex == presynaptic) {
				F_E = temp;
			}
			else {
				F_E2 = temp;
			}

			temp = StrToFloat(form->GnoiseEdit->Text);
			if (form->TabControl1->TabIndex == presynaptic) {
				F_Gnoise = temp;
			}
			else {
				F_Gnoise2 = temp;
			}

		} catch (EConvertError & e) {
			ok = false;
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
		}

		return ok;
	}

	/// Returns downcasted THH2CurrentForm* that is used to set values for this object
	virtual void* const __fastcall
											GetEditForm()
	{
		static THH2CurrentForm *form = new THH2CurrentForm(NULL);
		form->HH2Current = this;
		return form;
	}

	/// default constructor
	__fastcall THH2Current():
			TCurrent(NULL, L""), F_p_pre(0), F_q_pre(0), F_r_pre(0),
										F_p_post(0), F_q_post(0), F_r_post(0),
										F_E(0), F_Gmax(5), F_Gnoise(0),
										F_E2(0), F_Gmax2(5), F_Gnoise2(0),
										F_Add_Dont_Multiply(0)
	{
	}

	/// specialized constructor 2 param
	__fastcall THH2Current(TCurrentUser *owner, const std::wstring & name):
			TCurrent(owner, name), F_p_pre(0), F_q_pre(0), F_r_pre(0),
										F_p_post(0), F_q_post(0), F_r_post(0),
										F_E(0), F_Gmax(5), F_Gnoise(0),
										F_E2(0), F_Gmax2(5), F_Gnoise2(0),
										F_Add_Dont_Multiply(0)
	{
	}

	/// specialized constructor 1 param
	__fastcall THH2Current(const std::wstring & name):
			TCurrent(NULL, name), F_p_pre(0), F_q_pre(0), F_r_pre(0),
										F_p_post(0), F_q_post(0), F_r_post(0),
										F_E(0), F_Gmax(5), F_Gnoise(0),
										F_E2(0), F_Gmax2(5), F_Gnoise2(0),
										F_Add_Dont_Multiply(0)
   {
   }

   /// copy constructor
   __fastcall THH2Current( const THH2Current & source ) :
         TCurrent(source.Owner(), source.Name()),
         F_p_pre(source.F_p_pre),
         F_q_pre(source.F_q_pre),
         F_r_pre(source.F_r_pre),
         F_p_post(source.F_p_post),
         F_q_post(source.F_q_post),
         F_r_post(source.F_r_post),
			F_E(source.F_E),
			F_Gmax(source.F_Gmax),
			F_Gnoise(source.F_Gnoise),
			F_E2(source.F_E2),
			F_Gmax2(source.F_Gmax2),
			F_Gnoise2(source.F_Gnoise2),
			F_Add_Dont_Multiply(source.F_Add_Dont_Multiply),
			m_pre(source.m_pre),
			h_pre(source.h_pre),
			n_pre(source.n_pre),
			m_post(source.m_post),
			h_post(source.h_post),
			n_post(source.n_post)
	{
   }

   /// overloaded assignment operator
   THH2Current & operator = (const THH2Current & source)
   {
      if (this != &source) {  // make sure not same object
         this->TCurrent::operator=(source); //copy base class members
         // now copy THH2Current fields
         F_p_pre = (source.F_p_pre);
         F_q_pre = (source.F_q_pre);
         F_r_pre = (source.F_r_pre);
         F_p_post = (source.F_p_post);
         F_q_post = (source.F_q_post);
         F_r_post = (source.F_r_post);
         F_E = (source.F_E);
         F_Gmax = (source.F_Gmax);
			F_Gnoise = (source.F_Gnoise);
			F_E2 = (source.F_E2);
			F_Gmax2 = (source.F_Gmax2);
			F_Gnoise2 = (source.F_Gnoise2);
			F_Add_Dont_Multiply = (source.F_Add_Dont_Multiply);
         m_pre = (source.m_pre);
         h_pre = (source.h_pre);
         n_pre = (source.n_pre);
         m_post = (source.m_post);
         h_post = (source.h_post);
         n_post = (source.n_post);
      }
      return *this;
   }

   /// overloaded method for duplicating currents without complete assignment
   virtual void __fastcall CopyParamsFrom(const TCurrent * const source )
   {
      if (this != source) {  // make sure not same object
//         std::wstring this_type(this->ClassType());
//         std::wstring source_type(source->ClassType());
//         if (this->ClassType() == source->ClassType()) { // make sure same type
//         if (this_type == source_type) {
            THH2Current *cast_source = (THH2Current *)source;

            F_p_pre = (cast_source->F_p_pre);
            F_q_pre = (cast_source->F_q_pre);
            F_r_pre = (cast_source->F_r_pre);
            F_p_post = (cast_source->F_p_post);
            F_q_post = (cast_source->F_q_post);
            F_r_post = (cast_source->F_r_post);
            F_E = (cast_source->F_E);
            F_Gmax = (cast_source->F_Gmax);
				F_Gnoise = (cast_source->F_Gnoise);
				F_E2 = (cast_source->F_E2);
				F_Gmax2 = (cast_source->F_Gmax2);
				F_Gnoise2 = (cast_source->F_Gnoise2);
				F_Add_Dont_Multiply = (cast_source->F_Add_Dont_Multiply);
            m_pre = (cast_source->m_pre);
            h_pre = (cast_source->h_pre);
            n_pre = (cast_source->n_pre);
            m_post = (cast_source->m_post);
            h_post = (cast_source->h_post);
            n_post = (cast_source->n_post);
//         }
      }
   }

	virtual ~THH2Current(){;}
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(THH2Current)

BOOST_CLASS_VERSION(THH2Current, 1) //added 05/29/2014 for fitting wierd currents


#endif //SERIALIZE


////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
   static struct regHH2Current {
      regHH2Current(){
			GetCurrentFactory().registerBuilder(
                                 THH2Current_KEY, //L"HH Current",
                                 TypeID<THH2Current>(),
                                 TypeID<TCurrentUser*const>(),
                                 TypeID<const std::wstring>());
      }
   } RegHH2Current;
}

#pragma end_region


//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- THH2CurrentForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region GUI methods of THHCurrentForm


//---------------------------------------------------------------------------
__fastcall THH2CurrentForm::THH2CurrentForm(TComponent* Owner)
   : TForm(Owner)
{
   double start_mV = -150.0;
   double mVperPt = 1.0;

	// IV curve
	PlotIV = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[0], 0, "",
											clBlack, start_mV, mVperPt,
											NumPoints,
											&dataIV[0]);
	MultiPLOTPanel1->PLOTPanel[0]->AddPlot(PlotIV, false);

	// IV curve combined all 6 gates
	PlotIVCombined = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[0], 0, "",
											clRed, start_mV, mVperPt,
											NumPoints,
											&dataIVCombined[0]);
	MultiPLOTPanel1->PLOTPanel[0]->AddPlot(PlotIVCombined, false);



	// inf values
	Plotminf = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[1], 0, "",
											clRed, start_mV, mVperPt,
											NumPoints,
											&dataminf[0]);
	MultiPLOTPanel1->PLOTPanel[1]->AddPlot(Plotminf, false);

	Plothinf = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[1], 0, "",
											clBlue, start_mV, mVperPt,
											NumPoints,
											&datahinf[0]);
	MultiPLOTPanel1->PLOTPanel[1]->AddPlot(Plothinf, false);

	Plotninf = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[1], 0, "",
											clGreen, start_mV, mVperPt,
											NumPoints,
											&dataninf[0]);
	MultiPLOTPanel1->PLOTPanel[1]->AddPlot(Plotninf, false);



	// Tau values
	Plottaum = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[2], 0, "",
											clRed, start_mV, mVperPt,
											NumPoints,
											&datataum[0]);
	MultiPLOTPanel1->PLOTPanel[2]->AddPlot(Plottaum, false);

	Plottauh = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[2], 0, "",
											clBlue, start_mV, mVperPt,
											NumPoints,
											&datatauh[0]);
	MultiPLOTPanel1->PLOTPanel[2]->AddPlot(Plottauh, false);

	Plottaun = new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[2], 0, "",
											clGreen, start_mV, mVperPt,
											NumPoints,
											&datataun[0]);
	MultiPLOTPanel1->PLOTPanel[2]->AddPlot(Plottaun, false);

}
//---------------------------------------------------------------------------

/// Called when parameter changes need to be reflected in GUI
void __fastcall THH2CurrentForm::UpdatePlots()
{
	double volt = -150.0;
   double start_mV = -150.0;
	double mVperPt = 1.0;

	double p, q, r;
	THHKineticsFactor *m, *h, *n;
	double p2, q2, r2;
	THHKineticsFactor *m2, *h2, *n2;
	double Gmax, Gmax2, Erev, Erev2;

	const int presynaptic = 0;
   const int postsynaptic = 1;

   if (TabControl1->TabIndex == presynaptic) {
		p = HH2Current->p_pre;
		q = HH2Current->q_pre;
		r = HH2Current->r_pre;
		m = &HH2Current->m_pre;
		h = &HH2Current->h_pre;
		n = &HH2Current->n_pre;
		Gmax = HH2Current->Gmax;
		Erev = HH2Current->E;

		p2 = HH2Current->p_post;
		q2 = HH2Current->q_post;
		r2 = HH2Current->r_post;
		m2 = &HH2Current->m_post;
		h2 = &HH2Current->h_post;
		n2 = &HH2Current->n_post;
		Gmax2 = HH2Current->Gmax2;
		Erev2 = HH2Current->E2;
   }

   else if (TabControl1->TabIndex == postsynaptic) {
		p = HH2Current->p_post;
		q = HH2Current->q_post;
		r = HH2Current->r_post;
		m = &HH2Current->m_post;
		h = &HH2Current->h_post;
		n = &HH2Current->n_post;
		Gmax = HH2Current->Gmax2;
		Erev = HH2Current->E2;

		p2 = HH2Current->p_pre;
		q2 = HH2Current->q_pre;
		r2 = HH2Current->r_pre;
		m2 = &HH2Current->m_pre;
		h2 = &HH2Current->h_pre;
		n2 = &HH2Current->n_pre;
		Gmax2 = HH2Current->Gmax;
		Erev2 = HH2Current->E;
	}


	for (int v = 0; v < NumPoints; v++)
	{
      // make taus
		datataum[v] = m->tau(volt);
      datatauh[v] = h->tau(volt);
      datataun[v] = n->tau(volt);

      // Make infs
      dataminf[v] = m->inf(volt);
		datahinf[v] = h->inf(volt);
      dataninf[v] = n->inf(volt);

		// Make IV
		double mp = p == 0 ? 1 : pow(dataminf[v], p);
		double hq = q == 0 ? 1 : pow(datahinf[v], q);
		double nr = r == 0 ? 1 : pow(dataninf[v], r);
		dataIV[v] = Gmax*mp*hq*nr*(volt - Erev);

		double mp2 = p2 == 0 ? 1 : pow(m2->inf(volt), p2);
		double hq2 = q2 == 0 ? 1 : pow(h2->inf(volt), q2);
		double nr2 = r2 == 0 ? 1 : pow(n2->inf(volt), r2);

		if (HH2Current->Add_Dont_Multiply) { // add uses only two Gmaxs and Erevs
			dataIVCombined[v] = dataIV[v] + Gmax2*mp2*hq2*nr2*(volt - Erev2);
		}
		else {                   	// multiply uses only one Gmax and Erev
			dataIVCombined[v] = Gmax*mp*hq*nr*mp2*hq2*nr2*(volt - Erev);
      }

      volt += 1.0;
	}

	MultiPLOTPanel1->PLOTPanel[0]->ScalePlots(true);
	MultiPLOTPanel1->PLOTPanel[1]->ScalePlots(true);
	MultiPLOTPanel1->PLOTPanel[2]->ScalePlots(true);
}
//---------------------------------------------------------------------------

/// Calls UpdatePlots to reflect changes if valid, otherwise no changes accepted
void __fastcall THH2CurrentForm::ParamsEditors_KeyPress(TObject *Sender, wchar_t &Key)
{
   if (Key == VK_RETURN) {
      HH2Current->ValidateEditForm();
      UpdatePlots();
      Key = 0;
   }
}
//---------------------------------------------------------------------------

/// Opens saved parameters from dialog-chosen file
void __fastcall THH2CurrentForm::LoadButtonClick(TObject *Sender)
{
	OpenDialog1->InitialDir = ExtractFilePath(Application->ExeName)  + L"currents";
	if (OpenDialog1->Execute()){
      std::ifstream ifs(OpenDialog1->FileName.c_str());
      if (ifs.good()){
			ifs >> *HH2Current;
			HH2Current->PopulateEditForm();
      }
      else Application->MessageBox(L"Unable to open file for reading", L"I-O Error");
   }
}
//---------------------------------------------------------------------------

/// Saves parameters to dialog-chosen file
void __fastcall THH2CurrentForm::SaveButtonClick(TObject *Sender)
{
	SaveDialog1->InitialDir = ExtractFilePath(Application->ExeName) + L"currents";
	if (SaveDialog1->Execute()){
		// make an archive
      std::ofstream ofs(SaveDialog1->FileName.c_str());
      if (ofs.good()){
         ofs << *HH2Current;
      }
      else Application->MessageBox(L"Unable to open file for writing", L"I-O Error");
   }
}
//---------------------------------------------------------------------------




void __fastcall THH2CurrentForm::TabControl1Changing(TObject *Sender, bool &AllowChange)

{
   AllowChange = HH2Current->ValidateEditForm();
}
//---------------------------------------------------------------------------

void __fastcall THH2CurrentForm::TabControl1Change(TObject *Sender)
{
   HH2Current->PopulateEditForm();
   UpdatePlots();
}
//---------------------------------------------------------------------------


void __fastcall THH2CurrentForm::GCalcRadioGroupClick(TObject *Sender)
{
	HH2Current->Add_Dont_Multiply = GCalcRadioGroup->ItemIndex;
	HH2Current->PopulateEditForm();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#pragma end_region
