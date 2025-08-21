{
  File Name: AudioIO.PAS  V 4.00
  Created: 5-Oct-96 by John Mertus on the IBM PC
  Revision #1: 5-Oct-22 by John Mertus
                                        -John Mertus

  Version 1.00 Initial Release

}

{
  There are three Sound Components, the first is the base Component,
  TAudioIO.  This defines the sampling rates, buffers and some of the
  common events.

  The second component is AudioOut, which started just loops playing out
  buffers.

  The third component is AudioIN, which, when started, just loops filling
  buffer with digital data.

  See AudioIO.Hlp for detailed explaination.



}


{-----------------Unit-AudioOut-------------------John Mertus---Oct 96---}

                       Unit AudioIO;

{*************************************************************************}
                            Interface



uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, MMSystem, UAFDefs;

{ Could make this dynamic, but the effort doesn't seem worth it. }
Const
  MAXBUFFERS = 4;

Type

  {TBuffer Event is what is called when a buffer is need or is full }

  TBufferEvent = procedure(Sender : TObject; Buffer : lpstr;
                           Var Size : Integer; Var Continue : Boolean) of Object;
  PAudioIO = ^TAudioIO;
  PAudioOut = ^TAudioOut;
  PAudioIn  = ^TAudioIn;

  TCallBackWinOut = Class(TWinControl)
  private
    { Private declarations }
    AudioComponent : PAudioOut;

    procedure BufferDone(var Msg: TMessage); message  MM_WOM_DONE;
    procedure WaveOpen(var Msg: TMessage); message  MM_WOM_OPEN;
    procedure WaveClose(var Msg: TMessage); message  MM_WOM_CLOSE;

  protected
    { Protected declarations }

  public
    { Public declarations }

  published
    { Published declarations }

  End;


  TCallBackWinIn = Class(TWinControl)
  private
    { Private declarations }
    AudioComponent : PAudioIn;

    procedure BufferFinished(var Msg: TMessage); message  MM_WIM_DATA;
    procedure WaveOpenIn(var Msg: TMessage); message  MM_WIM_OPEN;
    procedure WaveCloseIn(var Msg: TMessage); message  MM_WIM_CLOSE;

  End;


{---------------------------TAudioIO Component-----------------------------}

  TAudioIO = class(TComponent)
  protected
    { Private declarations }
    FBufferSize : Integer;            { Actual buffer used }
    FRequestedBufferSize : Integer;   { Buffer size requested }
    FNumBuffers          : Integer;
    FPaused              : Boolean;

    FWaveFmtEx         : TWaveFormatEx;
    FonOpen            : TNotifyEvent;
    FonClose           : TNotifyEvent;
    FWaveDevice        : DWord;
    hWaveHeader	       : Array [0..MAXBUFFERS-1] of THANDLE;
    WaveHdr            : Array [0..MAXBUFFERS-1] of PWAVEHDR;
    WaveBuffer         : Array [0..MAXBUFFERS-1] of lpstr;
    hWaveBuffer        : Array [0..MAXBUFFERS-1] of THANDLE;
    BufIndex           : Integer;
    ContinueProcessing : Boolean;     { Set to TRUE to start FALSE to abort
                                         after filled buffers are done }

    { Property Functions }
    Procedure SetNumBuffers(Value : Integer);
    Procedure SetBufferSize(Value : Integer);
    Procedure SetFrameRate(Value : Integer);
    Procedure SetStereo(Value : Boolean);
    Procedure SetBits(Value : Word);
    Function  GetFrameRate : Integer;
    Function  GetStereo : Boolean;
    Procedure MakeWaveFmtConsistent;

  protected
    { Protected declarations }
    Function InitWaveHeaders : Boolean;
    Function AllocPCMBuffers : Boolean;
    Function FreePCMBuffers : Boolean;
    Function AllocWaveHeaders : Boolean;
    Procedure FreeWaveHeaders;

  public
    { Public declarations }
    ErrorMessage   : String;
    Active         : Boolean;
    FilledBuffers,
    QueuedBuffers,
    ProcessedBuffers   : Integer;

    Constructor Create(AOwner: TComponent); Override;
    Destructor  Destroy; Override;
    Procedure StopAtOnce;                 Virtual;
    Procedure StopGracefully;             Virtual;

  published
    { Published declarations }
      Property BufferSize : Integer read FBufferSize write SetBufferSize Default 8192;
      Property NumBuffers : Integer read FNumBuffers write SetNumBuffers Default 4;
      Property FrameRate : Integer read GetFrameRate Write SetFrameRate Default 22055;
      Property Stereo : Boolean read GetStereo Write SetStereo Default False;
      Property Quantization : Word Read FWaveFmtEx.wBitsPerSample Write SetBits Default 16;
      Property WaveDevice : DWord Read FWaveDevice Write FWaveDevice Default (0); //WAVE_MAPPER;
      Property OnStart : TNotifyEvent Read FOnOpen Write FOnOpen;
      Property OnStop  : TNotifyEvent Read FOnClose Write FOnClose;
  end;


{---------------------------TAudioOut Component-----------------------------}


  TAudioOut = Class(TAudioIO)
  private
    { Private declarations }
    WaveDeviceOpen : Boolean;
    CallBackWin    : TCallBackWinOut;
    FOnFillBuffer  : TBufferEvent;

    Function  QueueBuffer : Boolean;
    Function  ReadBuffer(Idx, N : Integer) : Boolean;  Virtual;
    Procedure SetPaused(Value : Boolean);
    Procedure CloseWaveDevice;   Virtual;
    Function  Setup(Var TS: TAudioOut) : Boolean;  Virtual;
    Function  StartIt : Boolean;

  protected
    { Protected declarations }

  public
    { Public declarations }
    WaveHandle     : HWaveOut;     { Waveform output handle }

    Function  Start(Var TS : TAudioOut) : Boolean;
    Procedure StopAtOnce;                         Override;
    Procedure StopGracefully;                     Override;
    Function  ElapsedTime : Real;

  published
    { Published declarations }
    Property Paused : Boolean Read FPaused Write SetPaused Default FALSE;
    Property OnFillBuffer : TBufferEvent Read FOnFillBuffer Write FOnFillBuffer;
  End;


{---------------------------TAudioIn Component-----------------------------}


  TAudioIn = Class(TAudioIO)
  private
    { Private declarations }
    WaveDeviceOpen : Boolean;
    CallBackWin      : TCallBackWinIn;
    FOnBufferFilled  : TBufferEvent;

    Function  QueueBuffer : Boolean;
    Function  ProcessBuffer(B : lpstr; N : Integer) : Boolean;  Virtual;
    Procedure CloseWaveDevice;   Virtual;
    Function  Setup(Var TS: TAudioIn) : Boolean;  Virtual;
    Function  StartIt : Boolean;

  protected
    { Protected declarations }

  public
    { Public declarations }
    WaveHandle        : HWaveOut;     { Waveform output handle }
    Function  Start(Var TS : TAudioIn) : Boolean;
    Procedure StopAtOnce;                       Override;
    Procedure StopGracefully;                   Override;
    Function  ElapsedTime : Real;

  published
    { Published declarations }
    Property OnBufferFilled : TBufferEvent Read FOnBufferFilled Write FOnBufferFilled;
  End;


procedure Register;

{*************************************************************************}

         implementation


{---------------TWaveOutGetErrorText------------John Mertus Oct 96---}

   Function TWaveOutGetErrorText(iErr : Integer) : String;

{  This just gets the error text assocated with the output error ierr.  }
{									}
{**********************************************************************}
Var
  ErrorMsgC   : Array [0..255] of Char;

BEGIN
  waveOutGetErrorText(iErr,ErrorMsgC,Sizeof(ErrorMsgC));
  Result := StrPas(ErrorMsgC);
END;

{---------------TWaveInGetErrorText------------John Mertus Oct 96---}

   Function TWaveInGetErrorText(iErr : Integer) : String;

{  This just gets the error text assocated with the output error ierr.  }
{									}
{**********************************************************************}
Var
  ErrorMsgC   : Array [0..255] of Char;

BEGIN
  waveInGetErrorText(iErr,ErrorMsgC,Sizeof(ErrorMsgC));
  Result := StrPas(ErrorMsgC);
END;

procedure Register;
begin
  RegisterComponents('Gotham Scientific', [TAudioOut, TAudioIn]);
end;

{---------------SetBufferSize-------------------John Mertus Oct 96---}

   Procedure TAudioIO.SetBufferSize(Value : Integer);

{  This just set the buffersize, making sure it is too small.           }
{									}
{**********************************************************************}
BEGIN
   If (Value < 512) Then Value := 512;
  { make the wave buffer size a multiple of the block align... }
   FRequestedBufferSize := Value;
   MakeWaveFmtConsistent;
   FreePCMBuffers;
   AllocPCMBuffers;
END;

{---------------SetNumBuffers-------------------John Mertus Oct 96---}

   Procedure TAudioIO.SetNumBuffers(Value : Integer);

{  This just set the numbers of buffers making sure it is between       }
{ and MaxNumberBuffers                                                  }
{									}
{**********************************************************************}
BEGIN
   If (Value < 2) Then Value := 2;
   If (Value > MAXBUFFERS) Then Value := MAXBUFFERS;
   FNumBuffers := Value;
END;

{---------------SetStereo-----------------------John Mertus Oct 96---}

   Procedure TAudioIO.SetStereo(Value : Boolean);

{  This just set the numbers of channels, True 2, false 1.              }
{									}
{**********************************************************************}
BEGIN
  If Value Then
     FWaveFmtEx.nChannels := 2
  Else
     FWaveFmtEx.nChannels := 1;
   MakeWaveFmtConsistent;
END;

{---------------SetBits-------------------------John Mertus Oct 96---}

   Procedure TAudioIO.SetBits(Value : Word);

{  This just set the numbers of buffers making sure it is between       }
{ and MaxNumberBuffers                                                  }
{									}
{**********************************************************************}
BEGIN
   If (Value < 8) Then Value := 8;
   If (Value > 8) Then Value := 16;
   FWaveFmtEx.wBitsPerSample := Value;
   MakeWaveFmtConsistent;
END;

{---------------SetFrameRate--------------------John Mertus Oct 96---}

   Procedure TAudioIO.SetFrameRate(Value : Integer);

{  This just set the frame rate for sampling.                           }
{									}
{**********************************************************************}
BEGIN
   FWaveFmtEx.nSamplesPerSec := Value;
   MakeWaveFmtConsistent;
END;

{---------------GetFrameRate--------------------John Mertus Oct 96---}

   Function TAudioIO.GetFrameRate : Integer;

{  This just returns the framerate for the current header.              }
{									}
{**********************************************************************}
BEGIN
  Result := FWaveFmtEx.nSamplesPerSec;
END;

{---------------GetStereo-----------------------John Mertus Oct 96---}

   Function TAudioIO.GetStereo : Boolean;

{  This just returns the True if stereo, e.g. 2 channels                }
{									}
{**********************************************************************}
BEGIN
  Result := (FWaveFmtEx.nChannels = 2);
END;


{-----------------Create------------------------John Mertus Oct 96---}

   Constructor TAudioIO.Create(AOwner: TComponent);

{  This just set the numbers of buffers making sure it is between       }
{ and MaxNumberBuffers                                                  }
{									}
{**********************************************************************}
Var
  i : Integer;


BEGIN
   Inherited Create(AOwner);
   FNumBuffers := 4;
   FRequestedBufferSize := 8192;
   Active := FALSE;
   FPaused := FALSE;
   FWaveDevice := WAVE_MAPPER;
   ErrorMessage := '';

  { Set the indendent sampling rates }
   FWaveFmtEx.wFormatTag := WAVE_FORMAT_PCM;
   FWaveFmtEx.wBitsPerSample := 16;
   FWaveFmtEx.nchannels := 1;
   FWaveFmtEx.nSamplesPerSec := 22050;
   MakeWaveFmtConsistent;

  { Now make sure we know buffers are not allocated }
   For i := 0 to MAXBUFFERS-1 Do WaveBuffer[i] := Nil;

   AllocWaveHeaders;
   AllocPCMBuffers;
END;

{-----------------Destroy-----------------------John Mertus Oct 96---}

   Destructor TAudioIO.Destroy;

{  This cleans up the buffers.                                          }
{									}
{**********************************************************************}
BEGIN
  FreePCMBuffers;
  FreeWaveHeaders;
  Inherited Destroy;
END;

{-----------------MakeWaveFmtConsistent---------John Mertus Oct 96---}

   Procedure TAudioIO.MakeWaveFmtConsistent;

{  This just trys to find the correct avgbytes and blockalign that      }
{ one needs to use for the format.  I DO NOT UNDERSTAND WHY MICROSOFT   }
{ did this.                                                             }
{									}
{**********************************************************************}
BEGIN
   With FWaveFmtEx Do
     Begin
        nBlockAlign := (wBitsPerSample div 8)*nchannels;
        nAvgBytesPerSec := nSamplesPerSec*nBlockAlign;
     End;

   FBufferSize := FRequestedBufferSize - (FRequestedBufferSize mod FWaveFmtEx.nBlockAlign);
END;

{-------------InitWaveHeaders----------------John Mertus---14-June--97--}

   Function TAudioIO.InitWaveHeaders : Boolean;

{ This just initalizes the waveform headers, no memory allocated        }
{									}
{**********************************************************************}
Var
  i : Integer;

BEGIN
  { This should not be necessary, but to be safe... }
  MakeWaveFmtConsistent;

  { Set the wave headers }
  For i := 0 to FNumBuffers-1 Do
    With WaveHdr[i]^ Do
      Begin
        lpData := WaveBuffer[i];         // address of the waveform buffer
        dwBufferLength := FBufferSize;   // length, in bytes, of the buffer
        dwBytesRecorded := 0;            // see below
        dwUser := 0;                     // 32 bits of user data
        dwFlags := 0;                    // see below
        dwLoops := 0;                    // see below
        lpNext := Nil;                   // reserved; must be zero
        reserved := 0;                   // reserved; must be zero
      End;

  InitWaveHeaders := TRUE;
END;


{-------------AllocPCMBuffers----------------John Mertus---14-June--97--}

   Function TAudioIO.AllocPCMBuffers : Boolean;

{ Allocate and lock the waveform memory.                                }
{									}
{***********************************************************************}
Var
  i : Integer;

BEGIN
  For i := 0 to fNumBuffers-1 Do
    begin
      hWaveBuffer[i] := GlobalAlloc( GMEM_MOVEABLE or GMEM_SHARE, fBufferSize );
      If (hWaveBuffer[i] = 0) Then
	begin
          FreePCMBuffers;
          ErrorMessage := 'Error allocating wave buffer memory';
          AllocPCMBuffers := False;
          Exit;
	end;

      WaveBuffer[i] := GlobalLock(hWaveBuffer[i]);
      If (WaveBuffer[i] = Nil) Then
	begin
          FreePCMBuffers;
          ErrorMessage := 'Error Locking wave buffer memory';
          AllocPCMBuffers := False;
          Exit;
	end;
      WaveHdr[i].lpData := WaveBuffer[i];
    End;

  AllocPCMBuffers := TRUE;
END;

{--------------FreePCMBuffers----------------John Mertus---14-June--97--}

   Function TAudioIO.FreePCMBuffers : Boolean;

{ Free up the meomry AllocPCMBuffers used.                              }
{									}
{***********************************************************************}
Var
  i : Integer;

BEGIN

  Result := FALSE;

  For i := 0 to MaxBuffers-1 Do
    begin
      If (hWaveBuffer[i] <> 0) Then
        Begin
  	  GlobalUnlock(hWaveBuffer[i] );
	  GlobalFree(hWaveBuffer[i] );
          hWaveBuffer[i] := 0;
          WaveBuffer[i] := Nil;
          Result := TRUE;
        End;
    end;

END;
{-------------AllocWaveHeaders---------------John Mertus---14-June--97--}

   Function TAudioIO.AllocWaveHeaders : Boolean;

{ Allocate and lock header memory                                       }
{									}
{***********************************************************************}
Var
  i : Integer;

BEGIN
  For i := 0 to MAXBUFFERS-1 Do
    begin
      hwaveheader[i] := GlobalAlloc( GMEM_MOVEABLE or GMEM_SHARE or GMEM_ZEROINIT, sizeof(TWAVEHDR));
      if (hwaveheader[i] = 0) Then
        begin
          FreeWaveHeaders;
          ErrorMessage := 'Error allocating wave header memory';
          AllocWaveHeaders := FALSE;
          Exit;
        end;

      WaveHdr[i] := GlobalLock (hwaveheader[i]);
      If (WaveHdr[i] = Nil ) Then
        begin
          FreeWaveHeaders;
          ErrorMessage := 'Could not lock header memory for recording';
          AllocWaveHeaders := FALSE;
          Exit;
        end;

    End;

  AllocWaveHeaders := TRUE;
END;

{---------------FreeWaveHeaders---------------John Mertus---14-June--97--}

   Procedure TAudioIO.FreeWaveHeaders;

{ Just free up the memory AllocWaveHeaders allocated.                   }
{									}
{***********************************************************************}
Var
  i : Integer;

BEGIN
  For i := 0 to MAXBUFFERS-1 Do
    begin
      If (hWaveHeader[i] <> 0) Then
        Begin
          GlobalUnlock(hwaveheader[i]);
          GlobalFree(hwaveheader[i]);
          hWaveHeader[i] := 0;
          WaveHdr[i] := Nil;
        End
    end;
END;

{--------------------StopAtOnce-------------John Mertus---14-June--97--}

   Procedure TAudioIO.StopAtOnce;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
BEGIN
   Active := False;
   ContinueProcessing := FALSE;
END;

{--------------------StopGracefully---------John Mertus---14-June--97--}

   Procedure TAudioIO.StopGracefully;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
BEGIN
  ContinueProcessing := FALSE;
END;

{-----------------ElapsedTime----------------John Mertus---14-June--97--}

   Function TAudioOut.ElapsedTime : Real;

{ This function returns the time since start of playout.                }
{									}
{**********************************************************************}
Var
  pmmt  : TMMTime;

BEGIN
  If (Active) Then
    Begin
      pmmt.wType := TIME_SAMPLES;
      If (waveOutGetPosition(WaveHandle, @pmmt, Sizeof(TMMTime)) <> 0) Then
         Result := 0
      Else
         Result := pmmt.Sample/FrameRate;
    End
  Else
    Result := 0;
END;

{---------------SetPaused-----------------------John Mertus Oct 96---}

   Procedure TAudioOut.SetPaused(Value : Boolean);

{  This pauses or restarts the output.                                  }
{									}
{**********************************************************************}
BEGIN
  FPaused := Value;
  If (Not Active) Then Exit;
  If FPaused Then
    WaveOutPause(WaveHandle)
  Else
    WaveOutReStart(WaveHandle);
END;

{-------------CloseWaveDevice----------------John Mertus---14-June--97--}

   Procedure TAudioOut.CloseWaveDevice;

{ Closes the wave output device.                                        }
{									}
{**********************************************************************}
Var
  i : Integer;

BEGIN
{ unprepare the headers }
  Active := FALSE;
  Paused := FALSE;
  For i := 0 to FNumBuffers-1 Do
    waveOutUnprepareHeader( WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));

{ close the device }
   waveOutClose(WaveHandle);
   WaveDeviceOpen := FALSE;

END;

{-------------SetupOutput--------------------John Mertus---14-June--97--}

   Function TAudioOut.Setup(Var TS : TAudioOut) : Boolean;

{ This function just sets up the board for output.                      }
{									}
{**********************************************************************}
Var
  iErr       : Integer;
  i          : Integer;

BEGIN

  { if the device is still open, return error }
  If (WaveDeviceOpen) Then
    Begin
      ErrorMessage := 'Wave output device is already open';
      Result := FALSE;
      Exit;
    End;

  BufIndex := 0;

  { Now create the window component to handle the processing }
  CallBackWin := TCallBackWinOut.CreateParented(TWinControl(Owner).Handle);
  CallBackWin.Visible := FALSE;
  CallBackWin.AudioComponent := @TS;

  { Open the device for playout }
  { Either go via interrupt or window }
  iErr := WaveOutOpen(@WaveHandle, FWaveDevice, @FWaveFmtEx, Integer(CallBackWin.Handle),
                      0, CALLBACK_WINDOW or WAVE_ALLOWSYNC );

  If (iErr <> 0) Then
    Begin
      ErrorMessage := TWaveOutGetErrorText(iErr);
      Result := FALSE;
      Exit;
    End;

  WaveDeviceOpen := TRUE;

  { Setup the buffers and headers }
  If (Not InitWaveHeaders) Then
    Begin
      Result := FALSE;
      Exit;
    End;

  { Now Prepare the buffers for output }
  For i := 0 to FNumBuffers-1 Do
    Begin
      iErr := WaveOutPrepareHeader(WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));
      If (iErr <> 0) Then
        Begin
          ErrorMessage := TWaveOutGetErrorText(iErr);
          CloseWaveDevice;
          Result := FALSE;
          Exit;
        End;
    End;

  { Read in the buffers }
  QueuedBuffers := 0;
  ProcessedBuffers := 0;
  FilledBuffers := 0;
  ContinueProcessing := TRUE;
  Active := TRUE;

  If (Not ReadBuffer(0, FBufferSize)) Then
    Begin
      CloseWaveDevice;
      ErrorMessage := 'There must be at least one filled buffer';
      Result := FALSE;
      Exit;
    End;

  For i := 1 to FNumBuffers - 1 Do ReadBuffer(i, FBufferSize);

  Result := TRUE;
END;

{----------------QueueBuffer----------------John Mertus---14-June--97--}

   Function TAudioOut.QueueBuffer : Boolean;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
Var
  iErr : Integer;

BEGIN
 { reset flags field (remove WHDR_DONE attribute) }
  WaveHdr[bufindex].dwFlags := WHDR_PREPARED;

 { now queue the buffer for output }
  iErr := waveOutWrite( WaveHandle, WaveHdr[bufindex], sizeof(TWAVEHDR));
  If (iErr <> 0) Then
    Begin
      ErrorMessage := TwaveOutGetErrorText(iErr);
      StopGracefully;
      Result := FALSE;
      Exit;
    End;

  { Advance index }
  bufindex := (bufindex+1) mod FNumBuffers;

  Result := TRUE;
END;

{-------------StartIt------------------------John Mertus---14-June--97--}

   Function TAudioOut.StartIt : Boolean;

{ This function just starts the waveform playing                        }
{									}
{**********************************************************************}
Var
  i : Integer;

BEGIN
  Active := TRUE;

  If (FPaused) Then WaveOutPause(WaveHandle);
  { Now we are ready to start the output }
  If (Not QueueBuffer) Then
    Begin
      CloseWaveDevice;
      Result := FALSE;
      Exit;
    End;

  For i := 0 to FNumBuffers - 2 Do QueueBuffer;
  Result := TRUE;
END;

{-----------------Start----------------------John Mertus---14-June--97--}

   Function TAudioOut.Start(Var TS : TAudioOut) : Boolean;

{ This function first sets up the output and then starts it.            }
{									}
{**********************************************************************}
BEGIN
  Result := Setup(TS);
  If (Not Result) Then Exit;

  Result := StartIt;
  If (Not Result) Then Exit;
END;


{-------------ReadBuffer---------------------John Mertus---14-June--97--}

   Function  TAudioOut.ReadBuffer(Idx, N : Integer) : Boolean;

{ This is called whenver move buffer data is needed.                    }
{									}
{**********************************************************************}
Var
  NSize : Integer;

BEGIN
  { Do not call the read buffer routine if we want to stop }
  If (Not ContinueProcessing) Then
     Begin
        Result := FALSE;
        Exit;
     End;

  { If assigned, process the buffer, Notice that the Size returned may not
    be the size sent,  so reset the output size }
  If Assigned(FOnFillBuffer) Then
    Begin
      NSize := N;
      FOnFillBuffer(Self, WaveBuffer[idx], NSize, Result);
      WaveHdr[idx].dwBufferLength := NSize;
    End
  Else
    Result := FALSE;

  { On a filled buffer, increment it }
  If (Result) Then FilledBuffers := FilledBuffers + 1;
  QueuedBuffers := FilledBuffers - ProcessedBuffers;
END;

{--------------------StopAtOnce-------------John Mertus---14-June--97--}

   Procedure TAudioOut.StopAtOnce;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
BEGIN
  { if the device isn't open, just return }
   If (Not WaveDeviceOpen) Then Exit;

   Active := False;
   ContinueProcessing := FALSE;

  { stop playing }
   waveOutReset(WaveHandle);

  { close the device and unprepare the headers }
   CloseWaveDevice;
END;

{--------------------StopGracefully---------John Mertus---14-June--97--}

   Procedure TAudioOut.StopGracefully;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
BEGIN
  { if the device isn't open, just return }
  If (Not WaveDeviceOpen) Then Exit;
  ContinueProcessing := FALSE;
END;

{------------------BufferDone---------------John Mertus---14-June--97--}

   Procedure TCallBackWinOut.BufferDone(Var Msg : TMessage);

{ This is called when a buffer si done playing                          }
{									}
{**********************************************************************}
BEGIN
  With AudioComponent^ Do
    Begin
      ProcessedBuffers := ProcessedBuffers + 1;
      QueuedBuffers := FilledBuffers - ProcessedBuffers;
      Active := (QueuedBuffers > 0);
      If (ReadBuffer(BufIndex, FBufferSize)) Then QueueBuffer;

      If (Not Active) then
        Begin
          ContinueProcessing := FALSE;
          CloseWaveDevice;
        End;

    End;
END;

{------------------WaveOpen-----------------John Mertus---14-June--97--}

   Procedure TCallBackWinOut.WaveOpen(Var Msg : TMessage);

{ This is called at the termination of each buffer.                     }
{									}
{**********************************************************************}
BEGIN
  If Assigned(AudioComponent.FonOpen) Then AudioComponent.FonOpen(Self);
END;


{------------------WaveClose----------------John Mertus---14-June--97--}

   Procedure TCallBackWinOut.WaveClose(Var Msg : TMessage);

{ This is called at the termination of each buffer.                     }
{									}
{**********************************************************************}
BEGIN
  If Assigned(AudioComponent.FonClose) Then AudioComponent.FonClose(Self);
END;

{-----------------ElapsedTime----------------John Mertus---14-June--97--}

   Function TAudioIn.ElapsedTime : Real;

{ This function returns the time since start of playout.                }
{									}
{**********************************************************************}
Var
  pmmt  : TMMTime;

BEGIN
  If (Active) Then
    Begin
      pmmt.wType := TIME_SAMPLES;
      If (waveInGetPosition(WaveHandle, @pmmt, Sizeof(TMMTime)) <> 0) Then
         Result := 0
      Else
         Result := pmmt.sample/FrameRate;
    End
  Else
    Result := 0;
END;

{-------------CloseWaveDevice----------------John Mertus---14-June--97--}

   Procedure TAudioIn.CloseWaveDevice;

{ Closes the wave output device.                                        }
{									}
{**********************************************************************}
Var
  i : Integer;

BEGIN
{ unprepare the headers }
  Active := FALSE;
  For i := 0 to FNumBuffers-1 Do
    waveInUnprepareHeader( WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));

{ close the device }
   waveInReset(WaveHandle);
   waveInClose(WaveHandle);
   WaveDeviceOpen := FALSE;

END;

{-------------SetupOutput--------------------John Mertus---14-June--97--}

   Function TAudioIn.Setup(Var TS : TAudioIn) : Boolean;

{ This function just sets up the board for output.                      }
{									}
{**********************************************************************}
Var
  iErr        : Integer;
  i : Integer;

BEGIN

  { if the device is still open, return error }
  If (WaveDeviceOpen) Then
    Begin
      ErrorMessage := 'Wave Input device is already open';
      Result := FALSE;
      Exit;
    End;

  BufIndex := 0;

  { Now create the window component to handle the processing }
  CallBackWin := TCallBackWinIn.CreateParented(TWinControl(Owner).Handle);
  CallBackWin.Visible := FALSE;
  CallBackWin.AudioComponent := @TS;

  { Open the device for playout }
  { Either go via interrupt or window }
  iErr := WaveInOpen(@WaveHandle, FWaveDevice, @FWaveFmtEx, Integer(CallBackWin.Handle),
                      0, CALLBACK_WINDOW or WAVE_ALLOWSYNC );

  If (iErr <> 0) Then
    Begin
      ErrorMessage := TWaveInGetErrorText(iErr);
      Result := FALSE;
      Exit;
    End;

  WaveDeviceOpen := TRUE;

  { Setup the buffers and headers }
  If (Not InitWaveHeaders) Then
    Begin
      Result := FALSE;
      Exit;
    End;

  { Now Prepare the buffers for output }
  For i := 0 to FNumBuffers-1 Do
    Begin
      iErr := WaveInPrepareHeader(WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));
      If (iErr <> 0) Then
        Begin
          ErrorMessage := TWaveInGetErrorText(iErr);
          CloseWaveDevice;
          Result := FALSE;
          Exit;
        End;
    End;

  { Read in the buffers }
  QueuedBuffers := 0;
  ProcessedBuffers := 0;
  FilledBuffers := 0;
  ContinueProcessing := TRUE;
  Active := TRUE;

  Result := TRUE;
END;

{----------------QueueBuffer----------------John Mertus---14-June--97--}

   Function TAudioIn.QueueBuffer : Boolean;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
Var
  iErr : Integer;

BEGIN
 { reset flags field (remove WHDR_DONE attribute) }
  WaveHdr[bufindex].dwFlags := WHDR_PREPARED;

 { now queue the buffer for output }
  iErr := waveInAddBuffer( WaveHandle, WaveHdr[bufindex], sizeof(TWAVEHDR));
  If (iErr <> 0) Then
    Begin
      ErrorMessage := TWaveInGetErrorText(iErr);
      StopGracefully;
      Result := FALSE;
      Exit;
    End;

  { Advance index }
  bufindex := (bufindex+1) mod FNumBuffers;
  QueuedBuffers := QueuedBuffers + 1;

  Result := TRUE;
END;

{-------------StartIt------------------------John Mertus---14-June--97--}

   Function TAudioIn.StartIt : Boolean;

{ This function just starts the waveform playing                        }
{									}
{**********************************************************************}
Var
  i, iErr : Integer;

BEGIN
  { start recording to first buffer }
  iErr := WaveInStart(WaveHandle);
  If (iErr <> 0) Then
    begin
      CloseWaveDevice;
      ErrorMessage := 'Error starting wave record: ' + TWaveInGetErrorText(iErr);
      Result := FALSE;
      Exit;
    end;

  Active := TRUE;

  { Now we are ready to start the output }
  For i := 0 to FNumBuffers - 1 Do
   If (Not QueueBuffer) Then
    Begin
      CloseWaveDevice;
      Result := FALSE;
      Exit;
    End;

  Result := TRUE;
END;

{-----------------Start----------------------John Mertus---14-June--97--}

   Function TAudioIn.Start(Var TS : TAudioIn) : Boolean;

{ This function first sets up the output and then starts it.            }
{									}
{**********************************************************************}
BEGIN
  Result := Setup(TS);
  If (Not Result) Then Exit;

  Result := StartIt;
  If (Not Result) Then Exit;
END;


{-----------ProcessBuffer---------------------John Mertus---14-June--97--}

   Function  TAudioIn.ProcessBuffer(B : lpstr; N : Integer) : Boolean;

{ This is called whenver move buffer data is needed.                    }
{									}
{**********************************************************************}

BEGIN
  { Do not call the read buffer routine if we want to stop }
  If (Not ContinueProcessing) Then
     Begin
        Result := FALSE;
        Exit;
     End;

  { N can change, but we dont' care }
  If Assigned(FOnBufferFilled) Then
    Begin
      FOnBufferFilled(Self, B, N, Result);
    End
  Else
    Result := TRUE;

  { On a filled buffer, increment it }
  If (Result) Then FilledBuffers := FilledBuffers + 1;
END;

{--------------------StopAtOnce-------------John Mertus---14-June--97--}

   Procedure TAudioIn.StopAtOnce;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
BEGIN
  { if the device isn't open, just return }
   If (Not WaveDeviceOpen) Then Exit;

   Active := False;
   ContinueProcessing := FALSE;

  { stop playing }
   waveInReset(WaveHandle);

  { close the device and unprepare the headers }
   CloseWaveDevice;
END;

{--------------------StopGracefully---------John Mertus---14-June--97--}

   Procedure TAudioIn.StopGracefully;

{ Write the buffer to the wave device and toggel buffer index.          }
{									}
{**********************************************************************}
BEGIN
  { if the device isn't open, just return }
  If (Not WaveDeviceOpen) Then Exit;
  ContinueProcessing := FALSE;
END;

{------------------BufferFinished-----------John Mertus---14-June--97--}

   Procedure TCallBackWinIn.BufferFinished(Var Msg : TMessage);

{ This is called when each buffer is filled.                            }
{									}
{**********************************************************************}
BEGIN
  With AudioComponent^ Do
    Begin
      ProcessedBuffers := ProcessedBuffers + 1;
      QueuedBuffers := QueuedBuffers - 1;
      Active := (QueuedBuffers > 0);
      If (ProcessBuffer(WaveBuffer[BufIndex], FBufferSize)) Then QueueBuffer;

      If (Not Active) then
        Begin
          ContinueProcessing := FALSE;
          CloseWaveDevice;
        End;

    End;
END;

{------------------WaveOpenIn---------------John Mertus---14-June--97--}

   Procedure TCallBackWinIn.WaveOpenIn(Var Msg : TMessage);

{ This is called at the termination of each buffer.                     }
{									}
{**********************************************************************}
BEGIN
  If Assigned(AudioComponent.FonOpen) Then AudioComponent.FonOpen(Self);
END;


{------------------WaveCloseIn----------------John Mertus---14-June--97--}

   Procedure TCallBackWinIn.WaveCloseIn(Var Msg : TMessage);

{ This is called at the termination of each buffer.                     }
{									}
{**********************************************************************}
BEGIN
  If Assigned(AudioComponent.FonClose) Then AudioComponent.FonClose(Self);
END;


End.
