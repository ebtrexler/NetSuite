{***************************************************************}
{                                                               }
{ TParser 10.2 for Borland Delphi                               }
{                                                               }
{ A component for parsing and evaluating mathematical           }
{ expressions specified at runtime                              }
{                                                               }
{ Developed by                                                  }
{   Renate Schaaf (schaaf@math.usu.edu), 1993                   }
{   Alin Flaider (aflaidar@datalog.ro), 1996                    }
{   Stefan Hoffmeister (Stefan.Hoffmeister@Uni-Passau.de), 1997 }
{   Hallvard Vassbotn (hallvard.vassbotn@c2i.net),              }
{     Added dynamic generation of code (DYNAMIC_CODE), 1999     }
{     Simplified a little for TDM article, 2000                 }
{                                                               }
{ See  PARSER10.TXT  for documentation                          }
{                                                               }
{***************************************************************}
unit Parser10;

{$IFDEF Win32}
  {$H+} { long strings }
{$ENDIF}

{$I+} { I/O checking ON }

interface

uses SysUtils, Classes;

type
  PParserFloat = ^ParserFloat;
  ParserFloat  = Double;
  TToken = (variab, constant, minus, sum, diff, prod, divis, modulo, IntDiv,
    integerpower, realpower, square, third, fourth, FuncOneVar, FuncTwoVar);

  POperation = ^TOperation;
  TMathProcedure = procedure(AnOperation: POperation);
  TOperation = record
    Arg1: PParserFloat;
    Arg2: PParserFloat;
    Dest: PParserFloat;
    NextOperation: POperation;
    MathProc: TMathProcedure;
    Token: TToken;
  end;

  EMathParserError      = class(Exception);
    ESyntaxError          = class(EMathParserError);
    EExpressionHasBlanks  = class(EMathParserError);
    EExpressionTooComplex = class(EMathParserError);
    ETooManyNestings      = class(EMathParserError);
    EMissMatchingBracket  = class(EMathParserError);
    EBadName              = class(EMathParserError);
    EParserInternalError  = class(EMathParserError);

  TParserExceptionEvent = procedure (Sender: TObject; E: Exception) of object;

  TCustomParser = class(TComponent)
  private
    FExpression         : string;
    FPascalNumberformat : boolean;
    FParserError        : boolean;
    FVariables          : TStringList;
    FStartOperationList : POperation;
    FOnParserError      : TParserExceptionEvent;
    FFunctionOne : TStringList;
    FFunctionTwo : TStringList;
    function GetValue: Extended; virtual;
    procedure SetExpression(const AnExpression: string);
    procedure SetVar(const VarName: string; const Value: Extended);
    procedure SetValue(const Value: Extended);
    function ParseExpression: boolean;
    procedure FreeExpression; virtual;
    function GetVariable(const VarName: string): Extended;
    procedure DisposeVariableFloatAt(Index: integer);
    function ConnectMandatoryOperations: integer;
    procedure GenerateDynamicCode(OperationCount: integer); virtual;
  protected
    procedure DefineBasicFunctions;
    property FunctionOne : TStringList read FFunctionOne;
    property FunctionTwo : TStringList read FFunctionTwo;
    property LinkedOperationList: POperation read FStartOperationList;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    { Function support }
    procedure AddFunctionOneParam(const AFunctionName: string; const Func: TMathProcedure);
    procedure AddFunctionTwoParam(const AFunctionName: string; const Func: TMathProcedure);
    procedure ClearFunctions;
    procedure ClearFunction(const AFunctionName: string);
    { Variable support }
    procedure ClearVariables;
    procedure ClearVariable(const AVarName: string);
    function  VariableExists(const AVarName: string): boolean;
    function SetVariable(VarName: string; const Value: Extended): PParserFloat;
    property Variable[const VarName: string]: Extended read GetVariable write SetVar;
    { Error handling }
    property ParserError: boolean read FParserError;
  published
    { To evaluate an expression simply read the Value property. }
    property Value: Extended read GetValue write SetValue stored False;
    property Expression: string read FExpression write SetExpression;
    property PascalNumberformat: boolean read FPascalNumberformat write FPascalNumberformat default True;
    property OnParserError: TParserExceptionEvent read FOnParserError write FOnParserError;
  end;

  TFParser = class(TCustomParser)
  private
    { some pre-allocated space for variables }
    FA: ParserFloat;
    FB: ParserFloat;
    FC: ParserFloat;
    FD: ParserFloat;
    FE: ParserFloat;
    FX: ParserFloat;
    FY: ParserFloat;
    FT: ParserFloat;
  public
    constructor Create(AOwner: TComponent); override;
  published
    { predefined variable properties }
    property A: ParserFloat read FA write FA;
    property B: ParserFloat read FB write FB;
    property C: ParserFloat read FC write FC;
    property D: ParserFloat read FD write FD;
    property E: ParserFloat read FE write FE;
    property T: ParserFloat read FT write FT;
    property X: ParserFloat read FX write FX;
    property Y: ParserFloat read FY write FY;
 end;

 { Compile assembly code dynamically,
 	gives a major performance boost on PII and later
   -- modified by ebt from Hallvard Vassbotn
   9/2004   }
  TFParserASM = class(TCustomParser)
  private
  { some pre-allocated space for variables }
    FA: ParserFloat;
    FB: ParserFloat;
    FC: ParserFloat;
    FD: ParserFloat;
    FE: ParserFloat;
    FX: ParserFloat;
    FY: ParserFloat;
    FT: ParserFloat;
  { variables and methods introduced or overriden for ASM}
    DynamicCode         : Pointer;
    function GetValue: Extended; override;
    procedure FreeExpression; override;
    procedure GenerateDynamicCode(OperationCount: integer); override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    { predefined variable properties }
    property A: ParserFloat read FA write FA;
    property B: ParserFloat read FB write FB;
    property C: ParserFloat read FC write FC;
    property D: ParserFloat read FD write FD;
    property E: ParserFloat read FE write FE;
    property T: ParserFloat read FT write FT;
    property X: ParserFloat read FX write FX;
    property Y: ParserFloat read FY write FY;
 end;

procedure Register;

implementation

{$DEFINE UseMath}
{ Note: if you do not have the MATH unit simply remove the conditional define
        the component will continue to work, just a bit slower }

uses
{$IFDEF UseMath}
  Math,
{$ENDIF}
  P10Build;

(*
{$IFDEF VER80}
  {$R *.D16}
{$ELSE}
  {$IFDEF VER90}
    {$R *.D32}
  {$ENDIF}
{$ENDIF}
*)

procedure Register;
begin
  RegisterComponents('Gotham Scientific', [TFParser]);
  RegisterComponents('Gotham Scientific', [TFParserASM]);
end;

function PointerToSelfInstance(Self: TObject; Ptr: Pointer): boolean;
{ Return True of the Pointer points inside the object instance }
begin
  Result := Assigned(Self) and
            (Longint(Ptr) >= Longint(Self)) and
            (Longint(Ptr) <= Longint(Self) + Self.InstanceSize);
end;

{****************************************************************}
{                                                                }
{   Following are "built-in" calculation procedures              }
{                                                                }
{****************************************************************}
{
Naming convention for functions:

  Name of built-in function, prepended with an underscore.
  Example:

    ln --> _ln

Passed arguments / results:

  If the function takes any arguments - i.e. if it has been added to
  either the FunctionOne or the FunctionTwo list:

  - First  argument --> arg1^
  - Second argument --> arg2^

  The result of the operation must ALWAYS be put into

     dest^


 Note: These are POINTERS to floats.
}

{****************************************************************}
{                                                                }
{   These are mandatory procedures - never remove them           }
{                                                                }
{****************************************************************}

procedure _nothing(AnOp: POperation); far;
{ do nothing - this only happens if the "term" is just a number
  or a variable; otherwise this procedure will never be called }
begin
end;

procedure _Add(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := arg1^ + arg2^;
end;

procedure _Subtract(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := arg1^ - arg2^;
end;

procedure _Multiply(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := arg1^ * arg2^;
end;

procedure _RealDivide(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := arg1^ / arg2^;
end;

procedure _Modulo(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := trunc(arg1^) mod trunc(arg2^);
end;

procedure _IntDiv(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := trunc(arg1^) div trunc(arg2^);
end;

procedure _Negate(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := -arg1^;
end;

procedure _IntPower(AnOp: POperation); far;
{$IFNDEF UseMath}
var
  n, i: longint;
{$ENDIF}
begin
{$IFNDEF UseMath}
  with AnOp^ do
  begin
    n := trunc(abs(arg2^))-1;

    case n of
      -1: dest^ := 1;
       0: dest^ := arg1^;
    else
      dest^ := arg1^;
      for i := 1 to n do
        dest^ := dest^ * arg1^;
    end;

    if arg2^ < 0 then
      dest^ := 1 / dest^;

  end;
{$ELSE}
  with AnOp^ do
    dest^ := IntPower(arg1^, trunc(arg2^));
{$ENDIF}
end;

procedure _square(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := sqr(arg1^);
end;

procedure _third(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := arg1^ * arg1^ * arg1^;
end;

procedure _forth(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := sqr(sqr(arg1^));
end;

procedure _power(AnOp: POperation); far;
begin
  with AnOp^ do
  begin
{$IFNDEF UseMath}
    if arg1^ = 0 then
      dest^ := 0
    else
      dest^ := exp(arg2^*ln(arg1^));
{$ELSE}
    dest^ := Power(arg1^, arg2^);
{$ENDIF}
  end;
end;

{****************************************************************}
{                                                                }
{   These are OPTIONAL procedures - you may remove them, though  }
{   it is preferable to not register them for use                }
{                                                                }
{****************************************************************}

procedure _sin(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := sin(arg1^);
end;

procedure _cos(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := cos(arg1^);
end;

procedure _arctan(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := arctan(arg1^);
end;

procedure _arg(AnOp: POperation); far;
begin
  with AnOp^ do
    if arg1^ < 0 then
      dest^ := arctan(arg2^/arg1^)+Pi
    else
      if arg1^>0 then
        dest^ := arctan(arg2^/arg1^)
      else
        if arg2^ > 0 then
          dest^ := 0.5 * Pi
        else
          dest^ := -0.5 * Pi;
end;

procedure _sinh(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := (exp(arg1^)-exp(-arg1^))*0.5;
end;

procedure _cosh(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := (exp(arg1^)+exp(-arg1^))*0.5;
end;

procedure _cotan(AnOp: POperation); far;
begin
  with AnOp^ do
  {$IFNDEF UseMath}
    dest^ := cos(arg1^) / sin(arg1^);
  {$ELSE}
    dest^ := cotan(arg1^);
  {$ENDIF}
end;

procedure _tan(AnOp: POperation); far;
begin
  with AnOp^ do
  {$IFNDEF UseMath}
    dest^ := sin(arg1^) / cos(arg1^);
  {$ELSE}
    dest^ := tan(arg1^);
  {$ENDIF}
end;

procedure _exp(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := exp(arg1^);
end;

procedure _ln(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := ln(arg1^);
end;

procedure _log10(AnOp: POperation); far;
const
  _1_ln10 =  0.4342944819033;
begin
  with AnOp^ do
{$IFDEF UseMath}
    dest^ := log10(arg1^);
{$ELSE}
    dest^ := ln(arg1^) * _1_ln10;
{$ENDIF}
end;

procedure _log2(AnOp: POperation); far;
const
  _1_ln2 = 1.4426950409;
begin
  with AnOp^ do
{$IFDEF UseMath}
    dest^ := log2(arg1^);
{$ELSE}
    dest^ := ln(arg1^) * _1_ln2;
{$ENDIF}
end;

procedure _logN(AnOp: POperation); far;
begin
  with AnOp^ do
{$IFDEF UseMath}
    dest^ := logN(arg1^, arg2^);
{$ELSE}
    dest^ := ln(arg1^) / ln(arg2^);
{$ENDIF}
end;

procedure _sqrt(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := sqrt(arg1^);
end;


procedure _abs(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := abs(arg1^);
end;

procedure _min(AnOp: POperation); far;
begin
  with AnOp^ do
    if arg1^ < arg2^ then
      dest^ := arg1^
    else
      dest^ := arg2^;
end;

procedure _max(AnOp: POperation); far;
begin
  with AnOp^ do
    if arg1^ < arg2^ then
      dest^ := arg2^
    else
      dest^ := arg1^;
end;

procedure _heaviside(AnOp: POperation); far;
begin
  with AnOp^ do
    if arg1^ < 0 then
      dest^ := 0
    else
      dest^ := 1;
end;

procedure _sign(AnOp: POperation); far;
begin
  with AnOp^ do
    if arg1^ < 0 then
      dest^ := -1
    else
      if arg1^ > 0 then
        dest^ := 1.0
      else
        dest^ := 0.0;
end;

procedure _zero(AnOp: POperation); far;
begin
  with AnOp^ do
    if arg1^ = 0.0 then
      dest^ := 0.0
    else
      dest^ := 1.0;
end;

procedure _trunc(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := int(arg1^)
end;

procedure _ceil(AnOp: POperation); far;
begin
  with AnOp^ do
    if frac(arg1^) > 0 then
      dest^ := int(arg1^ + 1)
    else
      dest^ := int(arg1^);
end;

procedure _floor(AnOp: POperation); far;
begin
  with AnOp^ do
    if frac(arg1^) < 0 then
      dest^ := int(arg1^ - 1)
    else
      dest^ := int(arg1^);
end;

procedure _rnd(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := Random * int(arg1^);
end;

procedure _random(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := Random;
end;

procedure _radius(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := sqrt(sqr(arg1^)+sqr(arg2^));
end;

procedure _phase(AnOp: POperation); far;
var
  a: ParserFloat;
begin
  with AnOp^ do
  begin
    a := arg1^ / (2/pi);
    dest^ := (2*pi) * (a-round(a));
  end;
end;

procedure _round(AnOp: POperation); far;
begin
  with AnOp^ do
    dest^ := round(arg1^);
end;

{****************************************************************}
{                                                                }
{   TCustomParser                                                }
{                                                                }
{    A base class which does not publish the variable properties }
{    and adds no functions by default                            }
{                                                                }
{****************************************************************}

function TCustomParser.ParseExpression: boolean;
begin
  try
    P10Build.ParseFunction( FExpression, FVariables, FunctionOne, FunctionTwo,
      FPascalNumberformat, FStartOperationList, FParserError);
    Result := True;
  except
    on E: EMathParserError do
    begin
      FParserError := True;

      if Assigned(FOnParserError) then
      begin
        FOnParserError(Self, E);
        Result := False;
      end
      else
        raise;
    end;
  end;
end;

procedure TCustomParser.GenerateDynamicCode(OperationCount: integer);
begin
end;

function TCustomParser.ConnectMandatoryOperations: integer;
const
  MandatoryOperationMap: array[TToken] of TMathProcedure
    = (_nothing, _nothing, _negate, _add, _subtract, _multiply, _RealDivide,
       _Modulo, _IntDiv, _IntPower, _Power, _square, _third, _forth, nil, nil);
var
  O: POperation;
begin
  Result := 0;
  O := FStartOperationList;
  while O <> nil do
  begin
    if O^.Token in [variab..fourth] then
      O^.MathProc := MandatoryOperationMap[O^.Token];
    O := O^.NextOperation;
    Inc(Result);
  end;
end;

procedure TCustomParser.SetExpression(const AnExpression: string);
var
  OperationCount: integer;
begin
  FreeExpression;
  FExpression := AnExpression;
  if FExpression <> '' then
    if ParseExpression then
    begin
      OperationCount := ConnectMandatoryOperations;
      GenerateDynamicCode(OperationCount);
    end;
end;

constructor TCustomParser.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FPascalNumberformat := True;

  FVariables := TStringList.Create;
  FVariables.Sorted := True;
  FVariables.Duplicates := dupIgnore;

  FFunctionOne := TStringList.Create;
  FunctionOne.Sorted := True;
  FunctionOne.Duplicates := dupError;

  FFunctionTwo := TStringList.Create;
  FunctionTwo.Sorted := True;
  FunctionTwo.Duplicates := dupError;
end;

destructor TCustomParser.Destroy;
begin
  FreeExpression;

  ClearVariables;
  FVariables.Free;

  FunctionOne.Free;
  FunctionTwo.Free;

  inherited Destroy;
end;

procedure TCustomParser.SetVar(const VarName: string; const Value: Extended);
begin
  SetVariable(VarName, Value);
end;

function TCustomParser.SetVariable(VarName: string; const Value: Extended): PParserFloat;
var
  i: integer;
begin
  { TString.Find is not case-sensitive }
  if FVariables.Find(VarName, i) then
  begin
    Result := PParserFloat(FVariables.Objects[i]);
    Result^ := Value;
  end
  else
  begin
    { is the variable name a valid identifier? }
    if not IsValidIdent(VarName) then
      raise EBadName.Create(VarName);

    { Convert to uppercase }
    VarName := UpperCase(VarName);

    { check whether the variable contains any of the operators (DIV and MOD)
      this would confuse the parser... }
    if pos('DIV', VarName) <> 0 then
        raise EBadName.Create(VarName);

    if pos('MOD', VarName) <> 0 then
        raise EBadName.Create(VarName);

    New(Result);
    Result^ := Value;

    FVariables.AddObject(VarName, TObject(Result));
  end;
end;

function TCustomParser.GetVariable(const VarName: string): Extended;
var
  i: integer;
begin
  if FVariables.Find(VarName, i) then
    Result := PParserFloat(FVariables.Objects[i])^
  else
    Result := 0.0;
end;

procedure TCustomParser.AddFunctionOneParam(const AFunctionName: string; const Func: TMathProcedure);
begin
  if IsValidIdent(AFunctionName) then
    FunctionOne.AddObject(UpperCase(AFunctionName), TObject(@Func))
  else
    raise EBadName.Create(AFunctionName);
end;

procedure TCustomParser.AddFunctionTwoParam(const AFunctionName: string; const Func: TMathProcedure);
begin
  if IsValidIdent(AFunctionName) then
    FunctionTwo.AddObject(UpperCase(AFunctionName), TObject(@Func))
  else
    raise EBadName.Create(AFunctionName);
end;

procedure TCustomParser.DisposeVariableFloatAt(Index: integer);
var
  APPFloat: PParserFloat;
begin
  APPFloat := PParserFloat(FVariables.Objects[Index]);
  { Dispose only user-defined variables }
  if not PointerToSelfInstance(Self, APPFloat) then
    Dispose(APPFloat);
end;

procedure TCustomParser.ClearVariables;
var
  i: integer;
begin
  for i := 0 to FVariables.Count-1 do
    DisposeVariableFloatAt(i);
  FVariables.Clear;
  SetExpression(''); { invalidate expression }
end;

procedure TCustomParser.ClearVariable(const AVarName: string);
var
  Index: integer;
begin
  if FVariables.Find(AVarName, Index) then
  begin
    DisposeVariableFloatAt(Index);
    FVariables.Delete(Index);
  end;
  SetExpression(''); { invalidate expression }
end;

function TCustomParser.VariableExists(const AVarName: string): boolean;
var
  Index: integer;
begin
  Result := FVariables.Find(AVarName, Index);
end;

procedure TCustomParser.ClearFunctions;
begin
  FunctionOne.Clear;
  FunctionTwo.Clear;
  SetExpression(''); { invalidate expression }
end;

procedure TCustomParser.ClearFunction(const AFunctionName: string);
var
  Index: integer;
begin
  if FunctionOne.Find(AFunctionName, Index) then
  begin
    FunctionOne.Delete(Index);
    SetExpression(''); { invalidate expression }
    Exit;
  end;

  if FunctionTwo.Find(AFunctionName, Index) then
  begin
    FunctionTwo.Delete(Index);
    SetExpression(''); { invalidate expression }
  end;
end;

procedure TCustomParser.FreeExpression;
var
  LastOP,
  NextOP: POperation;
begin
  LastOP := FStartOperationList;

  while LastOP <> nil do
  begin
    NextOP := LastOP^.NextOperation;

    while NextOP <> nil do
      with NextOP^ do
      begin
        if (Arg1 = lastop^.Arg1) or (Arg1 = lastop^.Arg2) or (Arg1 = lastop^.Dest) then
          Arg1 := nil;

        if (Arg2 = lastop^.Arg1) or (Arg2 = lastop^.Arg2) or (Arg2 = lastop^.Dest) then
          Arg2 := nil;

        if (Dest = lastop^.Arg1) or (Dest = lastop^.Arg2) or (Dest = lastop^.Dest) then
          Dest := nil;

        NextOP := NextOperation;
      end;

    with LastOP^ do
    begin
      if FVariables.IndexOfObject( TObject(Arg1)) >= 0 then Arg1 := nil;
      if FVariables.IndexOfObject( TObject(Arg2)) >= 0 then Arg2 := nil;
      if FVariables.IndexOfObject( TObject(Dest)) >= 0 then Dest := nil;

      if (Dest <> nil) and (Dest <> Arg2) and (Dest <> Arg1) then
        dispose(Dest);

      if (Arg2 <> nil) and (Arg2 <> Arg1) then
        dispose(Arg2);

      if (Arg1 <> nil) then
        dispose(Arg1);
    end;

    NextOP := LastOP^.NextOperation;
    dispose(LastOP);
    LastOP := NextOP;
  end;

  FStartOperationList := nil;
end;

function TCustomParser.GetValue: Extended;
var
  LastOP: POperation;
begin
  if FStartOperationList <> nil then
  begin
    LastOP := FStartOperationList;
    while LastOP^.NextOperation <> nil do
    begin
      with LastOP^ do
      begin
        MathProc(LastOP);
        LastOP := NextOperation;
      end;
    end;
    LastOP^.MathProc(LastOP);
    Result := LastOP^.Dest^;
  end
  else
    Result := 0;
end;

procedure TCustomParser.SetValue(const Value: Extended);
begin
  { Dummy set routine - to fool the Object Inspector }
end;

procedure TCustomParser.DefineBasicFunctions;
begin
  with FunctionOne do
  begin
    Capacity := 32;
    AddObject('TAN'   , TObject(@_tan));
    AddObject('SIN'   , TObject(@_sin));
    AddObject('COS'   , TObject(@_cos));
    AddObject('SINH'  , TObject(@_sinh));
    AddObject('COSH'  , TObject(@_cosh));
    AddObject('ARCTAN', TObject(@_arctan));
    AddObject('COTAN' , TObject(@_cotan));
    AddObject('ARG'   , TObject(@_arg));
    AddObject('EXP'   , TObject(@_exp));
    AddObject('LN'    , TObject(@_ln));
    AddObject('LOG10' , TObject(@_log10));
    AddObject('LOG2'  , TObject(@_log2));
    AddObject('SQR'   , TObject(@_square));
    AddObject('SQRT'  , TObject(@_sqrt));
    AddObject('ABS'   , TObject(@_abs));
    AddObject('TRUNC' , TObject(@_trunc));
    AddObject('INT'   , TObject(@_trunc)); { NOTE: INT = TRUNC ! }
    AddObject('CEIL'  , TObject(@_ceil));
    AddObject('FLOOR' , TObject(@_floor));
    AddObject('HEAV'  , TObject(@_heaviside));
    AddObject('SIGN'  , TObject(@_sign));
    AddObject('ZERO'  , TObject(@_zero));
    AddObject('PH'    , TObject(@_phase));
    AddObject('RND'   , TObject(@_rnd));
    AddObject('RANDOM', TObject(@_random));
    AddObject('ROUND' , TObject(@_round));  {//added by ebt}
  end;
  with FunctionTwo do
  begin
    Capacity := 8;
    AddObject('MAX'     , TObject(@_max));
    AddObject('MIN'     , TObject(@_min));
    AddObject('POWER'   , TObject(@_Power));
    AddObject('INTPOWER', TObject(@_IntPower));
    AddObject('LOGN'    , TObject(@_logN));
  end;
end;

{****************************************************************}
{                                                                }
{   TCustomParser                                                }
{                                                                }
{****************************************************************}
constructor TFParser.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  DefineBasicFunctions;
  with FVariables do
  begin
    Capacity := 16;
    AddObject( 'A', TObject(@FA));
    AddObject( 'B', TObject(@FB));
    AddObject( 'C', TObject(@FC));
    AddObject( 'D', TObject(@FD));
    AddObject( 'E', TObject(@FE));
    AddObject( 'X', TObject(@FX));
    AddObject( 'Y', TObject(@FY));
    AddObject( 'T', TObject(@FT));
  end;
end;

constructor TFParserASM.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  DefineBasicFunctions;
  with FVariables do
  begin
    Capacity := 16;
    AddObject( 'A', TObject(@FA));
    AddObject( 'B', TObject(@FB));
    AddObject( 'C', TObject(@FC));
    AddObject( 'D', TObject(@FD));
    AddObject( 'E', TObject(@FE));
    AddObject( 'X', TObject(@FX));
    AddObject( 'Y', TObject(@FY));
    AddObject( 'T', TObject(@FT));
  end;
end;

procedure TFParserASM.GenerateDynamicCode(OperationCount: integer);
type
  PCallOperation = ^TCallOperation;
  TCallOperation = packed record
    MOV_EAX    : Byte;
    LastOpAddr : POperation;
    CALL       : Byte;
    OFFSET     : Longint;
  end;
  PReturnLastOp = ^TReturnLastOp;
  TReturnLastOp = packed record
    MOV_EAX    : Byte;
    LastOpAddr : POperation;
    RET        : Byte;
  end;
const
  CallInstruction   = $E8;
  RetInstruction    = $C3;
  MovEAXInstruction = $B8;
var
  ThisCallOperation : PCallOperation;
  ReturnLastOp      : PReturnLastOp;
  Operation: POperation;
begin
  { Now generate some code dynamically on the heap to call the operations }
  if OperationCount > 0 then
  begin
    { Allocate a memory block of the right size }
    GetMem(DynamicCode, (OperationCount * SizeOf(TCallOperation)) + SizeOf(TReturnLastOp));

    { Loop through the operations and build code as we go }
    ThisCallOperation := DynamicCode;
    Operation := FStartOperationList;
    while True do
    begin
      with ThisCallOperation^ do
      begin
        MOV_EAX    := MovEAXInstruction;
        LastOpAddr := Operation;
        CALL       := CallInstruction;
        OFFSET     := PChar(@Operation^.MathProc) - (PChar(@ThisCallOperation^.CALL) + 5);
      end;
      Inc(ThisCallOperation);
      if Operation^.NextOperation = nil then
        Break;
      Operation := Operation^.NextOperation;
    end;
    { Add code to return the last node }
    ReturnLastOp := PReturnLastOp(ThisCallOperation);
    with ReturnLastOp^ do
    begin
      MOV_EAX    := MovEAXInstruction;
      LastOpAddr := Operation;
      RET        := RetInstruction;
    end;
  end;
end;

procedure TFParserASM.FreeExpression;
begin
  inherited FreeExpression;
  if Assigned(DynamicCode) then
  begin
    FreeMem(DynamicCode);
    DynamicCode := nil;
  end;
end;

function TFParserASM.GetValue: Extended;
type
  TCallOperationFunc = function: POperation;
begin
  if Assigned(DynamicCode) then
    Result := TCallOperationFunc(DynamicCode)^.Dest^
  else
    Result := 0;
end;

end.
