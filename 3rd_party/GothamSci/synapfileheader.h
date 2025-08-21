//---------------------------------------------------------------------------

#ifndef synapfileheader_h
#define synapfileheader_h
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  synapfileheader.h   created 07/07/2001 ebt
//  AT-MIO-16X is no longer supported
//  Only E-series (AT- and PCI-MIO-16XE-10)
//  Using NI-DAQ 6.9 or higher
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//NI-DAQ defines
//#include "NIDAQCNS.H"

//defined in <winbase.h>
//defined here to decrease header size
#ifndef _WINBASE_
typedef struct _SYSTEMTIME {  // st
	 WORD wYear;
	 WORD wMonth;
	 WORD wDayOfWeek;
	 WORD wDay;
	 WORD wHour;
	 WORD wMinute;
	 WORD wSecond;
	 WORD wMilliseconds;
} SYSTEMTIME;
#endif

//cool character --> ±

#define MAXCHANNELS			16
#define MAXTAGS				1000
#define SYNAPMAGICNUMBER	39291815L

//#ifndef _OLDDAQINUSE_
namespace NewDAQ {

enum 						BoardType 		{NOBOARD = -1, ATMIO16XE10, PCIMIO16XE10,
													NumBoards};
enum 						InstrumentType	{AxoPatch200B, AxoPatch1D_CV4_1_100,
													AxoPatch1D_CV4_01_100, UserDefInst};
enum						AcquisitionType {Continuous, Episodic, Triggered,
													EventTiming, NumAcquisitionTypes};
enum						TagType {UserClick, DigIO, DACHolding, UserStimulator,
													NumTagTypes};

//char                 *BoardTypeText[NumBoards] =
//								{"ATMIO16XE10", "PCIMIO16XE10"};
//char                 *InstrumentTypeText[UserDefInst] =
//								{"AxoPatch200B", "AxoPatch1D_CV4_1_100",
//								"AxoPatch1D_CV4_01_100"};
//char                 *AcquisitionTypeText[NumAcquisitionTypes] =
//								{"Continuous", "Episodic", "Triggered", "EventTiming"};
//char                 *TagTypeText[NumTagTypes] = {"UserClick", "DigIO",
//								"DACHolding", "UserStimulator"};

#pragma pack(4) //align the following on 4 byte boundaries

#define MAXADCNAMELENGTH 32
#define MAXADCUNITSNAMELENGTH 18

typedef struct PACKAGE tagADCConfig//////////////////////////////////////////////bytes  116
{
	char					Name[MAXADCNAMELENGTH];	// name of ADC						// 32

   char					InputMode;		// 0=Diff, 1=RSE, 2=NRSE					//  1
   char					Range;			// 0=Bipolar (-10 to +10V),				//  1
   											// 1=Unipolar (0 to +10V)
   char					Units[MAXADCUNITSNAMELENGTH];	// name of units			// 18
   unsigned short		ADCGain;			// adc gain on daq board					//  2
   short					GainADC;			// inst. gain sampled on this ADC		//  2
   short					FreqADC;			// lowpass freq. sampled on this ADC	//  2
   short					Instrument; 	// instrument enum							//  2
   float					LowpassFrequency;	// inst. output lowpass freq.			//  4
   												// (may be autosampled)
   float					InstrumentGain;//inst.gain (may be autosampled)			//  4
   float					InstrumentOffset;		// inst. offset correction			//  4
} ADCConfig;

typedef struct PACKAGE tagEpochDefinition//////////////////////////////////////////bytes 20
{
	int 					Type;	// 1=step, 2=ramp, 0=disabled, -1=appgenhold 	//  4
	float 				Level;			// in mV											//  4
	float 				LevelInc;		// in mV											//  4
	float 				Duration;		// in ms											//  4
	float 				DurationInc;	// in ms											//  4
}EpochDefinition;


typedef struct PACKAGE tagPNSubtractConfig///////////////////////////////////////bytes   20
{
	int					Active; 		// 0 = none, -1 = neg., +1 = pos.			//  4
   int					SaveData; 	// 0 = no, 1 = yes								//  4
   int					XScaleFactor;// 2 to 10,	representing episode 		//  4
   											// length divisor
   float					YScaleFactor;// should be <1 for waveform scaling 		//  4
   unsigned long		NumRepeats; // repeat scaled-down waveform n times 	//  4
}PNSubtractConfig;

typedef struct PACKAGE tagCondPulseConfig////////////////////////////////////////bytes   24
{
   float					BeforePulseLevel; //level before pulses (mV)				//  4
   float					BeforePulse_us;	// time before pulses (us)				//  4
   float					PulseHighLevel;// pulse level (mV)							//  4
   float					PulseLowLevel;	// pulse low level (mV)						//  4
   float					PulseHigh_us;	// length of high part of pulse (us)	//  4
	float					PulseLow_us;	// length of low part of pulse (us)		//  4
   float					DelayToEpStartLevel; // level during delay (mV)			//  4
   float					DelayToEpStart_us; // delay to start of episode acq.(us)
   													// i.e. start of P/N sub. if any	//  4
   unsigned long		NumPulses;		// number of pulses to apply				//  4
} CondPulseConfig;

#define MAXNUMEPOCHS 100
#define MAXFUNCTIONLENGTH 128

#define WFM_APP_USE_EPOCHS		0x00000001/////////////decimal  1//binary 00000001
#define WFM_APP_USE_FUNCTION	0x00000002/////////////decimal  2//binary 00000010
#define WFM_APP_USE_FILE		0x00000004/////////////decimal  4//binary 00000100
#define WFM_APP_USE_PNSUB		0x00000008/////////////decimal  8//binary 00001000
#define WFM_APP_USE_CONDPULSE 0x00000010/////////////decimal 16//binary 00010000

typedef struct PACKAGE tagWaveformDefinition/////////////////////////////////////bytes 2184
{
	unsigned long		WFMApplication;  // use of epochs, function, file		//  4
	int 					InterEpisodeHolding; //0=use holding, 1=use last		//  4
	int					NumEpochs; //actual number of epochs used					//  4
	EpochDefinition	EpochDef[MAXNUMEPOCHS]; //epoch definitions			//  2000
	char					FofT[MAXFUNCTIONLENGTH];  // function of time			//128
	PNSubtractConfig	PNSub;	//P-N subtraction configuration					// 20
	CondPulseConfig	CondPulse; // conditioning pulse(s) configuration		// 24
} WaveformDefinition;

#define MAXCOUNTERNAMELENGTH 32
typedef struct PACKAGE tagCounterConfig////////////////////////////////////////////bytes 64
{
   unsigned long		Application;	// defines in nidaqcns.h					//  4
   											// ND_BUFFERED_PERIOD_MSR
                                    // ND_PULSE_TRAIN_GNR
                                    // ND_RETRIG_PULSE_GNR
   char					Name[MAXCOUNTERNAMELENGTH];// name of counter		 	// 32
	unsigned long		OutputPolarity;// ND_POSITIVE or ND_NEGATIVE				//  4
   unsigned long		Source;			// timebase (nidaqcns.h) or other ctr	//  4
	unsigned long		SourcePolarity;// ND_POSITIVE or ND_NEGATIVE				//  4
   unsigned long		Gate;				// (nidaqcns.h) or other ctr				//  4
   unsigned long		GatePolarity;	// ND_POSITIVE or ND_NEGATIVE				//  4
   unsigned long		Delay;			// ND_COUNT_1, t=Delay*Timebase units	//  4
   unsigned long		Duration;		// ND_COUNT_2, t=Duration*Timebase units// 4
} CounterConfig;

#define MAXTAGLENGTH 256
typedef struct PACKAGE tagDIOConfig///////////////////////////////////////////////bytes 512
{
	char					HighText[MAXTAGLENGTH];//text for tagging when high	//256
   char					LowText[MAXTAGLENGTH]; //text for tagging when low		//256
} DIOConfig;

typedef struct PACKAGE tagEventTagStruct//////////////////////////////////////////bytes 272
{
   TagType				TagOrigin;		// user click or program placed?			//  4
   float             TimeStamp;		// number of seconds since acq. start	//  4
   unsigned long 		NumScans;		// number of scans since acq. start		//  4
   unsigned long		AudioByteOffset; // offset to beginning of tag audio	//  4
   char 					Comment[MAXTAGLENGTH];	// text identifying tag			//256
} EventTagStruct;

#define MAXAUTOSAMPNAMELENGTH 32
#define MAXAUTOSAMPVALUES 100
typedef struct PACKAGE tagAutoSampConfig
{
	int					NumVals;			// number of possible values				//  4
   char					Name[MAXAUTOSAMPNAMELENGTH];//name of autosamp value	// 32
   float					Val[MAXAUTOSAMPVALUES];	// possible values				//400
   float					Tel[MAXAUTOSAMPVALUES];	//corresp.telegraphed voltages//400
} AutoSampConfig;

#define MAXINSTRUMENTNAMELENGTH 256
#define MAXDACUNITSNAMELENGTH 32
typedef struct PACKAGE tagInstrumentConfig
{
   char					Name[MAXINSTRUMENTNAMELENGTH];		// name of instrument
   float					ExtCommand;		// units per V at external command,
   											// 0 if unused
   char					Units[MAXDACUNITSNAMELENGTH];		// external command units
   AutoSampConfig		AutoSampled[MAXCHANNELS];// limited to 16 possible
   											// telegraphed outputs, since only 16 ADCs
} InstrumentConfig;

////////////////////////////////////////////////////////////////////////////////
//FILE STRUCTURE
//		Header																			 8192 bytes
//		unsigned long SampSeq[SamplesPerSequence]		SamplesPerSequence *  4 bytes
//		short multiplexdata[NumPoints*NumEpisodes] NumPoints*NumEpisodes*  2 bytes
//		unsigned long EventIntervals[NumEvents]					NumEvents *  4 bytes
//		EventTag TagEvents[NumTagEvents]                   NumTagEvents *272 bytes
//    char AudioData[NumAudioBuffers*AudioBufferSize]
//														NumAudioBuffers*AudioBufferSize  bytes
////////////////////////////////////////////////////////////////////////////////

typedef struct PACKAGE tagSynapFileHeader_v2///////////////////////////////total bytes 8192
{
//File header/////////////////////////////////////////////////////////bytes   28
   int					MagicNumber;														//  4
	float					Ver; 	 //file version (2.0 for this version)         	//  4
   AcquisitionType	AcqType;																//  4
	SYSTEMTIME			Time; //time of acquisition start                  	// 16

//General acq. parameters/////////////////////////////////////////////bytes   72
	float		 			DACHolding[2]; //DAC holding in mV		            	//  8
	unsigned short		ADCResolution; //number of possible ADC values			//  2
												// 4095 for 12bit, 65535 for 16bit
	short             NumADCs; 		//number of A/D channels sampled			//  2
	short		 			NumEventTags; 	//number of events at end of file		//  2
   short					ScanTimebase;	// scan timebase (-3 or 2, E-Series)	//  2
   unsigned short		ScanInterval[2]; 	// interval between scans				//  4
   											// 2 to 65535 timebase units
                                    // float[2] to allow shift in epis. mode
   short					SampleTimebase;// sample timebase (-3 or 2, E-Series)	//  2
	unsigned short		SampleInterval;// interval between samples in a scan	//  2
   											// 2 to 65535 timebase units
   short					MRSamplesPerSequence;  // not yet implemented 			//  2
   short					MRScansPerSequence;    // not yet implemented			//  2
   short					MRSampleRateDivisor[MAXCHANNELS];// not yet implemented//32
	unsigned long		NumScans; //num of scans in acq., or num per episode	//  4
   unsigned long		NumEvents;	// number of timed events						//  4

//ADC parameters//////////////////////////////////////////////////////bytes 1888
	short					SamplingOrder[MAXCHANNELS];//array holding sample order//32
   ADCConfig			ADC[MAXCHANNELS];//array holding adc config. params.// 1856

//Triggered acq. parameters///////////////////////////////////////////bytes   40
	int					TriggerLine; // 0 = no trigger, -1 = HW_Analog, 		//  4
 	 										// user's digital trigger = ND_PFI_0
	int					TriggerADC; //-1 = digital trigger on PFI_0				//  4
   unsigned long		TriggerMode; //from NI-DAQ defines							//  4
   float					TrigLowLevel; //in UserUnits for TriggerADC if HW_Analog//4
   float					TrigHighLevel;//in UserUnits for TriggerADC if HW_Analog//4

//Episodic acq. parameters////////////////////////////////////////////bytes   40
   int 					NumEpisodes; //Number of episodes, =-1 if continuous	//  4
	float					EpisodeTime; //length of each episode, in ms				//  4
	unsigned	long		InterEpisodeDelay; // ms between end of one episode	//  4
			  								 		//  and the start of the next
   int					IEDPrecision; // 0 = guesstimate, no use of timer		//  4
												// 1 = use timer(s)
   unsigned long		SampleRateShiftPoint;	// samplerate shift point		//  4

//DAC Waveform output/////////////////////////////////////////////////bytes 4376
// -- 2 DACs with possible simultaneous output
	int						DACActive[2]; //0=inactive, 1=active					//  8
   WaveformDefinition	WaveformDef[2]; 											//  4368
   unsigned long			StimPeriod; // period for stim wfm output, similar	//  4
   											// to episode length
   float						DACOutputHz; // frequency for waveform output		//  4

//General Purpose Counter outputs/////////////////////////////////////bytes  136
//-- digital (TTL) pulse, and event timing
//both counters may be used simultaneously
   int					CounterActive[2];	//	0 = inactive, 1 = active,        //  8
   												// 2 = gate for other,
                                       // 3 = source for other
   CounterConfig		CounterDef[2];														//128

//Digital I/O lines///////////////////////////////////////////////////bytes 2076
//	DIOConfig			DigLine[8];														//  2048

//Audio tag description///////////////////////////////////////////////bytes   12
// audio saved at end of file 8 bit mono, usually 8kHz sampling
	unsigned long		AudioBufferSize; //size of each audio buffer          //  4
   												// (.25s=2000pts)
   unsigned long		NumAudioBuffers;  // total number of audio buffers		//  4
   int					AudioSampleRateHz;  //sample rate of audio in Hz		//  4

//padding
	char					Unused[156];														//156
//File description/comment
	char		 			Comment[1024]; //file comment								//  1024
} SynapFileHeader_v2;

#pragma pack() //go back to default alignment

};
//#endif //_OLDDAQINUSE_

//---------------------------------------------------------------------------
#endif
