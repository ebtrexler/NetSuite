unit UAFDefs;

               interface
               
{------------------------type defintions-----------------------}

Type
   UAF_FILE_TYPE =
        (UAF_TYPE_UNKNOWN,       { Invalid or Unknow File }
         UAF_TYPE_ADF,           { Bliss Audio Data Files }
         UAF_TYPE_WAV,           { Microsoft RIFF wav Files }
         UAF_TYPE_AU,            { Sun AU file format }
         UAF_TYPE_AIFF,          { Apple Wave format }
         UAF_TYPE_RAW);          { Raw input stream }

Type
   UAF_File = Record
     FrameRate     : Double;
     Channels      : Word;
     Quantization  : Word;
     Frames        : LongInt;
     FrameSize     : Word;
     FileRecord    : Pointer;
     FileType      : UAF_FILE_TYPE;
     SubType       : Integer;
     FrameSizeIn   : Word;         { Just the input frame size }
   End;

{---------------------Function and subroutines------------------------}

   Function UAF_open
      (
         Var uaf    : uaf_file;        { ADF handle                   }
         fname     : String;           { File name to open            }
         mode      : char;             { r,w for Read Write           }
         ForceType : UAF_FILE_TYPE     { If non-zero, force the type  }
      ) : Boolean; external 'uafdll.dll';

   Function UAF_create
      (
         Var uaf   : uaf_file;         { ADF handle                 }
         fname     : String;           { File name to open          }
	 ftype     : UAF_FILE_TYPE;    { Type of audio file         }
         subformat : Word;             { Subtype                    }
         srate     : Double;           { Sampling rate              }
         nchannels : Word;             { Channels 1 or 2            }
         bits      : Word              { Bits /sample               }
      ) : Boolean;  external 'uafdll.dll';

   Function UAF_close
      (
         var uaf  : uaf_file        { ADF handle                }
       ) : Boolean; external 'uafdll.dll';

   Function UAF_read
      (
         Var uaf    : uaf_file;         { ADF handle                }
         buffer     : Pointer;          { Buffer                    }
         nFrames    : LongInt;          { how many words            }
         lpos       : LongInt           { Position to read          }
      ) : LongInt; external 'uafdll.dll';

   Function UAF_write
      (
         Var uaf    : uaf_file;         { ADF handle                }
         buffer     : Pointer;            { Buffer                    }
         nFrames    : LongInt;          { how many words            }
         lpos       : LongInt           { Position to read          }
      ) : LongInt;  external 'uafdll.dll';


   Function UAF_CreateFromUAF(Var uafin, uafout : uaf_file; fname : String) : Boolean;
external 'uafdll.dll';
   Function UAF_SaveSection(Var uafin, uafout : uaf_file; lstart, lend : LongInt) : Boolean;
external 'uafdll.dll';
   Procedure UAF_Copy_Marks(Var uafin, uafout  : uaf_file);
external 'uafdll.dll';
   Function FindMinMaxUAF(FileIn : String; Var Min, Max : Real; OverrideType : UAF_FILE_TYPE) : Boolean;
external 'uafdll.dll';
   Function UAFTypeFromExtension (fname : String) : UAF_FILE_TYPE;
external 'uafdll.dll';
   Function UAF_ErrorMessage : PChar; External 'uafdll.dll';

   Function UAF_Identity(UAFIn : UAF_File) : PChar; External 'uafdll.dll';
   Function UAF_Description(UAFIn : UAF_File) : PChar; External 'uafdll.dll';

implementation

end.
