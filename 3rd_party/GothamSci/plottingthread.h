//---------------------------------------------------------------------------

#ifndef plottingthreadH
#define plottingthreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>

class TMultiPLOTPanel; //forward declaration

//---------------------------------------------------------------------------
class TPlottingThread : public TThread
{
private:
protected:
   TMultiPLOTPanel    *MultiPLOTPanelParent;

	void __fastcall   Execute();
public:
	__fastcall TPlottingThread(TMultiPLOTPanel *parent, bool CreateSuspended);

   bool              OK;
   int               CurrentBufferIndex;
   
   void __fastcall   Plot();
};
//---------------------------------------------------------------------------
#endif
