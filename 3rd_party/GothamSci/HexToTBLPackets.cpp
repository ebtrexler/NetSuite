//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HexToTBLPackets.h"
#include "serial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TReprogramForm *ReprogramForm;
//---------------------------------------------------------------------------
__fastcall TReprogramForm::TReprogramForm(TComponent* Owner)
	: TForm(Owner)
{
	HexStrings = new TStringList;
	MemStream = new TMemoryStream;

}
//---------------------------------------------------------------------------
void __fastcall TReprogramForm::OpenClick(TObject *Sender)
{
	if (OpenDialog1->Execute()) {
		HexStrings->LoadFromFile(OpenDialog1->FileName);
		Label1->Caption = OpenDialog1->FileName;
		Memo1->Clear();
		Memo1->Lines->AddStrings(HexStrings);
	}
}
//---------------------------------------------------------------------------
void __fastcall TReprogramForm::ConvertClick(TObject *Sender)
{
	HexToTinyBLPackets(HexStrings);
	unsigned char 			recv;
	SERIAL_CONTEXT       sc;
	bool                 ComOpened;
	#pragma pack(push, 1)

	struct PACKET
	{
		unsigned char zero;
		unsigned short addr;
		unsigned char length;
		unsigned char data[64];
		unsigned char crc;
	};

	union DATAPACKET{
		PACKET p;
		unsigned char bytedata[69];
	} tosend;

	#pragma pack(pop)

	unsigned char crc;
	unsigned short check = 0x100;
	unsigned char *bytesptr = FBytes;
	unsigned char *packetptr = tosend.bytedata;

	unsigned char packet[69];
	unsigned char swap;
	tosend.p.zero = 0;
	tosend.p.addr = 0;
	tosend.p.length = 0x40;  // 64 bytes length of data

	bool sendpacket;


	ComOpened = InitSerial(&sc, ComPortChooser->ItemIndex + 1, 19200);
	if (ComOpened) {
		this->Cursor = crHourGlass;
		Application->ProcessMessages();
		SendSerialByte(&sc, 0x7F); // assume pic is waiting for it -> send to reset
		::Sleep(300);
		SendSerialByte(&sc, 0xC1); //
		RecvSerialByte(&sc, &recv, 200);// pic identifier byte
		RecvSerialByte(&sc, &recv, 10);// 'K' byte from pic saying OK
		if (recv != 'K') {
				Application->MessageBox(L"Microcontroller Not ready...",L"Error");
				CloseSerial(&sc);
				return;
		}

		ProgressBar1->Max = 125;
		for (int i=0; i < 125; i++) {  //8000 bytes / 64 = 125
			ProgressBar1->Position = i;
			memcpy(tosend.p.data, bytesptr, 64);
			tosend.p.crc = 0;
			crc = 0;
			recv = 0;
			sendpacket = false;
			for (int j=0; j<64; j++) {
				if (tosend.p.data[j] != 0xFF){ //any program bytes in packet?
					sendpacket = true;
					break;
				}
			}
			if (sendpacket) {
				packetptr = tosend.bytedata;
				for (int j=0; j<68; j++) {
					crc += *packetptr++;
				}
				tosend.p.crc = check - crc;

				memcpy(packet, tosend.bytedata, 69);
				swap = packet[2];
				packet[2] = packet[1];
				packet[1] = swap;  //byte swap for correct order
				SendSerialBlock(&sc, packet, 69);
				RecvSerialByte(&sc, &recv, 100);// 'K' byte from pic saying OK
				if (recv != 'K') {
					Application->MessageBox(L"error in send",L"Error");
					CloseSerial(&sc);
					return;
				}
			}
			tosend.p.addr += 0x40;  //increment address
			bytesptr += 64;  //0x40
		}

		ProgressBar1->Position = 0;
		this->Cursor = crArrow;
		Application->ProcessMessages();
		CloseSerial(&sc);
	}
}
//---------------------------------------------------------------------------

bool __fastcall TReprogramForm::HexToTinyBLPackets(TStringList *strings)
{
	unsigned char ch, *inputbytes;
	int inputindex=0;
	TStringList *hexstrings = new TStringList;
	hexstrings->Assign(strings);
	hexstrings->Delete(0);
	hexstrings->Delete(0);     // remove first two strings
	hexstrings->Insert(0, ":04000000A0EF0FF06E\r\n"); // insert goto statement for bootloader

	AnsiString program_goto = strings->Strings[1];
// example	:0400000017EF01F005	 should be :041F380017EF01F005 ==> Change address
	char *addrchange = program_goto.c_str();
	addrchange[3] = '1';
	addrchange[4] = 'F';
	addrchange[5] = '3';
	addrchange[6] = '8';
	hexstrings->Insert(hexstrings->Count-1, addrchange);

	AnsiString inputstring = hexstrings->Text;
	inputbytes = (unsigned char *)inputstring.data();
	unsigned char pad = 0xFF;
	unsigned char *padptr = &pad;
	int temp;

	MemStream->Clear(); // reset memstream from last programming, if any
	
	int count=0,u,t;                /* count is the position the next data is */
				/*	-inserted into the output file */
				/* u is the position the data from a line in */
				/* 	-the hex file starts in the output */
				/*	-file.  It is obtained from characters */
				/*	-4 to 7 at the start of each line in */
				/*	-the input (hex) file. */
				/* t is the number of data bytes in the */
				/*	-current line in the hex file */

	while(1)
	{
		while(inputbytes[inputindex++]!=':');	/* look for start of line of i/p file */
					/* next two characters are no. of bytes for */
					/* 	-o/p file in this line */
		t = 16*aton(inputbytes[inputindex++]);
		t += aton(inputbytes[inputindex++]);
		if(t==0)
					/* if 0, this is the last line in i/p file */
					/*	-there is no data in it, so finish */
		{
			break;
		}
		u=16*16*16*aton(inputbytes[inputindex++])+16*16*aton(inputbytes[inputindex++])+16*aton(inputbytes[inputindex++])+aton(inputbytes[inputindex++]);
					/* o/p file adress of data */

		inputindex += 2;  /* next two chars always zero (I think) */

		while(u>count)          /* next comes data */
		{                       /* if count < adress of this line's data then */
					/*	-there is a gap in the data and we */
					/*	-need to fill this gap with padding */
					/*	-in this case '00' */
					MemStream->WriteBuffer(padptr, 1);//  fputc(0,otfile);
			 count++;            /* no. of bytes to fill = u-count */
			  }
	/*	while(t>0)*/
		while(u+t-count>0)
			  {
			  temp = 16*aton(inputbytes[inputindex++])+aton(inputbytes[inputindex++]);
			 MemStream->WriteBuffer(&temp,1);
					/* get next two ascii chars and turn them */
					/*	-into decimal value of byte to go */
					/*	-into o/p file */
	/*	    t--;*/
			 count++;
			  }
		 }

		 if (FBytes) delete[] FBytes;
		 FBytes = new char[count];
		 MemStream->Seek(0, soFromBeginning);
		 MemStream->ReadBuffer(FBytes, count);

		 delete hexstrings;

       return true;
	}



