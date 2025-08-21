//---------------------------------------------------------------------------
#ifndef ChooseDirDlgH
#define ChooseDirDlgH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TChooseDirDialog : public TComponent
{
private:
   AnsiString FDirectory;
   TLabel *FDirLabel;
   void __fastcall SetDirLabel(TLabel *label);
   void __fastcall SetDirectory(AnsiString dir);
   AnsiString __fastcall GetDirectory();
protected:
public:
   bool __fastcall Execute();
	__fastcall TChooseDirDialog(TComponent* Owner);
__published:
   __property 	AnsiString Directory = {read = FDirectory, write = FDirectory};
   __property  TLabel* DirLabel = {read = FDirLabel, write = SetDirLabel};
};
//---------------------------------------------------------------------------
#endif
 