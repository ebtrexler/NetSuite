//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "sine.h"
#include "tools.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//private functions:
void __fastcall TSineTable::build_sine_table()
{
  int i, quarter_table, half_table, x;

  // Assume 16-bit audio goes from -32767..32767, avoids clipping.
  // There are only 2^15 samples here, this simplfies the subsequent angle
  // calculation but might restrict the dynamic range produced with noise
  // sidebands.  However, in the quality of equipment likely to be
  // encountered this won't matter.  You've got the source code, so
  // you can alter this if you like.
  half_table = MAX_SINE_TABLE_SAMPLES / 2;
  quarter_table = half_table / 2;
  sine_table[0] = 0;
  x = 32767;
  sine_table[quarter_table] = x;
  sine_table[half_table] = 0;
  sine_table[half_table + quarter_table] = -x;
  for (i=1; i<quarter_table; i++)
  {
    x = round (32767.0 * sin (2.0 * i * M_PI / MAX_SINE_TABLE_SAMPLES));
    sine_table[i] = x;
    sine_table[half_table - i] = x;
    sine_table[half_table + i] = -x;
    sine_table[MAX_SINE_TABLE_SAMPLES - i] = -x;
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//public function:

__fastcall TSineTable::TSineTable()
{
  build_sine_table();
  angle = 0;
}

//---------------------------------------------------------------------------
__fastcall TSineTable::~TSineTable()
{
}

//---------------------------------------------------------------------------
void __fastcall TSineTable::FillBufferWithSineWave(short *buffer,
                                                  double samplerate,
                                                  double frequency,
                                                  int numsamples,
                                                  bool reset)
{
  if (reset) angle = 0;

// Fills a buffer with a sinewave.  The angle parameter is
// updated so that phase continuity is maintained between calls to this routine

  const int fract_bits = 15;

  int i_sine_table_samples = MAX_SINE_TABLE_SAMPLES;
  double f_sine_table_samples = i_sine_table_samples << fract_bits;

  int sample,            // looping over the required samples
  d_angle,               // 32-bit number, with 15 fractional bits, i.e. 17.15
  max_angle;             // maximum number of samples in the sine table
  short w;               // one single sample

  // Compute the angular step per sample corresponding to the desired frequency
  d_angle = round (f_sine_table_samples * frequency / samplerate);

  // This is the maximum number of samples in the sine table
  max_angle = (i_sine_table_samples << fract_bits) - 1;

  for (sample=0; sample<numsamples; sample++)
  {
    w = sine_table[angle >> fract_bits];   // get current sine value
    buffer[sample] = w;                    // store it in the caller's buffer
    angle += d_angle;                      // bump the angle
    angle = angle & max_angle;             // wrap to 360 degrees
  }
}

//---------------------------------------------------------------------------