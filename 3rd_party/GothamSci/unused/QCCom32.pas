unit QCCom32;
(*
(c) 1996 Quantum Composers, Bozeman, MT
This file contains Delphi source code to a Delphi Control used for
serial port communications. 32-bit version.
Author: Scott Pinkham
Date Created: 2/9/96
*)
interface

uses
	Windows,
  Classes,
  ExtCtrls,
  SysUtils,
  Forms,
  Dialogs,
  Graphics,
  Controls,
  Buttons,
  StdCtrls;

{ Class Definition ------------------------------}

{ dialog box to pick a COM port }
type
	TformPickCom = class(TForm)
		OKBtn: TBitBtn;
		CancelBtn: TBitBtn;
		radioCom: TRadioGroup;

	private
	  { Private declarations }
	public
	  { Public declarations }
end;

{ ComPort class }
type
	TQCCom32 = class(TComponent)

	private  { Private declarations }
		hPort: LongInt; {handle from OpenComm }
		nPort: Integer; { port #, 1-based }
		lBaud: LongInt; { baud rate }
		sInTerminator: String; { response terminator string }
		boolExpired: Boolean; { set true when a timeout occurs }
		Timer: TTimer; {timeout timer }
		formPickCom: TformPickCom; { dialog box to pick a port }
		boolShowErrors: Boolean; { true to enable error boxes }
		boolInUse: Boolean; { true while port is blocked, waiting for response }

		{ Events (method pointers) user can assign code to these }
		pmOnTimeout: TNotifyEvent;	{ timeout event }

		{ functions }
		procedure SetTimeout(lTimeout: LongInt);
		function GetTimeout: LongInt;
		procedure SetBaud(lBaudToSet: LongInt);
		procedure SetPort(nPortToSet: Integer);
		function GetInCount: LongInt;
		procedure TimesUp(Sender: TObject);
		function IsOpen: Boolean;

		{ dialog box button handlers }
		//procedure FormActivate(Sender: TObject);
		//procedure OKBtnClick(Sender: TObject);

	protected
		{ Protected declarations }
		procedure Timeout; dynamic;

	public { Public declarations }
		{ Methods }
		constructor Create(AOwner: TComponent); override;
		destructor Destroy; override;
		procedure Pick;
		function Open: Boolean;
		function Write(const sData: String): Boolean;
		function Read: String;
		procedure Flush;
		procedure Close;

      procedure SetComPort(nPortToSet: Integer); // added by ebt 3/31/05

	published { Published declarations }
		{ timeout for waiting for a response, in mS }
		property ResponseTime: LongInt read GetTimeout write SetTimeout;
		{ timeout event }
		property OnTimeout: TNotifyEvent read pmOnTimeout write pmOnTimeout;
		{ flag set when a timeout occured }
		property TimedOut: Boolean read boolExpired;
		{ baud rate }
		property Baud: LongInt read lBaud write SetBaud;
		{ COM port }
		property Port: Integer read nPort write SetPort;
		{ number of characters received }
		property InCount: LongInt read GetInCount;
		{ string to signal end of input from external device }
		property EndOfResponse: String read sInTerminator write sInTerminator;
		{ flag to enable showing error message boxes }
		property ShowErrors: Boolean read boolShowErrors write boolShowErrors;
		{ flag indicating if port is open }
		property Opened: Boolean read IsOpen;
		{ flag indication if port is blocked }
		property InUse: Boolean read boolInUse;
end;

//procedure Register;

implementation

//{$R *.DFM}

{
Register the component with the Delphi IDE
}
{
  procedure Register;
  begin
  	RegisterComponents('Components', [TQCCom32]);
  end;
}

{
Component constructor
}
constructor TQCCom32.Create(AOwner: TComponent);
begin
	inherited Create(AOwner);
	{ set default property values }
	hPort := INVALID_HANDLE_VALUE; { invalidate to start }
	lBaud := 9600;
	nPort := 1;
	boolExpired := False;
	boolShowErrors := True;
	boolInUse := False;

	{ create a timer for checking for timeout }
	Timer := TTimer.Create(Self);
	Timer.Enabled := False;
	Timer.Interval := 1000; { 1 second }
	Timer.OnTimer := TimesUp;
end;

{
Component destructor
}
destructor TQCCom32.Destroy;
begin
	{ deallocate the timer }
	Timer.Free;
	Timer := nil;
	{ close the com port (if open) }
	Close;
	inherited Destroy;	{ destroy ancestor class }
end;

{
Set the Timeout property to the specified milliseconds
}
procedure TQCCom32.SetTimeout(lTimeout: LongInt);
begin
	if Assigned(Timer) then Timer.Interval := lTimeout;
end;

{
Get the current timeout setting
}
function TQCCom32.GetTimeout: LongInt;
begin
	Result := 0;
	if Assigned(Timer) then Result := Timer.Interval;
end;

{
Return True if port is open
}
function TQCCom32.IsOpen: Boolean;
begin
	//if hPort <> INVALID_HANDLE_VALUE then Result := True else Result := False;
	Result := (hPort <> INVALID_HANDLE_VALUE);
end;

{
Set the baud rate property
}
procedure TQCCom32.SetBaud(lBaudToSet: LongInt);
begin
	if lBaudToSet <> lBaud then
	begin
		lBaud := lBaudToSet;
		{ if port is open, then close it and then reopen it
		to reset the baud rate }
		if IsOpen then
		begin
			Close;
			Open;
		end;
	end;
end;

{
Set the Port property
}
procedure TQCCom32.SetPort(nPortToSet: Integer);
begin
	if nPortToSet <> nPort then
	begin
		nPort := nPortToSet;
		{ if port was open, then close and reopen it}
		if hPort <> INVALID_HANDLE_VALUE then
		begin
			Close;
			Open;
		end;
	 end;
end;

{
Public function to Set the Port property
}
procedure TQCCom32.SetComPort(nPortToSet: Integer);
begin
	if nPortToSet <> nPort then
	begin
		nPort := nPortToSet;
		{ if port was open, then close and reopen it}
		if hPort <> INVALID_HANDLE_VALUE then
		begin
			Close;
			Open;
		end;
	 end;
end;

{ Pops a dialog to pick the COM port number }
procedure TQCCom32.Pick;
begin
	formPickCom := TformPickCom.Create(Self); // create the box
	formPickCom.radioCOM.ItemIndex := nPort - 1;
	formPickCom.ShowModal; // show it
	if formPickCom.ModalResult = mrOk then	SetPort(formPickCom.radioCOM.ItemIndex + 1);
	formPickCom.Destroy; // throw it away
end;

{ Opens the COM port, returns True if ok }
function TQCCom32.Open: Boolean;
var
	sCom: String;
	dcbPort: TDCB; {device control block }
	boolAbort: Boolean;
	sErrMsg: String;
begin

	{ init }
	boolAbort := True;
	//boolInUse := True;

	{ close port if open already }
	if hPort <> INVALID_HANDLE_VALUE then Close;

	{ try to open the port }
	repeat
	begin
		sCom := 'COM' + IntToStr(nPort);
		hPort := CreateFile(PChar(sCom), GENERIC_READ or GENERIC_WRITE, 0, nil,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, LongInt(0));

		if (hPort = INVALID_HANDLE_VALUE) and boolShowErrors then
		begin
			if MessageDlg('Error opening COM' + IntToStr(nPort) + ': ' + sErrMsg,
				mtWarning, [mbAbort, mbRetry], 0) = idAbort then
				boolAbort := True
			else
				boolAbort := False;
			end;
		end;
	until (hPort <> INVALID_HANDLE_VALUE) or (boolAbort = True);

	{ set the baud rate and other parameters }
	if (hPort <> INVALID_HANDLE_VALUE) then
	begin
		if GetCommState(hPort, dcbPort) then
		begin
			{ fill in the fields of the structure }
			dcbPort.BaudRate := lBaud;
			dcbPort.ByteSize := 8;
			dcbPort.Parity := NOPARITY;
			dcbPort.StopBits := ONESTOPBIT;
			dcbPort.Flags := 0;
			{ flag bit fields:
			dcb_Binary, dcb_Parity, dcb_OutxCtsFlow, dcb_fOutxDsrFlow,
			dcb_fOutX, dcb_fInX, dcb_DtrFlow, dcb_RtsFlow
			}
			SetCommState(hPort, dcbPort);
		end;
	end;

	{ return True if handle is valid }
	Result := (hPort <> INVALID_HANDLE_VALUE);
	//boolInUse := False;

end;

{ Close the COM port }
procedure TQCCom32.Close;
begin
	if hPort <> INVALID_HANDLE_VALUE then CloseHandle(hPort);
	hPort := INVALID_HANDLE_VALUE;
	//boolInUse := False;
end;

{ Write a string out the COM port, return true if all chars written }
function TQCCom32.Write(const sData: String): Boolean;
var
	dwCharsWritten: DWord;

begin
	{ init }
	//boolInUse := True;
	dwCharsWritten := 0;
	Result := False; { default to error return }

	if hPort <> INVALID_HANDLE_VALUE then
	begin
		WriteFile(hPort, PChar(sData)^, Length(sData), dwCharsWritten, nil);
		if dwCharsWritten = Length(sData) then Result := True;
	end;

	//boolInUse := False;

end;

{ Return the number of bytes waiting in the queue }
function TQCCom32.GetInCount: LongInt;
var
	statPort: TCOMSTAT;
	dwErrorCode: DWord;
begin
	Result := 0;
	if hPort <> INVALID_HANDLE_VALUE then
	 begin
		ClearCommError(hPort, dwErrorCode, @statPort);
		Result := statPort.cbInQue;
	 end;
end;

{
Reads a string from the port, puts it into pchBuffer, returns the
number of characters read
}
function TQCCom32.Read: String;
const
	BUF_LEN = 1024;
var
	cbCharsAvailable, cbCharsRead: DWord;
	boolExit: Boolean;
	sBuffer: String;

begin
	{ init }
	SetLength(sBuffer, BUF_LEN);
	Result := '';

   { check boolInUse in case of rentrancy }
	if boolInUse then
	begin
		if boolShowErrors then ShowMessage('Port is in use -- operation aborted');
		Exit;
	end
	else
		boolInUse := True;

		if hPort <> INVALID_HANDLE_VALUE then
		begin
			{ if no terminator is defined, simply read any available data and return }
			if Length(sInTerminator) = 0 then
			begin
				cbCharsAvailable := GetInCount;
				if cbCharsAvailable > 0 then
				begin
					SetLength(Result, cbCharsAvailable + 1); { allocate space }
					ReadFile(hPort, PChar(Result)^, cbCharsAvailable, cbCharsRead, nil);
					SetLength(Result, StrLen(PChar(Result))); { adjust length }
				end;
			end
		else { a terminator is defined, so read port until terminator found or timed out }
		begin
			boolExit := False;

			repeat
				boolExpired := False;
				Timer.Enabled := True;	{ start the timeout timer }

				{ loop until timeout or terminator recieved }
				repeat
					cbCharsAvailable := GetInCount;
					if cbCharsAvailable > 0 then
					begin
						if cbCharsAvailable >= Length(sBuffer) then
							SetLength(sBuffer, cbCharsAvailable + 1); { allocate space }

						ReadFile(hPort, PChar(sBuffer)^, cbCharsAvailable, cbCharsRead, nil);

						{ append chars read to end of result buffer }
						Result := Result + Copy(sBuffer, 0, StrLen(PChar(sBuffer)));
					end;

					if Pos(sInTerminator, Result) <> 0 then
					begin
						boolExit := True;
					end
					else
					begin
						{ give timer message a chance to occur - watch out for rentrancy!!}
						Application.ProcessMessages;
					end;

				until (boolExpired) or (boolExit);

				Timer.Enabled := False; { stop the timer }

				{ If timed out, then give user a chance to retry }
				if boolExpired and boolShowErrors then
				begin
					if MessageDlg('Timeout waiting for response.', mtWarning,
					[mbAbort, mbRetry], 0) = idAbort then boolExit := True;
				end;

			until boolExit = True;
		end;
	end;
	boolInUse := False;
end;

{
Timer procedure - signals a timeout
}
procedure TQCCom32.TimesUp(Sender: TObject);
begin
	boolExpired := True;
	Timer.Enabled := False;
	Timeout; // execute user code
end;

{
Timeout event - user assigned code
}
procedure TQCCom32.Timeout;
begin
	{ if user has assigned code to the timeout event, then call it }
	if Assigned(pmOnTimeout) then pmOnTimeout(self);
end;

{
Flush the port by reading any characters in the queue
}
procedure TQCCom32.Flush;
begin
	if hPort <> INVALID_HANDLE_VALUE then
	begin
		PurgeComm(hPort, PURGE_TXABORT or PURGE_RXABORT or PURGE_TXCLEAR or PURGE_RXCLEAR);
	end;
end;




//
// Dialog box functions
//

{
procedure TQCCom32.FormActivate(Sender: TObject);
begin
	formPickCom.radioCOM.ItemIndex := nPort - 1;
end;

procedure TQCCom32.OKBtnClick(Sender: TObject);
begin
	SetPort(formPickCom.radioCOM.ItemIndex + 1);
end;
}
end.
