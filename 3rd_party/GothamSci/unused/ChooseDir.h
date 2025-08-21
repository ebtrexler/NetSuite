//---------------------------------------------------------------------------
#ifndef ChooseDirH
#define ChooseDirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TChooseDirForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TLabel *Label1;
	TDirectoryListBox *DirectoryListBox1;
	TDriveComboBox *DriveComboBox1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);


private:	// User declarations
	AnsiString FChosenDirectory;
  AnsiString __fastcall GetChosenDirectory()
  	{
  		FChosenDirectory = DirectoryListBox1->Directory;
      return FChosenDirectory;
    }
  void __fastcall SetChosenDirectory(AnsiString cd)
  	{
    	FChosenDirectory = cd;
      DirectoryListBox1->Directory = cd;
    }
public:		// User declarations
	__property AnsiString ChosenDirectory =
  		{read = GetChosenDirectory, write = SetChosenDirectory};
	__fastcall TChooseDirForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TChooseDirForm *ChooseDirForm;
//---------------------------------------------------------------------------
#endif
