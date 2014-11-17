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

Please direct correspondence to ebtrexler _at_ gothamsci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------

#pragma hdrstop

#include "RT_HHLinearPiecewiseKineticsFactor.h"
//---------------------------------------------------------------------------

#include <ValEdit.hpp>

/// Fill in param names for GUI
void __fastcall THHLinearPiecewiseKineticsFactor::PopulateParams(void * guiElement)
{
	Vlotxt =    L"V_lo (mV)";
	Vhitxt = 	L"V_hi (mV)";
	slopetxt = 	L"slope";
	intcpttxt = L"intercept";
	tlotxt=     L"Tau lo (ms)";
	thitxt=     L"Tau hi (ms)";
//	inflotxt = 	L"Inf lo (0..1)";
//	infhitxt = 	L"Inf hi (0..1)";

	TValueListEditor *vle = (TValueListEditor *)guiElement;

	vle->Values[Vlotxt] = FloatToStrF(V_lo, ffFixed, 3, 3);
	vle->Values[Vhitxt] = FloatToStrF(V_hi, ffFixed, 3, 3);
	vle->Values[slopetxt] = FloatToStrF(slope, ffFixed, 3, 3);
	vle->Values[intcpttxt] = FloatToStrF(intcpt, ffFixed, 3, 3);
//	vle->Values[inflotxt] = FloatToStrF(inf_lo, ffFixed, 3, 3);
//	vle->Values[infhitxt] = FloatToStrF(inf_hi, ffFixed, 3, 3);
	vle->Values[tlotxt] = FloatToStrF(t_lo, ffFixed, 3, 3);
	vle->Values[thitxt] = FloatToStrF(t_hi, ffFixed, 3, 3);
}

/// called by ValidateEditForm
bool __fastcall THHLinearPiecewiseKineticsFactor::KineticFactorsValidate(
										THHKineticsFactor &f, wchar_t *factorname,
										void *ed, double &the_exp,
										wchar_t *exptext)
{
	double temp;
	bool ok = true;
	UnicodeString message, exponent;

	TValueListEditor *editor = (TValueListEditor * )ed;

	THHLinearPiecewiseKineticsFactor & fcast = (THHLinearPiecewiseKineticsFactor &)f;

	try {
		if (editor->Values[Vlotxt] != L"") {
			temp = StrToFloat(editor->Values[Vlotxt]);
			fcast.V_lo = temp;
		}

		if (editor->Values[Vhitxt] != L""){
			temp = StrToFloat(editor->Values[Vhitxt]);
			fcast.V_hi = temp;
		}

		if (editor->Values[slopetxt] != L"") {
			temp = StrToFloat(editor->Values[slopetxt]);
			fcast.slope = temp;
		}

		if (editor->Values[intcpttxt] != L""){
			temp = StrToFloat(editor->Values[intcpttxt]);
			fcast.intcpt = temp;
		}

		if (editor->Values[tlotxt] != L"") {
			temp = StrToFloat(editor->Values[tlotxt]);
			if (temp < 0.0){
				message = factorname;
				message += L"'s Tau_low must be greater than or equal to zero";
				Application->MessageBox(message.c_str(), L"Please correct input");
				ok = false;
			} else {
				fcast.t_lo = temp;
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
				fcast.t_hi = temp;
			}
		}


//		if (editor->Values[inflotxt] != L"") {
//			temp = StrToFloat(editor->Values[inflotxt]);
//			if (temp < 0.0 || temp > 1.0){
//				message = factorname;
//				message += L"'s Inf_Lo must be greater than or equal to zero and less"
//				L" than or equal to one.";
//				Application->MessageBox(message.c_str(), L"Please correct input");
//				ok = false;
//			} else {
//				fcast.inf_lo = temp;
//			}
//		}
//
//		if (editor->Values[infhitxt] != L"") {
//			temp = StrToFloat(editor->Values[infhitxt]);
//			if (temp < 0.0){
//				message = factorname;
//				message += L"'s Inf_hi must be greater than or equal to zero.";
//				Application->MessageBox(message.c_str(), L"Please correct input");
//				ok = false;
//			} else {
//				fcast.inf_hi = temp;
//			}
//		}

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


#pragma package(smart_init)
