//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "plottingthread.h"
#include "MultiPLOTPanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TPlottingThread::TPlottingThread(TMultiPLOTPanel *parent,
                                             bool CreateSuspended)
                                       	: TThread(CreateSuspended)
{
	Priority = tpIdle;
   MultiPLOTPanelParent = parent;
}                                                
//---------------------------------------------------------------------------
void __fastcall TPlottingThread::Execute()
{                                                        
	//---- Place thread code here ----

   while (!Terminated) {
		if (!MultiPLOTPanelParent) {
			continue;
		}
		if (MultiPLOTPanelParent->IsRealTimePlot && MultiPLOTPanelParent->RTPlotEnabled){
			Synchronize( &Plot );
//			Plot();
			Sleep(1);
		}
		else Sleep(10);
	};
}
//---------------------------------------------------------------------------
void __fastcall TPlottingThread::Plot()
{
   EnterCriticalSection(&MultiPLOTPanelParent->Plot_CS);
	if (MultiPLOTPanelParent->HandleAllocated()) {
		if (Terminated) return;
		if (MultiPLOTPanelParent->IsRealTimePlot){
			for (int i=0; i<MultiPLOTPanelParent->NumVisiblePlots; i++){
				if (Terminated) return;
				OK = MultiPLOTPanelParent->PLOTPanel[i]->PlotNextBuffer(&CurrentBufferIndex);
			}
			Application->ProcessMessages();
		}
	}
	LeaveCriticalSection(&MultiPLOTPanelParent->Plot_CS);
}
//---------------------------------------------------------------------------
