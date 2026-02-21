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

#include "RT_HHKineticsFactor.h"

#ifndef NO_VCL
#include <ValEdit.hpp>

/// Fill in param names for GUI
void THHKineticsFactor::PopulateParams(void * guiElement)
{
	V0txt =     L"V_1/2 (mV)";
	ktxt  =     L"k";
	tlotxt=     L"Tau low (ms)";
	thitxt=     L"Tau hi (ms)";
	infMintxt = L"SSmin";
	wtxt  =     L"den. exponent";

	TValueListEditor *vle = (TValueListEditor *)guiElement;

	vle->Values[V0txt] = FloatToStrF(V0, ffFixed, 3, 3);
	vle->Values[ktxt] = FloatToStrF(k, ffFixed, 3, 3);
	vle->Values[tlotxt] = FloatToStrF(t_lo, ffFixed, 3, 3);
	vle->Values[thitxt] = FloatToStrF(t_hi, ffFixed, 3, 3);
	vle->Values[infMintxt] = FloatToStrF(infMin, ffFixed, 3, 3);
	vle->Values[wtxt] = FloatToStrF(w, ffFixed, 3, 3);
}

/// called by ValidateEditForm
bool THHKineticsFactor::KineticFactorsValidate(
										THHKineticsFactor &f, wchar_t *factorname,
										void *ed, double &the_exp,
										wchar_t *exptext)
{
	double temp;
	bool ok = true;
	UnicodeString message, exponent;

	TValueListEditor *editor = (TValueListEditor * )ed;

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
#endif


//---------------------------------------------------------------------------

#pragma package(smart_init)
