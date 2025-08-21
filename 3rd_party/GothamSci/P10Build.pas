unit P10Build;

{$I+} { I/O checking is always on }

{$DEFINE UseIntegerOP}

// NOTE: Removed old and buggy code for Dyna conditional define - HV

interface

uses
  Parser10,
  SysUtils, Classes;

procedure ParseFunction( FunctionString: string; { the unparsed string }
                         Variables: TStringlist; { list of variables }

                         { lists of available functions }
                         FunctionOne,               { functions with ONE argument, e.g. exp() }
                         FunctionTwo: TStringList;  { functions with TWO arguments, e.g. max(,) }

                         UsePascalNumbers: boolean; { true: -> Val; false: StrToFloat }

                         { return pointer to tree, number of performed operations and error state }
                         var FirstOP : POperation;

                         var Error : boolean);
                         { error actually is superfluous as we are now using exceptions }



implementation


{ helper functions }

var
  CharTable:array[#0..#255] of byte;

(*function RemoveBlanks(const s: string): string;
{ deletes all blanks in s }
var
  i : integer;
begin
  Result := s;

  i := pos(' ', Result);
  while i > 0 do
  begin
    delete(Result, i, 1);
    i := pos(' ', Result);
  end;
end;*)

function TryStrToFloat(const S: string; var Value: Double): boolean;
var
  ExtValue: Extended;
begin
  Result := TextToFloat(PChar(S), ExtValue, fvExtended);
  if Result then
    Value := ExtValue;
end;

function HackSetLength(var S: String; NewLen: Integer): integer;
type
  PInteger = ^Integer;
begin
  Result := Length(S);
  PInteger(Longint(S)-4)^ := NewLen;
end;

{ case INSENSITIVE }
procedure MakeCharTable;
var
  I: Integer;
begin
  for I := 0 to 255 do
  begin
    If (I > 64) and (I < 91) then
      CharTable[Char(I)]:= I + 32
    else
      CharTable[Char(I)]:= I;
  end;
end;  { MakeCharTable }

function IPos(Pat, Text: PChar):Integer;
var
  RunPat, RunText, PosPtr: PChar;
begin
  Result:= 0;
  RunPat:= Pat;
  RunText:= Text;
  while RunText^ <> #0 do
  begin
    if (CharTable[RunPat^] = CharTable[RunText^]) then
      begin
        PosPtr := RunText;
        while RunPat^ <> #0 do begin
          if (CharTable[RunPat^] <> CharTable[RunText^]) then break;
          inc(RunPat); inc(RunText);
        end;
        if RunPat^ = #0 then
          begin
            Result:= PosPtr - Text +1;
            break;
          end;
      end else inc(RunText);
      RunPat:= Pat;
  end;
end; {IPos }

function IPosE(Pat, Text: PChar; StartPos, MaxPos: LongInt): Integer;
var
  AChar: char;

  RunPat,
  RunText,
  PosPtr: PChar;
begin
  Result:= 0;
  RunPat:= Pat;

  RunText := Text + MaxPos;
  AChar := RunText^;
  RunText^ := #0;

  RunText := Text + StartPos -1;

  while RunText^ <> #0 do
  begin
    if (CharTable[RunPat^] = CharTable[RunText^]) then
    begin
      PosPtr := RunText;

      while RunPat^ <> #0 do
      begin
        if (CharTable[RunPat^] <> CharTable[RunText^]) then
          break;

        inc(RunPat); inc(RunText);
      end;

      if (RunPat^ = #0) then
      begin
        Result:= PosPtr - Text +1;
        break;
      end;

    end
    else
      inc(RunText);

    RunPat := Pat;
  end;

  RunText := Text + MaxPos;
  RunText^ := AChar;

end; {IPosE }

function FastPos(Sign: Char; ToScan: PChar): integer;
var
  Input: PChar;
begin
  Result := 0;
  Input := ToScan;
  while (ToScan^ <> #0) do
  begin
    if ToScan^ = Sign then
    begin
      Result := ToScan - Input;
      break;
    end;
    inc(ToScan);
  end;
end;

{$IFDEF VER100}
resourcestring
{$ELSE}
const
{$ENDIF}
  msgErrBlanks = 'Expression has blanks';
  msgMissingBrackets = 'Missing brackets in expression (%s)';
  msgParseError = 'Error parsing expression:';
  msgNestings = 'Expression contains too many nestings';
  msgTooComplex = 'Expression is too complex';
  msgInternalError = 'TParser internal error';

const
  TokenOperators = [ sum, diff, prod, divis, modulo, IntDiv,
                     integerpower, realpower];

type
  TermString = string;

procedure ParseFunction( FunctionString: string;
                         Variables: TStringList;

                         FunctionOne,
                         FunctionTwo: TStringList;

                         UsePascalNumbers: boolean;

                         var FirstOP: POperation;

                         var Error: boolean);


          function CheckNumberBrackets(const s: string): integer; forward;
          { checks whether number of ( = number of ) }

          function CheckNumber(const s: string; var FloatNumber: ParserFloat): boolean; forward;
          { checks whether s is a number }

          function CheckVariable(const s: string; var VariableID: integer): boolean; forward;
          { checks whether s is a variable string }

          function CheckTerm(var s1: string): boolean; forward;
          { checks whether s is a valid term }

          function CheckBracket(const s: string; var s1: string): boolean; forward;
          { checks whether s =(...(s1)...) and s1 is a valid term }



          function CheckNegate(const s: string; var s1: string): boolean; forward;
          {checks whether s denotes the negative value of a valid operation}



          function CheckAdd(var s: string; var s1, s2: string): boolean; forward;
          {checks whether + is the primary operation in s}

          function CheckSubtract(const s: string; var s1, s2: string): boolean; forward;
          {checks whether - is the primary operation in s}

          function CheckMultiply(const s: string; var s1, s2: string): boolean; forward;
          {checks whether * is the primary operation in s}

{$IFDEF UseIntegerOP}
          function CheckIntegerDiv(const s: string; var s1, s2: string): boolean; forward;
          {checks whether DIV is the primary TOperation in s}

          function CheckModulo(const s: string; var s1, s2: string): boolean; forward;
          {checks whether MOD is the primary TOperation in s}
{$ENDIF UseIntegerOP}

          function CheckRealDivision(const s: string; var s1, s2: string): boolean;  forward;
          {checks whether / is the primary operation in s}



          function CheckFuncTwoVar(var s: string; var s1, s2: string): boolean; forward;
          {checks whether s=f(s1,s2); s1,s2 being valid terms}

          function CheckFuncOneVar(var s: string; var s1: string): boolean; forward;
          {checks whether s denotes the evaluation of a function fsort(s1)}


          function CheckPower(const s: string; var s1, s2: string; var AToken: TToken): boolean; forward;


          function CheckNumberBrackets(const s: string): integer;
          {checks whether # of '(' equ. # of ')'}
          var
            counter: integer;
          begin
            Result := 0;

            counter := length(s);
            while counter <> 0 do
            begin
              case s[counter] of
                '(': inc(Result);
                ')': dec(Result);
              end;
              dec(counter);
            end;
          end;


          function CheckNumber(const s: string; var FloatNumber: ParserFloat):boolean;
          {checks whether s is a number}
          var
            code: integer;
          begin
            if s = 'PI' then
            begin
              FloatNumber := Pi;
              Result := true;
            end
            else
            if s = '-PI' then
            begin
              FloatNumber := -Pi;
              Result := true;
            end
            else
            begin
              if UsePascalNumbers then
              begin
                val(s, FloatNumber, code);
                Result := code = 0;
              end
              else
                Result := TryStrToFloat(s, FloatNumber);
            end;
          end;


          function CheckVariable(const s: string; var VariableID: integer): boolean;
          {checks whether s is a variable string}
          begin
            Result := Variables.Find(s, VariableID);
          end;


          function CheckTerm(var s1: string) :boolean;
          { checks whether s is a valid term }
          var
            s2, s3: TermString;
            FloatNumber: ParserFloat;
            fsort: TToken;
            VariableID: integer;
          begin
            Result := false;

            if length(s1) = 0 then
              exit;

            if CheckNumber(s1, FloatNumber) or
               CheckVariable(s1, VariableID) or
               CheckNegate(s1, s2) or
               CheckAdd(s1, s2, s3) or
               CheckSubtract(s1, s2, s3) or
               CheckMultiply(s1, s2, s3) or
{$IFDEF UseIntegerOP}
               CheckIntegerDiv(s1, s2, s3) or
               CheckModulo(s1, s2, s3) or
{$ENDIF UseIntegerOP}
               CheckRealDivision(s1, s2, s3) or
               CheckPower(s1, s2, s3, fsort) or
               CheckFuncTwoVar(s1, s2, s3) or
               CheckFuncOneVar(s1, s2)
            then
              Result := true
            else
              if CheckBracket(s1, s2) then
              begin
                s1 := s2;
                Result := true
              end;

          end;

          function CheckBracket(const s: string; var s1: string): boolean;
          {checks whether s =(...(s1)...) and s1 is a valid term}
          var
            SLen : integer;
          begin
            Result := false;

            SLen := Length(s);
            if (SLen > 0) and (s[SLen] = ')') and (s[1] = '(') then
            begin
              s1 := copy(s, 2, SLen-2);
              Result := CheckTerm(s1);
            end;
          end;


          function CheckNegate(const s: string; var s1: string) :boolean;
          {checks whether s denotes the negative value of a valid TOperation}
          var
            s2, s3: TermString;
            fsort: TToken;
            VariableID: integer;
          begin
            Result := false;

            if (length(s) <> 0) and (s[1] = '-') then
            begin
              
              s1 := copy(s, 2, length(s)-1);
              if CheckBracket(s1, s2) then
              begin
                s1 := s2;
                Result := true;
              end
              else
                Result :=
                  CheckVariable(s1, VariableID) or
                  CheckPower(s1, s2, s3, fsort) or
                  CheckFuncOneVar(s1, s2) or
                  CheckFuncTwoVar(s1, s2, s3);

            end;
          end;


          function CheckAdd(var s: string; var s1, s2: string): boolean;
          {checks whether '+' is the primary TOperation in s}
          var
            s3, s4: TermString;
            OldLen,
            i, j: integer;
            FloatNumber: ParserFloat;
            fsort: TToken;
            VariableID: integer;
          begin
            Result := false;

            i := 0;
            j := length(s);
            repeat

              while i < j do
              begin
                inc(i);
                if s[i] = '+' then
                  break;
              end;

              if (i > 1) and (i < j) then
              begin

                Result := false;

                s2 := copy(s, i+1, j-i);
                if CheckNumberBrackets(s2) = 0 then
                begin
                  OldLen := HackSetLength(S, i-1);
                  Result := CheckNumberBrackets(s) = 0;
                  HackSetLength(S, OldLen);

                  if Result then
                  begin
                    s1 := copy(s, 1, i-1);
                    Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);


                    if not Result then
                    begin
                      Result := CheckBracket(s1, s3);
                      if Result then
                        s1 := s3;
                    end;

                    if not Result then
                      Result := CheckNegate(s1, s3) or
                                CheckSubtract(s1, s3, s4) or
                                CheckMultiply(s1, s3, s4) or
{$IFDEF UseIntegerOP}
                                CheckIntegerDiv(s1, s3, s4) or
                                CheckModulo(s1, s3, s4) or
{$ENDIF UseIntegerOP}
                                CheckRealDivision(s1, s3, s4) or
                                CheckPower(s1, s3, s4, fsort) or
                                CheckFuncOneVar(s1, s3) or
                                CheckFuncTwoVar(s1, s3, s4);

                    if Result then
                    begin
                      Result := CheckNumber(s2, FloatNumber) or CheckVariable(s2, VariableID);

                      if not Result then
                      begin
                        Result := CheckBracket(s2, s3);
                        if Result then
                          s2 := s3
                        else
                          Result := CheckAdd(s2, s3, s4) or
                                    CheckSubtract(s2, s3, s4) or
                                    CheckMultiply(s2, s3, s4) or
{$IFDEF UseIntegerOP}
                                    CheckIntegerDiv(s2, s3, s4) or
                                    CheckModulo(s2, s3, s4) or
{$ENDIF UseIntegerOP}
                                    CheckRealDivision(s2, s3, s4) or
                                    CheckPower(s2, s3, s4, fsort) or
                                    CheckFuncOneVar(s2, s3) or
                                    CheckFuncTwoVar(s2, s3, s4);
                      end;
                    end;


                  end
                end
              end
              else
                break;

            until Result;
          end;



          function CheckSubtract(const s: string; var s1, s2: string): boolean;
          {checks whether '-' is the primary TOperation in s}
          var
            s3, s4: TermString;
            i, j: integer;
            FloatNumber: ParserFloat;
            fsort: TToken;
            VariableID: integer;
          begin
            Result := false;

            i := 1; { bugfix -1-1 }
            j := length(s);

            repeat

              while i < j do { bugfix -1-1 }
              begin
                inc(i);
                if s[i] = '-' then
                  break;
              end;

              if (i > 1) and (i < j) then
              begin
                s1 := copy(s, 1, i-1);
                s2 := copy(s, i+1, j-i);

                Result := (CheckNumberBrackets(s2)=0) and (CheckNumberBrackets(s1)=0);

                if Result then
                begin
                  Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);

                  
                  if not Result then
                  begin
                    Result := CheckBracket(s1, s3);
                    if Result then
                      s1 := s3;
                  end;
                  if not Result then
                    Result := CheckNegate(s1, s3) or
                              CheckSubtract(s1, s3, s4) or
                              CheckMultiply(s1, s3, s4) or
{$IFDEF UseIntegerOP}
                              CheckIntegerDiv(s1, s3, s4) or
                              CheckModulo(s1, s3, s4) or
{$ENDIF UseIntegerOP}
                              CheckRealDivision(s1, s3, s4) or
                              CheckPower(s1, s3, s4, fsort) or
                              CheckFuncOneVar(s1, s3) or
                              CheckFuncTwoVar(s1, s3, s4);

                  if Result then
                  begin
                    Result := CheckNumber(s2, FloatNumber) or CheckVariable(s2, VariableID);

                    if not Result then
                    begin
                       Result := CheckBracket(s2, s3);
                       if Result then
                         s2 := s3
                       else
                         Result := CheckMultiply(s2, s3, s4) or
{$IFDEF UseIntegerOP}
                                   CheckIntegerDiv(s2, s3, s4) or
                                   CheckModulo(s2, s3, s4) or
{$ENDIF UseIntegerOP}
                                   CheckRealDivision(s2, s3, s4) or
                                   CheckPower(s2, s3, s4, fsort) or
                                   CheckFuncOneVar(s2, s3) or
                                   CheckFuncTwoVar(s2, s3, s4);
                    end;
                  end;

                end;
              end
              else
                break;

            until Result;

          end;


          function CheckMultiply(const s: string; var s1, s2: string): boolean;
          {checks whether '*' is the primary TOperation in s}
          var
            s3, s4: TermString;
            i, j: integer;
            FloatNumber: ParserFloat;
            fsort: TToken;
            VariableID: integer;
          begin
            Result := false;

            i := 0;
            j := length(s);

            repeat
              while i < j do
              begin
                inc(i);
                if s[i] = '*' then
                  break;
              end;

              if (i > 1) and (i < j) then
              begin
                s1 := copy(s, 1, i-1);
                s2 := copy(s, i+1, j-i);

                Result := (CheckNumberBrackets(s2)=0) and (CheckNumberBrackets(s1)=0);

                if Result then
                begin
                  Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);

                  
                  if not Result then
                  begin
                    Result := CheckBracket(s1, s3);
                    if Result then
                      s1 := s3;
                  end;

                  if not Result then
                    Result := CheckNegate(s1, s3) or
{$IFDEF UseIntegerOP}
                              CheckIntegerDiv(s1, s3, s4) or
                              CheckModulo(s1, s3, s4) or
{$ENDIF UseIntegerOP}
                              CheckRealDivision(s1, s3, s4) or
                              CheckPower(s1, s3, s4, fsort) or
                              CheckFuncOneVar(s1, s3) or
                              CheckFuncTwoVar(s1, s3, s4);

                  if Result then
                  begin
                    Result := CheckNumber(s2, FloatNumber) or CheckVariable(s2, VariableID);

                    if not Result then
                    begin
                      Result := CheckBracket(s2, s3);
                      if Result then
                        s2 := s3
                      else
                        Result := CheckMultiply(s2, s3, s4) or
{$IFDEF UseIntegerOP}
                                  CheckIntegerDiv(s2, s3, s4) or
                                  CheckModulo(s2, s3, s4) or
{$ENDIF UseIntegerOP}
                                  CheckRealDivision(s2, s3, s4) or
                                  CheckPower(s2, s3, s4, fsort) or
                                  CheckFuncOneVar(s2, s3) or
                                  CheckFuncTwoVar(s2, s3, s4);
                    end;
                  end;

                end;
              end
              else
                break;

            until Result;
          end;

{$IFDEF UseIntegerOP}
          function CheckIntegerDiv(const s: string; var s1, s2: string): boolean;
          {checks whether 'DIV' is the primary TOperation in s}
          var
            s3, s4: TermString;
            i, j: integer;
            VariableID: integer;
            FloatNumber: ParserFloat;
            fsort: TToken;
          begin
            Result := false;

            i := 0;

            repeat

              j := IPosE('DIV', PChar(s), i+1, length(s)-i);

              if j > 0 then
              begin

                inc(i, j);
                if (i > 1) and (i < length(s)) then
                begin
                  s1 := copy(s, 1, i-1);
                  s2 := copy(s, i+3, length(s)-i-2);

                  Result := (CheckNumberBrackets(s2)=0) and (CheckNumberBrackets(s1)=0);

                  if Result then
                  begin
                    Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);

                    if not Result then
                    begin
                      Result := CheckBracket(s1, s3);
                      if Result then
                        s1 := s3;
                    end;

                    if not Result then
                      Result := CheckNegate(s1, s3) or
                                CheckIntegerDiv(s1, s3, s4) or
                                CheckModulo(s1, s3, s4) or
                                CheckRealDivision(s1, s3, s4) or
                                CheckPower(s1, s3, s4, fsort) or
                                CheckFuncOneVar(s1, s3) or
                                CheckFuncTwoVar(s1, s3, s4);
                    if Result then
                    begin
                      Result := CheckNumber(s2,FloatNumber) or CheckVariable(s2,VariableID);

                      if not Result then
                      begin
                        Result := CheckBracket(s2, s3);
                        if Result then
                          s2 := s3
                        else
                          Result := CheckPower(s2, s3, s4, fsort) or
                                    CheckFuncOneVar(s2, s3) or
                                    CheckFuncTwoVar(s2, s3, s4);
                      end;
                    end;


                  end;
                end;
              end;

            until Result or (j = 0) or (i >= length(s));
          end;

          function CheckModulo(const s: string; var s1, s2: string): boolean;
          {checks whether 'MOD' is the primary TOperation in s}
          var
            s3, s4: TermString;
            i, j: integer;
            VariableID: integer;
            FloatNumber: ParserFloat;
            fsort: TToken;
          begin
            Result := false;

            i := 0;
            
            repeat
                j := IPosE('MOD', PChar(s), i+1, length(s)-i);
              if j > 0 then
              begin

                inc(i, j);
                if (i > 1) and (i < length(s)) then
                begin
                  s1 := copy(s, 1, i-1);
                  s2 := copy(s, i+3, length(s)-i-2);

                  Result := (CheckNumberBrackets(s2)=0) and (CheckNumberBrackets(s1)=0);

                  if Result then
                  begin
                    Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);

                    if not Result then
                    begin
                      Result := CheckBracket(s1, s3);
                      if Result then
                        s1 := s3;
                    end;
                    if not Result then
                      Result := CheckNegate(s1, s3) or
                                CheckIntegerDiv(s1, s3, s4) or
                                CheckModulo(s1, s3, s4) or
                                CheckRealDivision(s1, s3, s4) or
                                CheckPower(s1, s3, s4, fsort) or
                                CheckFuncOneVar(s1, s3) or
                                CheckFuncTwoVar(s1, s3, s4);

                    if Result then
                    begin
                      Result := CheckNumber(s2, FloatNumber) or CheckVariable(s2, VariableID);

                      if not Result then
                      begin
                        Result := CheckBracket(s2, s3);
                        if Result then
                          s2 := s3
                        else
                          Result := CheckPower(s2, s3, s4, fsort) or
                                    CheckFuncOneVar(s2, s3) or
                                    CheckFuncTwoVar(s2, s3, s4);

                      end
                    end;


                  end;
                end;
              end;
            until Result or (j = 0) or (i >= length(s));
          end;
{$ENDIF UseIntegerOP}


          function CheckRealDivision(const s: string; var s1, s2: string): boolean;
          {checks whether '/' is the primary TOperation in s}
          var
            s3, s4: TermString;
            i, j: integer;
            VariableID: integer;
            FloatNumber: ParserFloat;
            fsort: TToken;
          begin
            Result := false;

            i := 0;
            j := length(s);

            repeat

              while i < j do
              begin
                inc(i);
                if s[i] = '/' then
                  break;
              end;

              if (i > 1) and (i < j) then
              begin
                s1 := copy(s, 1, i-1);
                s2 := copy(s, i+1, j-i);

                Result := (CheckNumberBrackets(s2)=0) and (CheckNumberBrackets(s1)=0);

                if Result then
                begin
                  Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);

                  
                  if not Result then
                  begin
                    Result := CheckBracket(s1, s3);
                    if Result then
                      s1 := s3;
                  end;

                  if not Result then
                    Result := CheckNegate(s1, s3) or
{$IFDEF UseIntegerOP}
                              CheckIntegerDiv(s1, s3, s4) or
                              CheckModulo(s1, s3, s4) or
{$ENDIF UseIntegerOP}
                              CheckRealDivision(s1, s3, s4) or
                              CheckPower(s1, s3, s4, fsort) or
                              CheckFuncOneVar(s1, s3) or
                              CheckFuncTwoVar(s1, s3, s4);

                  if Result then
                  begin
                    Result := CheckNumber(s2, FloatNumber) or CheckVariable(s2, VariableID);

                    if not Result then
                    begin
                      Result := CheckBracket(s2, s3);
                      if Result then
                        s2 := s3
                      else
                        Result := CheckPower(s2, s3, s4, fsort) or
                                  CheckFuncOneVar(s2, s3) or
                                  CheckFuncTwoVar(s2, s3, s4);

                    end;
                  end;

                end;
              end
              else
                break;

            until Result;
          end;


          function CheckFuncTwoVar(var s: string; var s1, s2: string): boolean;
          {checks whether s=f(s1,s2); s1,s2 being valid terms}

            function CheckComma(const s: string; var s1, s2: string): boolean;
            var
              i, j: integer;
            begin
              Result := false;

              i := 0;
              j := length(s);
              repeat

                while i < j do
                begin
                  inc(i);
                  if s[i] = ',' then
                    break;
                end;

                if (i > 1) and (i < j) then
                begin
                  s1 := copy(s, 1, i-1);
                  if CheckTerm(s1) then
                  begin
                    s2 := copy(s, i+1, j-i);
                    Result := CheckTerm(s2);
                  end;

                end
                else
                  break;

              until Result;
            end;

          var
            OldLen,
            SLen,
            counter : integer;
          begin

            Result := false;

              SLen := FastPos('(', PChar(s));

            if (SLen > 0) and (s[length(s)] = ')') then
            begin

              OldLen := HackSetLength(S, SLen);
              Result := FunctionTwo.Find(s, counter);
              HackSetLength(S, OldLen);

{              Result := FunctionTwo.Find(copy(s, 1, SLen), counter); }
              if Result then
              begin
                inc(SLen, 2);
                Result := CheckComma( copy(s, SLen, length(s)-SLen), s1, s2);
              end;
            end;
          end;


          function CheckFuncOneVar(var s: string; var s1: string): boolean;
          {checks whether s denotes the evaluation of a function fsort(s1)}
          var
            OldLen,
            counter: integer;
            SLen: integer;
          begin
            Result := false;

            { change}
              SLen := FastPos('(', PChar(s));

            if (SLen > 0) then
            begin
              OldLen := HackSetLength(S, SLen);
              Result := FunctionOne.Find(s, counter);
              HackSetLength(S, OldLen);

              { Result := FunctionOne.Find(copy(s, 1, SLen), counter); }
              if Result then
              begin
                Result := CheckBracket(copy(s, SLen+1, length(s)-SLen), s1);
              end;
            end;
          end;

          function CheckPower(const s: string; var s1, s2: string; var AToken: TToken): boolean;
          var
            s3, s4: TermString;
            i, j: integer;
            FloatNumber: ParserFloat;
            VariableID: integer;
          begin
            Result := false;

            i := 0;
            j := length(s);
            repeat

              while i < j do
              begin
                inc(i);
                if s[i] = '^' then
                  break;
              end;

              if (i > 1) and (i < j) then
              begin
                s1 := copy(s, 1, i-1);
                s2 := copy(s, i+1, j-i);

                Result := (CheckNumberBrackets(s2)=0) and (CheckNumberBrackets(s1)=0);

                if Result then
                begin
                  Result := CheckNumber(s1, FloatNumber) or CheckVariable(s1, VariableID);

                  if not Result then
                  begin
                    Result := CheckBracket(s1, s3);
                    if Result then
                      s1 := s3;
                  end;

                  if not Result then
                    Result := CheckFuncOneVar(s1, s3) or
                              CheckFuncTwoVar(s1, s3, s4);

                  if Result then
                  begin

                    if CheckNumber(s2, FloatNumber) then
                    begin
                      i := trunc(FloatNumber);

                      if (i <> FloatNumber) then
                      begin
                        { this is a real number }
                        AToken := realpower;
                      end
                      else
                      begin
                        case i of
                          2: AToken := square;
                          3: AToken := third;
                          4: AToken := fourth;
                        else
                          AToken := integerpower;
                        end;
                      end;
                    end
                    else
                    begin
                      Result := CheckVariable(s2, VariableID);

                      if not Result then
                      begin
                        Result := CheckBracket(s2, s3);
                        if Result then
                          s2 := s3;
                      end;

                      if not Result then
                      begin
                        Result := CheckFuncOneVar(s2, s3) or
                                  CheckFuncTwoVar(s2, s3, s4);
                      end;

                      if Result then
                        AToken := realPower;
                    end;
                  end;

                end;
              end
              else
                break;

            until Result;
          end;

          function CreateOperation(const Term: TToken; const Proc: Pointer): POperation;
          begin
            new(Result);
            with Result^ do
            begin
              Arg1 := nil;
              Arg2 := nil;
              Dest := nil;

              NextOperation := nil;

              Token := Term;

              MathProc := TMathProcedure(Proc);
            end;
          end;

const
  BlankString = ' ';

type
  PTermRecord = ^TermRecord;
  TermRecord = record
                 { this usage of string is a bit inefficient,
                   as in 16bit always 256 bytes are consumed.
                   But since we
                   a) are allocating memory dynamically and
                   b) this will be released immediately when
                      finished with parsing
                   this seems to be OK

                   One COULD create a "TermClass" where this is handled }
                 StartString: string;
                 LeftString, RightString: string;

                 Token: TToken;

                 Position: array[1..3] of integer;

                 Next1,
                 Next2,
                 Previous: PTermRecord;
               end;


const
  { side effect: for each bracketing level added
      SizeOf(integer) bytes additional stack usage
      maxLevelWidth*SizeOf(Pointer) additional global memory used }
  maxBracketLevels = 20;

  { side effect: for each additional (complexity) level width
      maxBracketLevels*SizeOf(Pointer) additional global memory used }
  maxLevelWidth = 50;
type
  LevelArray = array[0..maxBracketLevels] of integer;

  OperationPointerArray = array[0..maxBracketLevels, 1..maxLevelWidth] of POperation;
  POperationPointerArray = ^OperationPointerArray;

var
  Matrix: POperationPointerArray;

  { bracket positions }
  CurrentBracket,
  i,
  CurBracketLevels: integer;

  BracketLevel: LevelArray;

  LastOP: POperation;
  FloatNumber: ParserFloat;
  VariableID: integer;


  ANewTerm, { need this particlar pointer to guarantee a good, flawless memory cleanup in except }

  FirstTerm,
  Next1Term,
  Next2Term,
  LastTerm: PTermRecord;

  counter1,
  counter2: integer;
begin
  { initialize local variables for safe checking in try..finally..end}

  { FirstTerm := nil; } { not necessary since not freed in finally }
  LastTerm := nil;
  ANewTerm := nil;
  Next1Term := nil;
  Next2Term := nil;

  Error := false;

  FillChar(BracketLevel, SizeOf(BracketLevel), 0); { initialize bracket array }
  BracketLevel[0] := 1;
  CurBracketLevels := 0;

  new(Matrix);

  try { this block protects the whole of ALL assignments...}
    FillChar(Matrix^, SizeOf(Matrix^), 0);

    new(ANewTerm);
    with ANewTerm^ do
    begin

      StartString := UpperCase(FunctionString);

      { remove leading and trailing spaces }
      counter1 := 1;
      counter2 := length(StartString);
      while counter1 <= counter2 do
        if StartString[counter1] <> ' ' then
          break
        else
          inc(counter1);

      counter2 := length(StartString);
      while counter2 > counter1 do
        if StartString[counter2] <> ' ' then
          break
        else
          dec(counter2);

      StartString := Copy(StartString, counter1, counter2 - counter1 + 1);

      { change }
         if FastPos(' ', PChar(StartString)) <> 0 then
        raise EExpressionHasBlanks.Create(msgErrBlanks);
      {
      Old code:

         StartString := RemoveBlanks(UpperCase(FunctionString));

      ...do not use! Using it would create the following situation:

         Passed string:   "e xp(12)"
         Modified string: "exp(12)"

      This MAY or may not be the desired meaning - there may well exist
      a variable "e" and a function "xp" and just the operator would be missing.

      Conclusion: the above line has the potential of changing the meaning
                  of an expression.
      }
                                                 
      i := CheckNumberBrackets(StartString);
      if i > 0 then
        raise EMissMatchingBracket.CreateFmt(msgMissingBrackets, ['")"', i])
      else
        if i < 0 then
          raise EMissMatchingBracket.CreateFmt(msgMissingBrackets, ['"("', i]);

      { remove enclosing brackets, e.g. ((pi)) }
      while CheckBracket(StartString, FunctionString) do
        StartString := FunctionString;

      LeftString := BlankString;
      RightString := BlankString;

      Token := variab;

      Next1 := nil;
      Next2 := nil;
      Previous := nil;
    end;

    Matrix[0,1] := CreateOperation(variab, nil);

    LastTerm := ANewTerm;
    FirstTerm := ANewTerm;
    ANewTerm := nil;

    with LastTerm^ do
    begin
      Position[1] := 0;
      Position[2] := 1;
      Position[3] := 1;
    end;

    repeat

      repeat

        with LastTerm^ do
        begin

          CurrentBracket := Position[1];
          i := Position[2];

          if Next1 = nil then
          begin
            if CheckNumber(StartString, FloatNumber) then
            begin
              Token := constant;
              if Position[3] = 1 then
              begin
                new(Matrix[CurrentBracket, i]^.Arg1);
                Matrix[CurrentBracket, i]^.Arg1^ := FloatNumber;
              end
              else
              begin
                new(Matrix[CurrentBracket, i]^.Arg2);
                Matrix[CurrentBracket, i]^.Arg2^ := FloatNumber;
              end;
            end
            else
            begin
              if CheckVariable(StartString, VariableID) then
              begin
                Token := variab;

                if Position[3] = 1 then
                  Matrix[CurrentBracket, i]^.Arg1 := PParserFloat(Variables.Objects[VariableID])
                else
                  Matrix[CurrentBracket, i]^.Arg2 := PParserFloat(Variables.Objects[VariableID])
              end
              else
              begin
                if CheckNegate(StartString, LeftString) then
                  Token := minus
                else
                begin
                  if CheckAdd(StartString, LeftString, RightString) then
                    Token := sum
                  else
                  begin
                    if CheckSubtract(StartString, LeftString, RightString) then
                      Token := diff
                    else
                    begin
                      if CheckMultiply(StartString, LeftString, RightString) then
                        Token := prod
                      else
                      begin
{$IFDEF UseIntegerOP}
                        if CheckIntegerDiv(StartString, LeftString, RightString) then
                          Token := IntDiv
                        else
                        begin
                          if CheckModulo(StartString, LeftString, RightString) then
                            Token := modulo
                          else
{$ELSE}
                        begin
{$ENDIF UseIntegerOP}
                          begin
                            if CheckRealDivision(StartString, LeftString, RightString) then
                              Token := divis
                            else
                            begin
                              if not CheckPower(StartString, LeftString, RightString, Token) then
                              begin
                                if CheckFuncOneVar(StartString, LeftString) then
                                  Token := FuncOneVar
                                else
                                begin
                                  if CheckFuncTwoVar(StartString, LeftString, RightString) then
                                    Token := FuncTwoVar
                                  else
                                  begin
                                    Error := true; {with an exception raised this is meaningless...}
                                    if (LeftString = BlankString) and (RightString = BlankString) then
                                      raise ESyntaxError.CreateFmt(
                                         msgParseError+#13'%s', [StartString]
                                                                   )
                                    else
                                      raise ESyntaxError.CreateFmt(
                                         msgParseError+#13'%s'#13'%s', [Leftstring, RightString]
                                                                   )
                                  end;
                                end;
                              end;
                            end;
                          end;
                        end;
                      end;
                    end;
                  end;
                end;
              end;
            end;
          end;
        end; { with LastTerm^ }


        if LastTerm^.Token in ( [minus, square, third, fourth, FuncOneVar, FuncTwoVar] + TokenOperators) then
        begin
          if LastTerm^.Next1 = nil then
          begin
            try
              Next1Term := nil;
              new(Next1Term);

              inc(CurrentBracket);
              if CurrentBracket > maxBracketLevels then
              begin
                Error := true;
                raise ETooManyNestings.Create(msgNestings);
              end;

              i := BracketLevel[CurrentBracket] + 1;
              if i > maxLevelWidth then
              begin
                Error := true;
                raise EExpressionTooComplex.Create(msgTooComplex);
              end;

              if CurBracketLevels < CurrentBracket then
                CurBracketLevels := CurrentBracket;

              with Next1Term^ do
              begin
                StartString := LastTerm^.LeftString;
                LeftString := BlankString;
                RightString := BlankString;

                Position[1] := CurrentBracket;
                Position[2] := i;
                Position[3] := 1;

                Token := variab;

                Previous := LastTerm;
                Next1 := nil;
                Next2 := nil;
              end;

              with LastTerm^ do
              begin
                case Token of
                  FuncOneVar:
                    with FunctionOne do
                    begin
                        SetLength(StartString, FastPos('(', PChar(StartString)));
                      Find(StartString, counter1);

                      Matrix[CurrentBracket, i] := CreateOperation(
                                       Token, Objects[counter1] );

                    end;


                  FuncTwoVar:
                    with FunctionTwo do
                    begin
                        SetLength(StartString, FastPos('(', PChar(StartString)));
                      Find(StartString, counter1);

                      Matrix[CurrentBracket, i] := CreateOperation(
                                       Token, Objects[counter1] );
                    end;
                else
                  Matrix[CurrentBracket, i] := CreateOperation(Token, nil);
                end;

                new(Matrix[CurrentBracket, i]^.Dest);
                Matrix[CurrentBracket, i]^.Dest^ := 0;

                if Position[3] = 1 then
                  Matrix[Position[1], Position[2]]^.Arg1 :=
                    Matrix[CurrentBracket, i]^.Dest
                else
                  Matrix[Position[1], Position[2]]^.Arg2 :=
                    Matrix[CurrentBracket, i]^.Dest;

                Next1 := Next1Term;
                Next1Term := nil;
              end;

              if LastTerm^.Token in [minus, square, third, fourth, FuncOneVar] then
              inc(BracketLevel[CurrentBracket]);

            except
              if assigned(Next1Term) then
              begin
                dispose(Next1Term);
                Next1Term := nil;
              end;
              raise;
            end;

          end

          else
          begin
            if LastTerm^.Token in (TokenOperators + [FuncTwoVar]) then
            begin
              try
                Next2Term := nil;
                new(Next2Term);

                inc(CurrentBracket);
                if CurrentBracket > maxBracketLevels then
                begin
                  Error := true;
                  raise ETooManyNestings.Create(msgNestings);
                end;

                i := BracketLevel[CurrentBracket] + 1;
                if i > maxLevelWidth then
                begin
                  Error := true;
                  raise EExpressionTooComplex.Create(msgTooComplex);
                end;

                if CurBracketLevels < CurrentBracket then
                  CurBracketLevels := CurrentBracket;

                with Next2Term^ do
                begin
                  StartString := LastTerm^.RightString;

                  LeftString := BlankString;
                  RightString := BlankString;

                  Token := variab;

                  Position[1] := CurrentBracket;
                  Position[2] := i;
                  Position[3] := 2;

                  Previous := LastTerm;
                  Next1 := nil;
                  Next2 := nil;
                end;

                LastTerm^.Next2 := Next2Term;
                Next2Term := nil;
                inc(BracketLevel[CurrentBracket]);

              except
                if assigned(Next2Term) then
                begin
                  dispose(Next2Term);
                  Next2Term := nil;
                end;

                raise;
              end;
            end
            else
              raise EParserInternalError.Create(msgInternalError);
          end;
        end;


        with LastTerm^ do
          if Next1 = nil then
          begin
          { we are done with THIS loop }
            break;
          end
          else
            if Next2 = nil then
              LastTerm := Next1
            else
              LastTerm := Next2;

      until false; { endless loop, break'ed 7 lines above }

      if LastTerm = FirstTerm then
      begin
        dispose(LastTerm);
        FirstTerm := nil;
        break; { OK - that is it, we did not find any more terms}
      end;

      repeat
        with LastTerm^ do { cannot use "with LastTerm^" OUTSIDE loop }
        begin
          if Next1 <> nil then
          begin
            dispose(Next1);
            Next1 := nil;
          end;

          if Next2 <> nil then
          begin
            dispose(Next2);
            Next2 := nil;
          end;

          LastTerm := Previous;
        end;
      until ((LastTerm^.Token in (TokenOperators + [FuncTwoVar])) and (LastTerm^.Next2 = nil)) or
            (LastTerm = FirstTerm);

      with FirstTerm^ do
      if (LastTerm = FirstTerm) and
            (  (Token in [minus, square, third, fourth, FuncOneVar]) or
               ((Token in (TokenOperators + [FuncTwoVar])) and Assigned(Next2))
            ) then
      begin
        break;
      end;


    until false;


    { after having built the expression matrix, translate it into a tree/list }

    with FirstTerm^ do
      if FirstTerm <> nil then
      begin
        if Next1 <> nil then
        begin
          dispose(Next1);
          Next1 := nil;
        end;

        if Next2 <> nil then
        begin
          dispose(Next2);
          Next2 := nil;
        end;

        dispose(FirstTerm);
      end;

    BracketLevel[0] := 1;

    if CurBracketLevels = 0 then
    begin
      FirstOP := Matrix[0,1];
      Matrix[0,1] := nil;
      FirstOP^.Dest := FirstOP^.Arg1;
    end
    else
    begin

      FirstOP := Matrix[CurBracketLevels, 1];
      LastOP := FirstOP;

      for counter2 := 2 to BracketLevel[CurBracketLevels] do
      begin
        LastOP^.NextOperation := Matrix[CurBracketLevels, counter2];
        LastOP := LastOP^.NextOperation;
      end;


      for counter1 := CurBracketLevels-1 downto 1 do
        for counter2 := 1 to BracketLevel[counter1] do
        begin
          LastOP^.NextOperation := Matrix[counter1, counter2];
          LastOP := LastOP^.NextOperation;
        end;


      with Matrix[0,1]^ do
      begin
        Arg1 := nil;
        Arg2 := nil;
        Dest := nil;
      end;

      dispose(Matrix[0,1]);
    end;

    dispose(Matrix);

  except
    if Assigned(Matrix) then
    begin
      if Matrix[0,1] <> nil then
        dispose(Matrix[0,1]);

      for counter1 := CurBracketLevels downto 1 do
        for counter2 := 1 to BracketLevel[counter1] do
          if Matrix[counter1, counter2] <> nil then

            dispose(Matrix[counter1, counter2]);

      dispose(Matrix);
    end;

    if Assigned(Next1Term) then
      dispose(Next1Term);

    if Assigned(Next2Term) then
      dispose(Next2Term);

{   do NOT kill this one at it is possibly the same as LastTerm (see below)!
    if Assigned(FirstTerm) then
      dispose(FirstTerm);

    instead, DO kill ANewTerm, which will only be <> nil if it has NOT passed
    its value to some other pointer already so it can safely be freed
}
    if Assigned(ANewTerm) then
      dispose(ANewTerm);

    if Assigned(LastTerm) and (LastTerm <> Next2Term) and (LastTerm <> Next1Term) then
      dispose(LastTerm);

    FirstOP := nil;

    raise; { re-raise exception }
  end;
end;

initialization
  MakeCharTable;
end.

