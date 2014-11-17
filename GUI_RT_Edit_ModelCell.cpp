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

#include <vcl.h>
#pragma hdrstop

#include "GUI_RT_Edit_ModelCell.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TModelCellForm *ModelCellForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TModelCell methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region RT classes with methods that define TModelCell

#include "RT_Network.h"


////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TModelCell   ***************************
//
//
/// Implements a cell whose voltage is updated by numerical integration
/*!
CalcVm() method called to update membrane voltage.  Then all Update() is called so that
intrinsic currents, synaptic currents, and electrode currents are calculated and
summed.

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TModelCell: public TCell
{

#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TCell>(*this);
      ar & BOOST_SERIALIZATION_NVP(FCapacitance);
      ar & BOOST_SERIALIZATION_NVP(FInitialVm);
	}
#endif

private:
   double                     FCapacitance;
   double                     FInitialVm;

protected:

	void const __fastcall WriteToStream(ostream &stream) const
	{
		stream.write((char *)&FCapacitance, sizeof(double));
		stream.write((char *)&FInitialVm, sizeof(double));
//		TCell::WriteToStream(stream);  //write base class members
	}

	void const __fastcall ReadFromStream(istream &stream)
	{
		stream.read((char *)&FCapacitance, sizeof(double));
		stream.read((char *)&FInitialVm, sizeof(double));
//		TCell::ReadFromStream(stream); //read base class members
	}

public:
	void __fastcall            PopulateEditForm()
	{
		TModelCellForm *form = (TModelCellForm *)GetEditForm();
		form->CapacitanceEdit->Text = FloatToStr(FCapacitance);
		form->InitialVmEdit->Text = FloatToStr(FInitialVm);
	}

	bool __fastcall            ValidateEditForm()
	{
		TModelCellForm *form = (TModelCellForm *)GetEditForm();
		bool ok = true;
		try {
			// get capacitance
			double temp = StrToFloat(form->CapacitanceEdit->Text);
			if (temp < 0.0) {
				Application->MessageBox(L"Capacitance should be positive",
												L"Please correct input");
				ok = false;
			}
			FCapacitance = temp;

			// get Initial Vm
			temp = StrToFloat(form->InitialVmEdit->Text);
			FInitialVm = temp;

		} catch (EConvertError & e) {
		  Application->MessageBox(e.ToString().c_str(), L"Please correct input");
		  ok = false;
		}
		return ok;
	}

   void* const __fastcall    GetEditForm()
   {
      static TModelCellForm *form = new TModelCellForm(NULL);
      form->ModelCell = this;
      return form;
   }

	// implement pure virtual
	#define TModelCell_KEY L"Model Cell"
	const std::wstring & __fastcall ClassKey() const
	{
		static std::wstring Key(TModelCell_KEY);
		return Key;
	}

	/// Default constructor
	__fastcall TModelCell() : TCell(),  FCapacitance(10), FInitialVm(-60)
	{
	}

   /// Specialized constructor
	__fastcall TModelCell(const std::wstring & name) : TCell(name),
                                       FCapacitance(10), FInitialVm(-60)
	{
	}

   /// Copy constructor
   __fastcall TModelCell(const TModelCell &source) :
      TCell(source.Name()),
      FCapacitance(source.FCapacitance),
      FInitialVm(source.FInitialVm)
   {
   }

   /// Overloaded assignment operator
   TModelCell& operator = (const TModelCell & source)
   {
      if (this != &source) {  // make sure not same object
         this->TCell::operator=(source); //copy base class members
         // now copy TModelCell fields
         FCapacitance = source.FCapacitance;
         FInitialVm = source.FInitialVm;
      }
      return *this;
   }

	~TModelCell()
	{
	}

//	// friend insertion << operator
//	friend ostream &operator<<(ostream &stream, const TModelCell &modelcell)
//	{
//		stream << *((TCell*)&modelcell); //write base class members
//		// now write TModelCell fields
//		stream.write((char *)&modelcell.FCapacitance, sizeof(double));
//		stream.write((char *)&modelcell.FInitialVm, sizeof(double));
//		return stream;
//	}
//
//	// friend extraction >> operator
//	friend istream &operator>>(istream &stream, TModelCell &modelcell)
//	{
//		stream >> *((TCell*)&modelcell); //read base class members
//		// now read TModelCell fields
//		stream.read((char *)&modelcell.FCapacitance, sizeof(double));
//		stream.read((char *)&modelcell.FInitialVm, sizeof(double));
//		return stream;
//   }

   // pure virtual functions overidden

   virtual double __fastcall     SetVm(double Vm)
   {
      throw Exception(L"Can't set Vm of Model cell");
      return 0;
   }

   virtual inline double f(double y)
	{
		return F_I / FCapacitance;  // dV/dt = I/C, mV/ms = nA/nF
	}
	virtual inline double rk4(double y_n, double step) // returns y_n+1 by 4th order Runge-Kutta
	{
		// y' = f(t,y) --> ODE to solve numerically

		double h=step/2.0,                 /* the midpoint */
		t1, t2, t3,            /* temporary storage arrays */
		k1, k2, k3,k4;                  /* for Runge-Kutta */

		t1=y_n+0.5*(k1=step*f(y_n));
		t2=y_n+0.5*(k2=step*f(t1));
		t3=y_n+    (k3=step*f(t2));
		k4=            step*f(t3);

		return y_n+=(k1+2*k2+2*k3+k4)/6.0;
	}

   virtual double __fastcall     CalcVm(double step)
   {

      if (F_Vm > 200.0) {
         F_Vm = 200.0;
      }
      else if (F_Vm < -200.0) {
         F_Vm = -200.0;
      }
      else {
         F_Vm = rk4(F_Vm, step);
      }
      return F_Vm;
   }

   bool __fastcall               IsVoltageDependent()
   {
      return false;
   }

   // implement pure virtual
   bool __fastcall               Initialize(bool Reset)
   {
      if (Reset) {
         F_Vm = FInitialVm;
         F_I = 0;
      }
      return true;
	}

	// implement pure virtual
	// Informs caller whether can accept currents or not
	virtual const bool __fastcall AcceptsCurrents() const
	{
		return true;
	}
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TModelCell)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace ModelCellspace
{
   static struct regModelCell {
	  regModelCell(){
		 GetCellFactory().registerBuilder(
								 TModelCell_KEY, //L"Model Cell",
								 TypeID<TModelCell>(),
								 TypeID<const std::wstring>());
      }
   } RegModelCell;

}

//---------------------------------------------------------------------------

#pragma end_region

//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TModelCellForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region GUI methods of TModelCellForm


//---------------------------------------------------------------------------
__fastcall TModelCellForm::TModelCellForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TModelCellForm::EditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
      ModelCell->ValidateEditForm();
      Key = 0;
   }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma end_region

