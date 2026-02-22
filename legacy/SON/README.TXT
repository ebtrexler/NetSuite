SON library distribution disk                            12 Sep 2006
--------------------------------------------------------------------

The SON library distribution disk provides support for using the
SON library as part of a 32-bit Windows application or Win32 console
application. The library is supplied as a DLL and as a simple .OBJ
file suitable for static linking; use of the DLL version is
recommended.

The installer puts all of the standard SON library files plus special
support for the languages selected into the directory specified.
The files provided are as follows:


SON.H, SONINTL.H and MACHINE.H
The include files required. You should only need to include
SON.H directly, WINDOWS.H should be included before SON.H.

SON32.DLL and SON32.LIB
The SON library built as a DLL, and the library file to link with
to use automatic loading and linking to the DLL. Both of these were
built with MS Visual Studio 6. The DLL should be suitable for
direct use with many other programming languages as it includes
IDL resources defining the functions within, these resources can
be read by development systems such as Delphi.

SON32.ODL
The extracted SON32 type library, which can be directly imported by
some development systems to provide all the information needed to
use and link to the functions in SON32.DLL.

SON32.OBJ
A simple compilation of SON suitable for direct linkage where
DLLs are not appropriate.


SON32.PAS
A file suitable for inclusion in a Delphi project that defines
all of the functions in the SON32 DLL. This is suitable if the
internal definitions of function provided by the DLL or the ODL
file are not suitable.

SON32BAS.TXT
A file suitable for inclusion in a Visual BASIC project that
defines all of the functions in the SON32 DLL. This is suitable
if the internal definitions of function provided by the DLL or
the ODL are not suitable.

son9.pdf
The documentation of the SON library in PDF (Adobe Acrobat) format.



Please contact CED if the versions of compilers used do not match
your requirements.
