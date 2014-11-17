#ifndef _tools_h
#define _tools_h

#include <Controls.hpp>
#include <Classes.hpp>

class PACKAGE AveVarParms
{
private:

protected:

public:
   int     n;
   double   sum;
   double   v;
   double   ep;
   AveVarParms();
};

extern PACKAGE int		round(float x);
extern PACKAGE int		round(double x);
extern PACKAGE int		round(long double x);
extern PACKAGE short*	medianfilter(short *in, int nopoints, int &windowwidth);
extern PACKAGE short*	digitalgaussianfilter(short *in, int nopoints, double fc);
extern PACKAGE void		CummAveVar(double data, double &ave, double &var,
										bool reset = false);
extern PACKAGE void		CummAveVarP(AveVarParms *parms, double data, double &ave,
                              double &var, bool reset);// = false);
extern PACKAGE Types::TRect  	GetIntersection(Types::TRect &r1, Types::TRect &r2);
int PACKAGE __fastcall  SaveComponent(String filename, TComponent* Component);
int PACKAGE __fastcall  LoadComponent(String filename, TComponent*& Component);
int PACKAGE __fastcall  SaveComponentToStream(TFileStream *fs, TComponent* Component);
int PACKAGE __fastcall  LoadComponentFromStream(TFileStream *fs, TComponent*& Component);
void PACKAGE __fastcall RegisterOwnedComponents(TComponent& Component);
extern PACKAGE int      nexteven(double x);
extern PACKAGE bool     FloatsAreEqual(long double a, long double b, int precision = 5);

template<class T> inline bool within(const T& value, const T& low,const T& high)
{
   return ((high >= value) && (value >= low));
}

template<class T> inline T constrain(const T& value, const T& low,const T& high)
{
   return (high < value) ? high : (value < low) ? low : value;
}

extern PACKAGE const String GetDocumentsAppPath(const String Company, const String AppName);

#endif
