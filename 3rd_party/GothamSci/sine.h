//---------------------------------------------------------------------------

#ifndef sineH
#define sineH

#define MAX_SINE_TABLE_SAMPLES 32768

class PACKAGE TSineTable : public TObject
{
  private:
    int angle;
    short sine_table[MAX_SINE_TABLE_SAMPLES];
    void __fastcall build_sine_table();
  public:
    __fastcall TSineTable();
    __fastcall ~TSineTable();
    void __fastcall FillBufferWithSineWave(short *buffer, double sample_rate,
                                          double frequency, int numsamples,
                                          bool reset = false);
};

//---------------------------------------------------------------------------
#endif
