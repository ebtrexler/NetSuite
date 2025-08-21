//---------------------------------------------------------------------------

#ifndef HexToTBLPacketsH
#define HexToTBLPacketsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TReprogramForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TOpenDialog *OpenDialog1;
	TLabel *Label3;
	TButton *Close;
	TGroupBox *GroupBox1;
	TLabel *Filename;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Open;
	TButton *Convert;
	TComboBox *ComPortChooser;
	TProgressBar *ProgressBar1;
	TLabel *Label4;
	void __fastcall OpenClick(TObject *Sender);
	void __fastcall ConvertClick(TObject *Sender);
private:	// User declarations
	TStringList *HexStrings;
	TMemoryStream *MemStream;
	int aton(unsigned char ch)     	/* ascii character to a numerical value */
	{
		int n;
		if(ch<0x3A)n=ch-0x30;
		else n=ch-0x37;
		return n;
	}
	unsigned long FNumPackets;
	unsigned char *FBytes;
	unsigned char *GotoStartBytes;
	unsigned char **FPackets;
//	unsigned char __fastcall *GetBytes();
//	unsigned char __fastcall *GetPacket(int Index);
//	int __fastcall GetNumPackets();
	bool __fastcall HexToTinyBLPackets(TStringList *strings);
public:		// User declarations
	__fastcall TReprogramForm(TComponent* Owner);
//	__property unsigned long NumPackets = {read = GetNumPackets};
//	__property unsigned char *Bytes = {read = GetBytes};
//	__property unsigned char *Packets[int Index] = {read = GetPacket};
};
//---------------------------------------------------------------------------
extern PACKAGE TReprogramForm *ReprogramForm;
//---------------------------------------------------------------------------
#endif
