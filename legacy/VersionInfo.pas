unit VersionInfo;
{ Defines a class TVersionInfo which returns:
  1: the Project Version Info strings,
  2: the Locale ID converted to a Language ID string,
  3: the Major, Minor, Release versions and Build number,
  4: the module attributes Debug Build, Pre-Release, Private Build and Special
     Build

  Remake of David Simpson (drs@ihug.com.au) code

  This unit is partly based on code originally supplied by Josef Garvi
  (josef@eden-foundation.org).  The key difference between his implementation
  and mine is that his returned values as procedural parameters while mine
  does as properties.}

interface

uses Classes, Windows, SysUtils;

type
  TModuleAttributes = set of (maDebugBuild, maPreRelease, maPatched,
    maPrivateBuild, maInfoInferred, maSpecialBuild);

  TVersionInfo = class(TObject)
  private
    FLocaleIndex: cardinal;
    FLangID: DWORD;
    FVerInfo: Pointer;
    FVerInfoSize: DWORD;
    FVerValue: PVSFixedFileInfo;
    function GetPropertyString(Index: integer): string;
    function GetVersionNumber(Index: integer): word;
    function GetModuleAttributes: TModuleAttributes;
    function GetLangString(LangID: DWORD): string;
    function GetLocaleInfoCount: cardinal;
    procedure SetLocalIndex(const Value: cardinal);
  public
    constructor Create(FileName: string);
    destructor Destroy; override;
    function GetVersionString(Value: string): string;
    function GetLocaleInfo(Index: cardinal): DWORD;
    property LocaleInfoCount: cardinal read GetLocaleInfoCount;
    property LocaleIndex: cardinal read FLocaleIndex write SetLocalIndex;
    property Language: string index 0 read GetPropertyString;
    property CompanyName: string index 1 read GetPropertyString;
    property FileDescription: string index 2 read GetPropertyString;
    property FileVersion: string index 3 read GetPropertyString;
    property InternalName: string index 4 read GetPropertyString;
    property LegalCopyright: string index 5 read GetPropertyString;
    property LegalTrademarks: string index 6 read GetPropertyString;
    property OriginalFileName: string index 7 read GetPropertyString;
    property ProductName: string index 8 read GetPropertyString;
    property ProductVersion: string index 9 read GetPropertyString;
    property Comments: string index 10 read GetPropertyString;
    property PrivateBuild: string index 11 read GetPropertyString;
    property SpecialBuild: string index 12 read GetPropertyString;
    property MajorVersion: word index 0 read GetVersionNumber;
    property MinorVersion: word index 1 read GetVersionNumber;
    property ReleaseVersion: word index 2 read GetVersionNumber;
    property Build: word index 3 read GetVersionNumber;
    property ModuleAttributes: TModuleAttributes read GetModuleAttributes;
  end;

implementation

constructor TVersionInfo.Create;
var
  Dummy: DWORD;
begin
  inherited Create;
  FVerInfoSize := GetFileVersionInfoSize(PChar(FileName), Dummy);
  if FVerInfoSize = 0 then
    RaiseLastOSError;
  GetMem(FVerInfo, FVerInfoSize);
  Win32Check(GetFileVersionInfo(PChar(FileName), 0, FVerInfoSize, FVerInfo));
  VerQueryValue(FVerInfo, '\', Pointer(FVerValue), Dummy);
  LocaleIndex:= 0;
end;

destructor TVersionInfo.Destroy;
begin
  FreeMem(FVerInfo, FVerInfoSize);
  inherited Destroy;
end;

function TVersionInfo.GetPropertyString(Index: integer): string;
{ Returns the Project Version Info strings plus the Locale ID converted to
  a Language ID string. }
begin
  case Index of
    0: Result := GetLangString(FLangID); // Language
    1: Result := GetVersionString('CompanyName');
    2: Result := GetVersionString('FileDescription');
    3: Result := GetVersionString('FileVersion');
    4: Result := GetVersionString('InternalName');
    5: Result := GetVersionString('LegalCopyright');
    6: Result := GetVersionString('LegalTrademarks');
    7: Result := GetVersionString('OriginalFilename');
    8: Result := GetVersionString('ProductName');
    9: Result := GetVersionString('ProductVersion');
    10: Result := GetVersionString('Comments');
    11: Result := GetVersionString('PrivateBuild');
    12: Result := GetVersionString('SpecialBuild');
  end;
end;

function TVersionInfo.GetVersionNumber(Index: integer): word;
{ Returns the Major, Minor, Release versions and Build number }
begin
  // dummy code to stop compiler warning
  Result := 0;
  // real code
  case Index of
    0: Result := HiWord(FVerValue^.dwFileVersionMS); // Major Version
    1: Result := LoWord(FVerValue^.dwFileVersionMS); // Minor Version
    2: Result := HiWord(FVerValue^.dwFileVersionLS); // Release Version
    3: Result := LoWord(FVerValue^.dwFileVersionLS); // Build
  end;
end;

function TVersionInfo.GetModuleAttributes: TModuleAttributes;
begin
  Result := [];
  if (FVerValue^.dwFileFlags and VS_FF_DEBUG) <> 0 then
    Result:= Result + [maDebugBuild];
  if (FVerValue^.dwFileFlags and VS_FF_PRERELEASE) <> 0 then
    Result:= Result + [maPreRelease];
  if (FVerValue^.dwFileFlags and VS_FF_PATCHED) <> 0 then
    Result:= Result + [maPatched];
  if (FVerValue^.dwFileFlags and VS_FF_PRIVATEBUILD) <> 0 then
    Result:= Result + [maPrivateBuild];
  if (FVerValue^.dwFileFlags and VS_FF_INFOINFERRED) <> 0 then
    Result:= Result + [maInfoInferred];
  if (FVerValue^.dwFileFlags and VS_FF_SPECIALBUILD) <> 0 then
    Result:= Result + [maSpecialBuild];
end;

type
  TDWORDArray = array[0..16300] of DWORD;
const
  sTransalion = '\VarFileInfo\Translation';

function TVersionInfo.GetLocaleInfo(Index: cardinal): DWORD;
{ Retrieves the language ID, needed to know the locale / language code needed
  to retrieve the specific strings. }
var
  wArr: ^TDWORDArray;
  FLen: DWORD;
begin
  Result := 0;
  if VerQueryValue(FVerInfo, sTransalion, pointer(wArr), FLen) then
    if FLen >= (Succ(Index) * SizeOf(DWORD)) then
      Result:= wArr^[Index];
end;

function TVersionInfo.GetVersionString(Value: string): string;
var
  ItemName: string;
  FVVers: Pointer;
  FLen: DWORD;
begin
  Result := '';
  ItemName:= Format('\StringFileInfo\%.4x%.4x\%s',
    [LoWord(FLangID), HiWord(FLangID), Value]);
  if VerQueryValue(FVerInfo, PChar(ItemName), FVVers, FLen) then
    if FLen > 1 then
      Result:= PChar(FVVers);
end;

function TVersionInfo.GetLangString(LangID: DWORD): string;
begin
  Result:= StringOfChar(#0, 100);
  SetLength(Result,
    VerLanguageName(LoWord(LangID), PChar(Result), Length(Result)));
end;

function TVersionInfo.GetLocaleInfoCount: cardinal;
var
  wArr: ^TDWORDArray;
  FLen: DWORD;
begin
  Result := 0;
  if VerQueryValue(FVerInfo, sTransalion, pointer(wArr), FLen) then
    Result:= FLen div 4;
end;

procedure TVersionInfo.SetLocalIndex(const Value: cardinal);
begin
  if Value < LocaleInfoCount then
  begin
    FLocaleIndex := Value;
    FLangID := GetLocaleInfo(Value);
  end;
end;

end.

