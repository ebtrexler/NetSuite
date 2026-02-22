/* /////////////////////////////////////////////////////////////////////////////
NetSuite: NetClamp/NetSim/NetFit Neural Network Modeling and Dynamic Clamp Software
	 Copyright (C) 2011-2014  E. Brady Trexler, Ph.D.

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

Please direct correspondence to ebtrexler _at_ hudsonvalleysci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------

#ifndef GUI_PlaybackWaveformH
#define GUI_PlaybackWaveformH

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TPlaybackWaveform methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#include "RT_Base.h"


// include RationalApproximationRealNumber RARN
#include "RARN.h"
#include "MonotCubicInterpolator.h"

//3rd party file support
#include "son.h"
#include "abffiles.h"

#include "PlotPanel.h"


//-----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TPlaybackWaveform   ***************************
//
//
/// Derived class that loads and stores pre-recorded waveforms
/*!
	Uses the MonotonicCubicInterpolator class to generate unevenly spaced
	points if necessary. Text file import methods assume 8 bit ASCI data.

	@author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TPlaybackWaveform : public TRTBase
{
#ifdef SERIALIZE
///  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<TRTBase>(*this);
		ar & BOOST_SERIALIZATION_NVP(FFileName);
		ar & BOOST_SERIALIZATION_NVP(FUnits);
		ar & BOOST_SERIALIZATION_NVP(FFileType);
		ar & BOOST_SERIALIZATION_NVP(FScaleFactor);
		ar & BOOST_SERIALIZATION_NVP(FOffset);
		ar & BOOST_SERIALIZATION_NVP(FSampleRate);
		ar & BOOST_SERIALIZATION_NVP(FFileDuration);
		ar & BOOST_SERIALIZATION_NVP(FWaveformDuration);
		ar & BOOST_SERIALIZATION_NVP(FPlaybackRate);
		ar & BOOST_SERIALIZATION_NVP(FSelectedChannel);
		ar & BOOST_SERIALIZATION_NVP(FTXTTimeColumn);
		ar & BOOST_SERIALIZATION_NVP(FTXTHeaderRow);
		ar & BOOST_SERIALIZATION_NVP(FRepeatNumber);

	  if (version == 2) {
		ar & BOOST_SERIALIZATION_NVP(FNumEpisodes);
		ar & BOOST_SERIALIZATION_NVP(FEpisodeSpacing);
		ar & BOOST_SERIALIZATION_NVP(FConcatenateEpisodes);
	  }

	  if (version == 3) {
		ar & BOOST_SERIALIZATION_NVP(FDelayBeforeRepeat);
	  }

	  // now that all data is in, try to load in the waveform data and info
	  if (!GetFileInfo()) {
		throw Exception(L"Error in serialization-load.  Perhaps waveform data file has moved");
	  }
	}

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & boost::serialization::base_object<TRTBase>(*this);
		ar & BOOST_SERIALIZATION_NVP(FFileName);
		ar & BOOST_SERIALIZATION_NVP(FUnits);
		ar & BOOST_SERIALIZATION_NVP(FFileType);
		ar & BOOST_SERIALIZATION_NVP(FScaleFactor);
		ar & BOOST_SERIALIZATION_NVP(FOffset);
		ar & BOOST_SERIALIZATION_NVP(FSampleRate);
		ar & BOOST_SERIALIZATION_NVP(FFileDuration);
		ar & BOOST_SERIALIZATION_NVP(FWaveformDuration);
		ar & BOOST_SERIALIZATION_NVP(FPlaybackRate);
		ar & BOOST_SERIALIZATION_NVP(FSelectedChannel);
		ar & BOOST_SERIALIZATION_NVP(FTXTTimeColumn);
		ar & BOOST_SERIALIZATION_NVP(FTXTHeaderRow);
		ar & BOOST_SERIALIZATION_NVP(FRepeatNumber);
//	  if (version == 2) {
		ar & BOOST_SERIALIZATION_NVP(FNumEpisodes);
		ar & BOOST_SERIALIZATION_NVP(FEpisodeSpacing);
		ar & BOOST_SERIALIZATION_NVP(FConcatenateEpisodes);
//	  if (version == 3) {
		ar & BOOST_SERIALIZATION_NVP(FDelayBeforeRepeat);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif

private:
	// fields
	std::wstring                  FFileName; 			// full path of file
	std::wstring						FUnits;    			// usually mV or nA
	int                           FFileType; 			// abf, smr, or txt
	double                        FScaleFactor;  	// gain -- amplitude
	double								FOffset;				// offset -- amplitude
	double								FSampleRate;		// original sample rate (ms)
	double								FFileDuration;		// length of file
	double								FWaveformDuration;// altered length of waveform
	double								FPlaybackRate;		// altered sample rate (ms)
	int                           FSelectedChannel; // which chan of multichan file
	bool									FTXTTimeColumn;   // is 1st column timebase?
	bool									FTXTHeaderRow;    // is 1st row header with chan names?
	int									FRepeatNumber;		// -1 = until run finishes,
																	//		or fixed number >= 1

	// new in version 3
	double								FDelayBeforeRepeat;//time before waveform restarts

	// not streamed
	double 								*FPlotData;
	long 									FNumPoints;
	int 									FNumChans;
	std::vector<std::string> 		FChannelNames;

	// constants
   static int const              Spike2File = 100;
	static int const              ABFFile = 101;
	static int const					TXTFile = 102;

   // other vars

	// common to all file types
	double ElapsedTime;
	int whichrep;
	MonotCubicInterpolator *terp;

	// for ABF files
   int FileHandleABF;
	ABFFileHeader FileHeaderABF;
	UINT MaxSamps;
	DWORD MaxEpis;
		// new in version 2  - to handle episodic ABF data from pClamp
	int									FNumEpisodes;		// 1 = continuous, >1 means segmented
	double								FEpisodeSpacing;	// time between episode starts (ms)
	bool									FConcatenateEpisodes; // overlapped (false) or concatenated (true)

   // for Spike2 files
	std::map<int, int> ChannelMap;


   // for TXT files
   std::vector<double> txtdata;
protected:

	// implement pure virtual
	virtual void const __fastcall WriteToStream(ostream &stream) const;
	// implement pure virtual
	virtual void const __fastcall ReadFromStream(istream &stream);

	/// Opens a file and get available channels and sample rate
	bool __fastcall GetFileInfo();

	bool __fastcall GetFileInfoABF();
	bool __fastcall GetFileInfoSpike2();
	bool __fastcall GetFileInfoTXT();

   /// Opens a file and reads in selected channel
	bool __fastcall ReadChannel();

	bool __fastcall ReadABFChannel();
	bool __fastcall ReadSpike2Channel();
	bool __fastcall ReadTXTChannel();

public:
	__fastcall TPlaybackWaveform();
	__fastcall TPlaybackWaveform(const std::wstring & name);
	~TPlaybackWaveform();

	// Updates the plot based on current file and form settings
	void __fastcall UpdatePlot();

	// Sets the filename and calls PopulateEditForm()
	void __fastcall               SetFileName(std::wstring fname);

	// Sets the channel to use and calls UpdatePlot()
	void __fastcall               SetChannel(int channel);

	// Changes the amplitude gain and calls UpdatePlot()
	void __fastcall               SetScaleFactor(double factor);

	// Changes the amplitude offset and calles UpdatePlot()
	void __fastcall					SetOffset(double offset);

	// Changes the sample duration and calles UpdatePlot()
	void __fastcall					SetSampleRate(double rate);

	// Changes the waveform duration and calles UpdatePlot()
	void __fastcall					SetWfmDuration(double time);

	// Changes the units and updates the form appropriately
	void __fastcall					SetUnits(std::wstring units);

	// Sets whether text reading function should use first column as time
	void __fastcall					SetTXTTimeColumn(bool set);

	// Sets whether text reading function should use first row as header
	void __fastcall					SetTXTHeaderRow(bool set);

	// Sets the number of repeats used for waveform generation
	void __fastcall					SetRepeatNumber(int num);

	// Sets the display style - overlapped episodes or concatenated
	void __fastcall					SetConcatenateEpisodes(bool set);

	// Sets the interval before waveform restarts for repeats
	void __fastcall					SetDelayBeforeRepeat(double delay);

	//	Called by owner during updates
	double __fastcall					EvaluateAtNext(double step);

	// Gets the file duration for display or calculations
	double __fastcall					GetFileDuration();

	// Gets the chosen/altered sample rate
	double __fastcall					GetPlaybackRate();

	// Gets the number of episodes
	int __fastcall						GetNumEpisodes();

	// Gets the interval between episode starts
	double __fastcall					GetEpisodeSpacing();

	// Get vector of channel names
	const std::vector<std::string> & __fastcall
											GetChannelNames();

	const std::wstring & __fastcall
											GetFileName();

	// Get number of points in channel
	long __fastcall					GetNumPoints();

	// Get number of channels
	int __fastcall						GetNumChans();

	// Get channel data
	bool __fastcall               GetChannelData(int chan, std::vector<double> &chandata);

	// implement pure virtual
	bool __fastcall               Initialize(bool Reset);

	// implement pure virtual
	const std::wstring & __fastcall ClassKey() const;

	// implement pure virtual
	virtual void* const __fastcall  GetEditForm();
	// implement pure virtual
	virtual void __fastcall       PopulateEditForm();
	// implement pure virtual
	virtual bool __fastcall       ValidateEditForm();

	// virtual functions overridden

};

BOOST_CLASS_VERSION(TPlaybackWaveform, 3) //added 05/16/2014 for inter waveform delay

//---------------------------------------------------------------------------
#endif
