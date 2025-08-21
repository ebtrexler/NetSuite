#include <vcl.h>
#pragma hdrstop

#include <memory>
#include <cassert>
#include <fastmath.h>
#include "tools.h"

#pragma package(smart_init)

AveVarParms::AveVarParms(): n(0), sum(0.0), v(0.0), ep(0.0)
   {
   }

int round(long double x) {
	long double top, bottom, avg;
	top = ceil(x);
	bottom = floor(x);
	avg = (top + bottom) / 2;
	return(x < avg) ? (int)bottom : (int)top;
}

int round(double x) {
	double top, bottom, avg;
	top = ceil(x);
	bottom = floor(x);
	avg = (top + bottom) / 2;
	return(x < avg) ? (int)bottom : (int)top;
}

int round(float x) {
	float top, bottom, avg;
	top = ceil(x);
	bottom = floor(x);
	avg = (top + bottom) / 2;
	return(x < avg) ? (int)bottom : (int)top;
}


short getmedian(int windowwidth, short *window) {
	short min1, min2;
	min1 = 32767;
	min2 = -32767;
	for (int i = 0; i < (windowwidth / 2 + 1); i++) {
		min1 = window[0];
		for (int j = 0; j < windowwidth; j++)
			if (window[j] < min1 && window[j] > min2)
				min1 = window[j];
		min2 = min1;
	}
	return min1;
}

short* medianfilter(short *in, int nopoints, int &windowwidth) {
	int i, j;
	if (!(windowwidth % 2))
		windowwidth -= 1;
	if (windowwidth < 3)
		windowwidth = 3;
	if (windowwidth > 105)
		windowwidth = 105;
	short *out = new short[nopoints];
	short *pointer;
	for (i = 0; i < (windowwidth / 2 + 1); i++) {
		out[i] = in[i];
		out[nopoints - 1 - i] = in[nopoints - 1 - i];
	}
	for (j = i; j < (nopoints - i); j++) {
		pointer = &in[j - i];
		out[j] = getmedian(windowwidth, pointer);
	}
	return out;
}

short* digitalgaussianfilter(short *in, int nopoints, double fc) {
	int nc; // number of coefficients
	short *out; // return array
	double a[55]; // array of coefficients
	double sigma; // width of impulse response
	double b, temp, sum; // other locals
	int jl, ju; //
	int i, j, k; // loop counters
	out = new short[nopoints];

	// calculate coefficients.........................
	sigma = 0.132505 / fc;
	if (sigma < 0.62) { // only 3 terms for narrow impulse response
		a[2] = sigma * sigma / 2.0;
		a[1] = 1.0 - 2.0 * a[2];
		nc = 1;
	}
	else { // standard gaussian coefficients.
		nc = ceil(4.0 * sigma); // nc is the number of coefficients minus central
		if (nc > 53)
			nc = 53;
		b = -0.5 / (sigma * sigma);
		a[1] = 1.0;
		sum = 0.5;
		for (i = 1; i <= nc; i++) {
			temp = exp(i * i * b);
			a[i + 1] = temp;
			sum += temp;
		}
		// normalize the coefficients
		sum *= 2.0;
		for (i = 1; i <= nc + 1; i++) {
			a[i] /= sum;
		}
	}

	// do the actual filtering here.......................
	for (i = 1; i <= nopoints; i++) {
		jl = i - nc;
		if (jl < 1)
			jl = 1;
		ju = i + nc;
		if (ju > nopoints)
			ju = nopoints;
		sum = 0.0;

		for (j = jl; j <= ju; j++) {
			k = abs(j - i) + 1;
			sum += in[j - 1] * a[k];
		}
		out[i - 1] = sum;
	}

	return out;
}

void CummAveVar(double data, double &ave, double &var, bool reset) {
	static int n = 0;
	static double sum = 0.0;
	static double v = 0.0;
	static double ep = 0.0;

	if (reset) {
		n = 0;
		sum = 0.0;
		v = 0.0;
		ep = 0.0;
	}

	n++;
	sum += data;
	ave = sum / n;

	double s = data - ave;
	ep += s;
	v += s * s;

	if (n == 1) {
		var = 0.0;
		return;
	}
	var = (v - pow(ep, 2) / n) / (n - 1);
}

void CummAveVarP(AveVarParms *parms, double data, double &ave, double &var,
	bool reset) {
	if (reset) {
		parms->n = 0;
		parms->sum = 0.0;
		parms->v = 0.0;
		parms->ep = 0.0;
	}

	parms->n++;
	parms->sum += data;
	ave = parms->sum / parms->n;

	double s = data - ave;
	parms->ep += s;
	parms->v += s * s;

	if (parms->n == 1) {
		var = 0.0;
		return;
	}
	var = (parms->v - pow(parms->ep, 2) / parms->n) / (parms->n - 1);
}

TRect GetIntersection(TRect &r1, TRect &r2) {
	int left, right, top, bottom;
	if (r1.left <= r2.left && r2.left < r1.right)
		left = r2.left;
	if (r2.left <= r1.left && r1.left < r2.right)
		left = r1.left;
	if (r1.right >= r2.right && r2.right > r1.left)
		right = r2.right;
	if (r2.right >= r1.right && r1.right > r2.left)
		right = r1.right;
	if (r1.top <= r2.top && r2.top < r1.bottom)
		top = r2.top;
	if (r2.top <= r1.top && r1.top < r2.bottom)
		top = r1.top;
	if (r1.bottom >= r2.bottom && r2.bottom > r1.top)
		bottom = r2.bottom;
	if (r2.bottom >= r1.bottom && r1.bottom > r2.top)
		bottom = r1.bottom;

	return Rect(left, top, right, bottom);
}

// ---------------------------------------------------------------------------

int __fastcall SaveComponent(String filename, TComponent* Component) {
	assert(Component != NULL);
	assert(Component->Owner != NULL);

	std::auto_ptr<TFileStream>fs(new TFileStream(filename, fmCreate));
	std::auto_ptr<TWriter>Writer(new TWriter(fs.get(), 4096));

	// specify the Root component
	Writer->Root = Component->Owner;
	// write the component
	Writer->WriteComponent(Component);
	// return the number of bytes written
	return Writer->Position;
}
// ---------------------------------------------------------------------------

int __fastcall SaveComponentToStream(TFileStream *fs, TComponent* Component) {
	assert(Component != NULL);
	assert(Component->Owner != NULL);

	// std::auto_ptr<TFileStream> fs(
	// new TFileStream(filename, fmCreate)
	// );
	std::auto_ptr<TWriter>Writer(new TWriter(fs, 4096));

	// specify the Root component
	Writer->Root = Component->Owner;
	// write the component
	Writer->WriteComponent(Component);
	// return the number of bytes written
	return Writer->Position;
}
// ---------------------------------------------------------------------------

int __fastcall LoadComponent(String filename, TComponent*& Component) {
	assert(Component != NULL);
	assert(Component->Owner != NULL);

	std::auto_ptr<TFileStream>fs(new TFileStream(filename, fmOpenRead));
	std::auto_ptr<TReader>Reader(new TReader(fs.get(), 4096));

	// set Root, Owner, and Parent
	Reader->Root = Component->Owner;
	Reader->Owner = Component->Owner;
	TControl* Control = dynamic_cast<TControl*>(Component);
	if (Control) {
		Reader->Parent = Control->Parent;
	}

	// remove the existing component
	delete Component;
	Component = NULL;

	// load the stored component
	Reader->BeginReferences();
	try {
		Component = Reader->ReadComponent(NULL);
	}
	__finally {
		Reader->FixupReferences();
		Reader->EndReferences();
	}

	// return the number of bytes read
	return Reader->Position;
}
// ---------------------------------------------------------------------------

int __fastcall LoadComponentFromStream(TFileStream *fs, TComponent*& Component)
{
	assert(Component != NULL);
	assert(Component->Owner != NULL);

	// std::auto_ptr<TFileStream> fs(
	// new TFileStream(filename, fmOpenRead)
	// );
	std::auto_ptr<TReader>Reader(new TReader(fs, 4096));

	// set Root, Owner, and Parent
	Reader->Root = Component->Owner;
	Reader->Owner = Component->Owner;
	TControl* Control = dynamic_cast<TControl*>(Component);
	if (Control) {
		Reader->Parent = Control->Parent;
	}

	// remove the existing component
	delete Component;
	Component = NULL;

	// load the stored component
	Reader->BeginReferences();
	try {
		Component = Reader->ReadComponent(NULL);
	}
	__finally {
		Reader->FixupReferences();
		Reader->EndReferences();
	}

	// return the number of bytes read
	return Reader->Position;
}
// ---------------------------------------------------------------------------

void __fastcall RegisterOwnedComponents(TComponent& Component) {
	const int count = Component.ComponentCount;
	for (int idx = 0; idx < count; ++idx) {
		TComponent& OwnedComponent = *Component.Components[idx];
		/*
		const
		 */ TComponentClass classes[1] = {
			OwnedComponent.ClassType()
		};

		// register OwnedComponent
		RegisterClasses(classes, 0);

		// recursively register all of
		// OwnedComponent's components
		RegisterOwnedComponents(OwnedComponent);
	}
}
// ---------------------------------------------------------------------------

int nexteven(double x) {
	return 2 * floor((x - 1) / 2) + 2;
}

bool FloatsAreEqual(long double a, long double b, int precision) {
	// precision is int giving number of digits precision

	int powten = 10;
	int a_result, b_result;

	for (int i = 0; i < precision; i++)
		powten *= 10;

	a_result = round(a * powten);
	b_result = round(b * powten);

	return a_result == b_result;
}

const String GetDocumentsAppPath(const String Company, const String AppName)
{
	const int SHGFP_TYPE_CURRENT = 0;
	TCHAR szPath[MAX_PATH];
	String path(NULL);
	if(SUCCEEDED(SHGetFolderPath(NULL,
									  CSIDL_PERSONAL|CSIDL_FLAG_CREATE,
									  NULL,
									  0,
									  szPath))){

		path = String(szPath) + "\\" + Company + "\\" + AppName + "\\";
	}
	return path;
}

