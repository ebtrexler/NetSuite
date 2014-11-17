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

Please direct correspondence to ebtrexler _at_ gothamsci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------


#pragma hdrstop

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GUI_PlaybackWaveform.h"
#include "GUI_PlaybackWaveformForm.h"

// text functions for delimited text files
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	 std::stringstream ss(s);
	 std::string item;
	 while (std::getline(ss, item, delim)) {
		  elems.push_back(item);
	 }
	 return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	 std::vector<std::string> elems;
	 split(s, delim, elems);
	 return elems;
}

void const __fastcall 	TPlaybackWaveform::WriteToStream(ostream &stream) const
{
	int len = FFileName.length();
	const wchar_t* data = FFileName.data();
	stream.write((char*) & len, sizeof(int));
	stream.write((char*) & data, len*sizeof(wchar_t));

	len = FUnits.length();
	data = FUnits.data();
	stream.write((char*) & len, sizeof(int));
	stream.write((char*) & data, len*sizeof(wchar_t));

	stream.write((char *)&FFileType, sizeof(int));
	stream.write((char *)&FScaleFactor, sizeof(double));
	stream.write((char *)&FOffset, sizeof(double));
	stream.write((char *)&FSampleRate, sizeof(double));
	stream.write((char *)&FFileDuration, sizeof(double));
	stream.write((char *)&FWaveformDuration, sizeof(double));
	stream.write((char *)&FPlaybackRate, sizeof(double));
	stream.write((char *)&FSelectedChannel, sizeof(int));
	stream.write((char *)&FTXTTimeColumn, sizeof(int));
	stream.write((char *)&FTXTHeaderRow, sizeof(int));
	stream.write((char *)&FRepeatNumber, sizeof(int));
	stream.write((char *)&FNumEpisodes, sizeof(int));
	stream.write((char *)&FEpisodeSpacing, sizeof(double));
	stream.write((char *)&FConcatenateEpisodes, sizeof(bool));
	stream.write((char *)&FDelayBeforeRepeat, sizeof(double));
}

void const __fastcall 	TPlaybackWaveform::ReadFromStream(istream &stream)
{
	int len;
   stream.read((char*) & len, sizeof(int));
   wchar_t* data = new wchar_t[len];
	stream.read((char*) & data, len*sizeof(wchar_t));
	FFileName.assign(data);
   delete[]data;
   stream.read((char*) & len, sizeof(int));
   data = new wchar_t[len];
	stream.read((char*) & data, len*sizeof(wchar_t));
	FUnits.assign(data);

	stream.read((char *)&FFileType, sizeof(int));
	stream.read((char *)&FScaleFactor, sizeof(double));
	stream.read((char *)&FOffset, sizeof(double));
	stream.read((char *)&FSampleRate, sizeof(double));
	stream.read((char *)&FFileDuration, sizeof(double));
	stream.read((char *)&FWaveformDuration, sizeof(double));
	stream.read((char *)&FPlaybackRate, sizeof(double));
	stream.read((char *)&FSelectedChannel, sizeof(int));
	stream.read((char *)&FTXTTimeColumn, sizeof(int));
	stream.read((char *)&FTXTHeaderRow, sizeof(int));
	stream.read((char *)&FRepeatNumber, sizeof(int));
	stream.read((char *)&FNumEpisodes, sizeof(int));
	stream.read((char *)&FEpisodeSpacing, sizeof(double));
	stream.read((char *)&FConcatenateEpisodes, sizeof(bool));
	stream.read((char *)&FDelayBeforeRepeat, sizeof(double));

}

double __fastcall			TPlaybackWaveform::EvaluateAtNext(double step)
{
	double val = 0;

	ElapsedTime += step;

	if (ElapsedTime < FWaveformDuration){  // during waveform
		val = terp->evaluate(ElapsedTime);
	}
														 // end of waveform
	if (ElapsedTime >= (FWaveformDuration + FDelayBeforeRepeat)){
		if (FRepeatNumber == -1){		// -1 = until run finishes,
			 ElapsedTime = 0;          // reset ElapsedTime so waveform repeats
			 val = terp->evaluate(ElapsedTime);
		}
		else if (FRepeatNumber > 1){	//		or fixed number > 1
			whichrep++;
			if (whichrep <= FRepeatNumber){ // still within repeat count
				ElapsedTime = 0;        // reset ElapsedTime so waveform repeats
				val = terp->evaluate(ElapsedTime);
			}
		}
		else val = 0.0;              // one repeat only; done, so return 0.0
	}

	return val;
}

void* const __fastcall	TPlaybackWaveform::GetEditForm()
{
	static TPlaybackWaveformForm *form = new TPlaybackWaveformForm(NULL);
	form->Waveform = this;
	return form;
}

void __fastcall       	TPlaybackWaveform::PopulateEditForm()
{
	TPlaybackWaveformForm *form = (TPlaybackWaveformForm * )GetEditForm();
	form->Align = alClient;
	form->FileLabel->Caption = ExtractFileName(UnicodeString(FFileName.c_str()));
	form->ChannelComboBox->Clear();
	form->WaveformDurationLabel->Caption = L"";
	form->ConvertGainEdit->Text = FScaleFactor;
	form->OffsetEdit->Text = FOffset;

	if (FRepeatNumber == -1) {  // repeat forever
		form->RepeatsRadioGroup->ItemIndex = 1;
		form->FixedRepeatsEdit->Text = L"-1";
		form->FixedRepeatsEdit->Enabled = false;
	}
	else {
		form->FixedRepeatsEdit->Enabled = true;
		form->FixedRepeatsEdit->Text = FRepeatNumber;
	}

	form->DelayBetweenRepeatsEdit->Text = FloatToStr(FDelayBeforeRepeat);

	form->PlotPanel1->YUnits = UnicodeString(FUnits.c_str());
	form->PlotPanel1->ClearPlots();
	form->PlotPanel1->Invalidate();

	if (FFileName == L"") {
		return;
	}

	form->ChannelComboBox->Clear();
	for (int i=0; i<FNumChans; i++) {
		form->ChannelComboBox->Items->Add(FChannelNames[i].c_str());
	}

	if (FNumChans > 0 && FSelectedChannel < 0) {
		FSelectedChannel = 0;
	}

	if (FSelectedChannel > -1) {
		form->WaveformDurationLabel->Caption = FloatToStr(FFileDuration);
      form->WaveformDurationEdit->Text = FloatToStr(FWaveformDuration);
		form->ChannelComboBox->ItemIndex = FSelectedChannel;
		UpdatePlot();
	}
}

bool __fastcall       	TPlaybackWaveform::ValidateEditForm()
{
	TPlaybackWaveformForm *form = (TPlaybackWaveformForm * )GetEditForm();
	bool ok = true;

	try {
		SetScaleFactor(StrToFloat(form->ConvertGainEdit->Text));
	} catch (EConvertError & e) {
		Application->MessageBox(e.ToString().c_str(), L"Cannot convert Amplitude Gain to float");
		ok = false;
	}

	try {
		SetOffset(StrToFloat(form->OffsetEdit->Text));
	} catch (EConvertError & e) {
		Application->MessageBox(e.ToString().c_str(), L"Cannot convert Amplitude Offset to float");
		ok = false;
	}

	try {
		SetWfmDuration(StrToFloat(form->WaveformDurationEdit->Text));
	} catch (EConvertError & e) {
		Application->MessageBox(e.ToString().c_str(), L"Cannot convert Waveform Duration to float");
		ok = false;
	}

	try {
		int reps = StrToInt(form->FixedRepeatsEdit->Text);
		if (reps < -1 || reps == 0){
			Application->MessageBox(L"Number of repeats must be -1, or >= 1", L"Please correct input");
			ok = false;
		}
		else SetRepeatNumber(reps);
	} catch (EConvertError & e) {
		Application->MessageBox(e.ToString().c_str(), L"Cannot convert number of repeats to int");
		ok = false;
	}

	try {
		double delay = StrToFloat(form->DelayBetweenRepeatsEdit->Text);
		if (delay < 0.0){
			Application->MessageBox(L"Delay between repeats must be greater or equal to zero", L"Please correct input");
			ok = false;
		}
		else SetDelayBeforeRepeat(delay);
	} catch (EConvertError & e) {
		Application->MessageBox(e.ToString().c_str(), L"Cannot convert delay between repeat to float");
		ok = false;
	}

	return ok;
}

/// Opens a file and get available channels, their names, and sample rate
bool __fastcall 			TPlaybackWaveform::GetFileInfo()
{
	bool ok = false;
	switch (FFileType){
		case ABFFile:
			ok = GetFileInfoABF();
			break;
		case Spike2File:
			ok = GetFileInfoSpike2();
			break;
		case TXTFile:
			ok = GetFileInfoTXT();
			break;
		default:
		;
	}
//	if (ok){
//		FPlaybackRate =
//		FFileDuration = numpoints * samplerate
//	}
	return ok;
}

bool __fastcall 			TPlaybackWaveform::GetFileInfoABF()
{
	int Error = 0;
	bool ok = true;
	char fname[256];
	char channame[64];

	if (FFileName == L"") {
		return false;
	}

	try {
		strcpy(fname, AnsiString(FFileName.c_str()).c_str());

		MaxSamps = 2147483648; //2^31  make big request so all points in one episode
		// open new file name
		ok = ABF_ReadOpen(
			fname,
			&FileHandleABF,
			ABF_DATAFILE,
			&FileHeaderABF,
			&MaxSamps,
			&MaxEpis,
			&Error );
		if (!ok){
			throw Exception("OpenABFFileError");
		}
	} catch (...) {
		char szTxt[256];
		ABF_BuildErrorText( Error, fname, szTxt, sizeof(szTxt) );
		UnicodeString msg(szTxt);
		Application->MessageBox(msg.c_str(), L"Error opening...");
		return false;
	}

	// ms in between samples
	FSampleRate = FileHeaderABF.fADCSequenceInterval / 1000.0;

	// channel names
	FNumChans = FileHeaderABF.nADCNumChannels;
	FChannelNames.clear();
	for (int i=0; i < FNumChans; i++) {
		int channum = FileHeaderABF.nADCSamplingSeq[i];
		memset(channame, ' ', 64);
		channame[63] = 0;
		sprintf(channame, "(%d) ", channum);
		memcpy(&channame[4], FileHeaderABF.sADCUnits[channum], ABF_ADCUNITLEN);
		memcpy(&channame[5+ ABF_ADCUNITLEN], FileHeaderABF.sADCChannelName[channum], ABF_ADCNAMELEN);
		FChannelNames.push_back(channame);
	}

	// number of points per channel
//	FNumPoints = FileHeaderABF.lActualAcqLength / FileHeaderABF.nADCNumChannels;
// ebt 1/31/2014 changed to deal with data reduction
	FNumPoints = FileHeaderABF.lActualAcqLength / FileHeaderABF.nADCNumChannels;


	/* TODO -oebt : for now, abf files are the only ones that have episodic recordings */
	FNumEpisodes = FileHeaderABF.lActualEpisodes;
	FEpisodeSpacing = FileHeaderABF.fEpisodeStartToStart * 1000.0; //convert to ms

	ABF_Close(FileHandleABF, NULL);
	FileHandleABF = -1;
	return true;
}

bool __fastcall 			TPlaybackWaveform::GetFileInfoSpike2()
{
	int Error = 0;
	bool ok = true;
	char fname[256];
	short sFH;

	if (FFileName == L"") {
		return false;
	}

	try {
		strcpy(fname, AnsiString(FFileName.c_str()).c_str());
		// open new file name
		sFH = SONOpenOldFile(fname ,1); /* try to open file, 1 for read only */
		if (sFH < 0) {
			throw Exception("SONOpenOldFile error");
		}
		int numchans = SONMaxChans(sFH);
		FNumChans = 0;
		FChannelNames.clear();
		ChannelMap.clear();
		char title[10];
		for (int i=0;i<SONMaxChans(sFH); ++i) /* for each data channel */
		{
			switch (SONChanKind(sFH, i))
			{
				case ChanOff: break; /* do nothing if channel unused */
				case Adc: {
					SONGetChanTitle(sFH, i, title);
					FChannelNames.push_back(title);
					ChannelMap[FNumChans] = i;
					FNumChans++;
				}; break;
				case EventFall: break;
				case EventRise: break;
				case EventBoth: break;
				case Marker: break;
				case AdcMark: break;
				case RealMark: break;
				case TextMark: break;
				case RealWave: {
					SONGetChanTitle(sFH, i, title);
					FChannelNames.push_back(title);
					ChannelMap[FNumChans] = i;
					FNumChans++;
				}; break;
				default: break;
			};
		}
	} catch (Exception &e) {
		Application->MessageBox(e.Message.c_str(), L"Error in GetFileInfoSpike2");
		SONCloseFile(sFH);
		return false;
	}

	// ms in between samples
	double usPerTime = SONGetusPerTime(sFH);
	double dTickLen = SONTimeBase(sFH, 0.0); /* base tick units, seconds */
	// calculate FSampleRate with proper channel
	FSampleRate = SONChanDivide(sFH,FNumChans-1); // number of ticks per sample, for this channel
	FSampleRate *= dTickLen * usPerTime;
	FSampleRate *= 1000.0;  // convert from s to ms

	SONCloseFile(sFH);
	return true;
}

bool __fastcall 			TPlaybackWaveform::GetFileInfoTXT()
{
	char fname[256];
	char channame[256];
	std::string input;
	std::ifstream fin;
	double dtemp;

	// open the file for reading
	strcpy(fname, AnsiString(FFileName.c_str()).c_str());
	try {
		fin.open(fname);
	} catch (Exception &e) {
		Application->MessageBox(e.Message.c_str(), L"File Open Error in GetFileInfoTXT");
		return false;
	}

	// read a line into input
	try {
		getline(fin,input);
	} catch (Exception &e) {
		Application->MessageBox(e.Message.c_str(), L"getline() error in GetFileInfoTXT");
		return false;
	}
	if (!fin) return false; //check for eof

	// how many tab delimited tokens?
	std::vector<std::string> tokens = split(input, '\t');
	int numcols = tokens.size();
	int colindex = 0;
	if (FTXTTimeColumn){
		colindex = 1;
	}
	else {
		colindex = 0;
	}

	// read in or set channel names
	FChannelNames.clear();
	FNumChans = 0;
	for (int i=colindex; i<numcols; i++){
		if (FTXTHeaderRow) {
			FChannelNames.push_back(tokens[i]);
		}
		else {
			sprintf(channame, "Channel #%d", FNumChans);
			FChannelNames.push_back(channame);
		}
		FNumChans++;
	}
	// because reading text files is slow, we only want to do as seldom as possible
	// -- decision to do it here rests on variables and streams already set up
	// fill txtdata vector with all numerical data from file

	try {
   	fin.seekg(0,fin.beg);	// reset file stream to beginning
		if (FTXTHeaderRow){     // read in header if there
			getline(fin,input);
		}
		txtdata.clear();
		while (fin.good()){
			getline(fin,input);
			tokens = split(input, '\t');
			for (int i=0; i<numcols; i++){
				txtdata.push_back(atof(tokens[i].c_str()));
			}
		}   
	} catch (...) {
	}

	// ms in between samples for files with time column
	if (FTXTTimeColumn){
		// get 3 times and get average spacing
		double times[3];
		double rate = 0;
		for (int i=0; i<3; i++){
			times[i] = txtdata[i*numcols];
			if (i>0){
				rate += times[i] - times[i-1];
			}
		}
		FSampleRate = rate / 2.0;
	}

	fin.close();

	return true;
}


/// Opens a file and reads in selected channel
bool __fastcall 			TPlaybackWaveform::ReadChannel()
{
	bool result = false;
	switch (FFileType) {
		case ABFFile:
			result = ReadABFChannel();
			break;
		case Spike2File:
			result = ReadSpike2Channel();
			break;
		case TXTFile:
			result = ReadTXTChannel();
			break;
		default:
		;
	}
	return result;
}


bool __fastcall 			TPlaybackWaveform::ReadABFChannel()
{
	if (FPlotData != NULL) {
		delete[] FPlotData;
		FPlotData = NULL;
	}

	bool ok;
	int channum = FileHeaderABF.nADCSamplingSeq[FSelectedChannel];
	int Error = 0;
	UINT numsamps = 2147483648;  //2^31  make big request so all points in one episode
	FNumPoints = 0;
	char fname[256];
	float * data;

	try {
		if (FPlotData == NULL){
			FPlotData = new double[FileHeaderABF.lActualAcqLength];
		}
		data = new float[FileHeaderABF.lActualAcqLength];
		float *pfBuffer = data;
		strcpy(fname, AnsiString(FFileName.c_str()).c_str());

		MaxSamps = 2147483648; //2^31  make big request so all points in one episode
		// open new file name
		ok = ABF_ReadOpen(
			fname,
			&FileHandleABF,
			ABF_DATAFILE,
			&FileHeaderABF,
			&MaxSamps,
			&MaxEpis,
			&Error );
		if (!ok){
			throw Exception("ABF_ReadOpen Error");
		}
		for (DWORD i = 1; i <= MaxEpis; i++) {
			ok = ABF_ReadChannel(
					FileHandleABF,
					&FileHeaderABF,
					channum,
					i,
					pfBuffer,
					&numsamps,
					&Error);
			if (!ok) {
				throw Exception("ABF_ReadChannel Error");
			}
			pfBuffer += numsamps;
			FNumPoints += numsamps;
		}
		if (FNumPoints != FileHeaderABF.lActualAcqLength / FileHeaderABF.nADCNumChannels) {
			throw Exception("NumPoints mismatch");
		}
		else {
			for (long i = 0; i < FNumPoints; i++) {
				FPlotData[i] = data[i] * FScaleFactor + FOffset;
			}
		}
	} catch (Exception &e) {
		char szTxt[256];
		char *fname = AnsiString(FFileName.c_str()).c_str();
		ABF_BuildErrorText( Error, fname, szTxt, sizeof(szTxt) );
		std::wstring text = L"";
		text = text + e.Message.c_str() + L"\n" + UnicodeString(szTxt).c_str();
		Application->MessageBox(text.c_str(), L"Error reading channel from ABF file.");
		delete[] data;

		return false;
	}

	delete[] data;
	ABF_Close(FileHandleABF, NULL);
	FileHandleABF = -1;

	return true;
}

bool __fastcall 			TPlaybackWaveform::ReadSpike2Channel()
{
	if (FPlotData != NULL) {
		delete[] FPlotData;
		FPlotData = NULL;
	}

	bool ok;
	FNumPoints = 0;
	char fname[256];
	float * data;
	int chan = ChannelMap[FSelectedChannel];
	short sFH;
	long numReads = 0;

	try {
		strcpy(fname, AnsiString(FFileName.c_str()).c_str());

		// open new file name
		sFH = SONOpenOldFile(fname ,1); /* try to open file, 1 for read only */
		if (sFH < 0) {
			throw Exception("SONOpenOldFile error");
		}

		TDataKind kind; //the type of data
		kind = SONChanKind(sFH, chan);
		if ((kind != RealWave) && (kind != Adc)){
			throw Exception("SONChanKind error"); // Check channel type
		}
		const long NBUFSZ = 100000; // The buffer size to use
		float *rBuff = new float[NBUFSZ]; //Buffer to read data into
		long lTime; //time to read from and to
		long lPoints; //number of points
		long nRead; //number of points read

		// first run through to find number of points
		lPoints = 0;// initialise the points
		lTime = 0; // start searching here
		nRead = 1; // so loop works as expected
		numReads = 0;
		while (nRead > 0) {
			nRead = SONGetRealData(sFH, chan, rBuff, NBUFSZ, lTime,
				SONMaxTime(sFH), &lTime, NULL);
			if (nRead > 0) {
				lPoints = lPoints + nRead; // increase count of points read
				lTime = lTime + nRead * SONChanDivide(sFH, chan); // ' next read time
				numReads++;
			}
			else if (nRead < 0 ){  // error on read
				throw Exception("SONGetRealData Error");
			}
		}
		delete[] rBuff;

		// next run through and fill plotdata buffer
		if (FPlotData == NULL){
			FPlotData = new double[lPoints];
		}
		data = new float[numReads * NBUFSZ]; //give room for all calls to SONread
		float *pfBuffer = data;

		FNumPoints = 0;// initialise the points
		lTime = 0; // start searching here
		nRead = 1; // so loop works as expected
		while (nRead > 0) {
			nRead = SONGetRealData(sFH, chan, pfBuffer, NBUFSZ, lTime,
				SONMaxTime(sFH), &lTime, NULL);
			if (nRead > 0) {
				FNumPoints = FNumPoints + nRead; // increase count of points read
				lTime = lTime + nRead * SONChanDivide(sFH, chan); //  next read time
				pfBuffer += nRead;
			}
			else if (nRead < 0 ){  // error on read{
				throw Exception("SONGetRealData Error");
			}
		}

		if (FNumPoints != lPoints) { // the two different read loops don't match??
			throw Exception("NumPoints mismatch");
		}
		else {
			for (long i = 0; i < FNumPoints; i++) {
				FPlotData[i] = data[i] * FScaleFactor + FOffset;
			}
		}
	} catch (Exception &e) {
		Application->MessageBox(e.Message.c_str(), L"Error reading channel from Spike2 file.");
		delete[] data;
		SONCloseFile(sFH);
		return false;
	}

	// ms in between samples
	double usPerTime = SONGetusPerTime(sFH);
	double dTickLen = SONTimeBase(sFH, 0.0); /* base tick units, seconds */
	// calculate FSampleRate with proper channel
	FSampleRate = SONChanDivide(sFH,chan); // number of ticks per sample, for this channel
	FSampleRate *= dTickLen * usPerTime;
	FSampleRate *= 1000.0;  // convert from s to ms

	SONCloseFile(sFH);
	delete[] data;
	return true;
}

bool __fastcall 			TPlaybackWaveform::ReadTXTChannel()
{
	// demultiplex from txtdata vector
	int numcols = FNumChans;
	int colindex = FSelectedChannel;
	FNumPoints = 0;

	if (FTXTTimeColumn) {
		numcols = FNumChans + 1;
		colindex = FSelectedChannel + 1;
	}

	// how many data points per channel
	FNumPoints = txtdata.size() / numcols - 1;  /* TODO : why need subtract one here? */

	if (FPlotData != NULL) {
		delete[] FPlotData;
		FPlotData = NULL;
	}
	FPlotData = new double[FNumPoints];

	std::vector<double>::iterator it = txtdata.begin();
	it += colindex;

	for (int i = 0; i < FNumPoints; i++){
		FPlotData[i] = (*it) * FScaleFactor + FOffset;
		it += numcols;
	}
	return true;
}

void __fastcall 			TPlaybackWaveform::UpdatePlot()
{
	TPlaybackWaveformForm *form = (TPlaybackWaveformForm * )GetEditForm();

	// open file and read in channel
	if (ReadChannel()) {
		form->PlotPanel1->ClearPlots();
//		if (DataPlot != NULL) {      // ebt 6-22-13 - turns out ClearPlots deletes plots
//			delete DataPlot;          // if we do it here, exception occurs.
//			DataPlot = NULL;
//		}

		form->PlotPanel1->BeginUpdate();

		for (int i=0; i < FNumEpisodes; i++) {
			TStaticFunctionPlot *DataPlot;
			int numpts = FNumPoints/FNumEpisodes;
			DataPlot = new TStaticFunctionPlot(form->PlotPanel1,
															i, "",
															clRed, i*FEpisodeSpacing,
															FPlaybackRate,
															numpts,
															&FPlotData[i*numpts]);
			form->PlotPanel1->AddPlot(DataPlot, false);
		}
		form->PlotPanel1->ConcatenatePlots = FConcatenateEpisodes;

		form->PlotPanel1->ScalePlots(true);

		form->PlotPanel1->EndUpdate();
	}

}

__fastcall					TPlaybackWaveform::TPlaybackWaveform() : TRTBase(),
	FUnits(L""), FFileType(0), FScaleFactor(1), FOffset(0), FSampleRate(1),
	FFileDuration(0), FWaveformDuration(0), FPlaybackRate(1), FileHandleABF(1),
	FSelectedChannel(-1), FTXTTimeColumn(false), FTXTHeaderRow(false),
	FRepeatNumber(1), FNumEpisodes(1), FEpisodeSpacing(0.0),
	FConcatenateEpisodes(false), FDelayBeforeRepeat(0.0),
	FPlotData(NULL), FNumPoints(0), FNumChans(0),
	terp(NULL), ElapsedTime(0)
{
}

__fastcall 					TPlaybackWaveform::TPlaybackWaveform(const std::wstring & name) : TRTBase(name),
	FUnits(L""), FFileType(0), FScaleFactor(1), FOffset(0), FSampleRate(1),
	FFileDuration(0), FWaveformDuration(0), FPlaybackRate(1), FileHandleABF(1),
	FSelectedChannel(-1), FTXTTimeColumn(false), FTXTHeaderRow(false),
	FRepeatNumber(1), FNumEpisodes(1), FEpisodeSpacing(0.0),
	FConcatenateEpisodes(false), FDelayBeforeRepeat(0.0),
	FPlotData(NULL), FNumPoints(0), FNumChans(0),
	terp(NULL), ElapsedTime(0)
{
}

TPlaybackWaveform::~TPlaybackWaveform()
{
	if (FPlotData != NULL) {
		delete[] FPlotData;
	}
	if (terp) {
		delete terp;
	}
}

void __fastcall         TPlaybackWaveform::SetFileName(std::wstring fname)
{
	FFileName = fname;
	UnicodeString ext(ExtractFileExt(FFileName.c_str()));
	if (ext.CompareIC(UnicodeString(L".abf")) == 0) {
		FFileType = ABFFile;
	}
	else if (ext.CompareIC(UnicodeString(L".dat")) == 0) {
		FFileType = ABFFile;
	}
	else if (ext.CompareIC(UnicodeString(L".smr")) == 0) {
		FFileType = Spike2File;
	}
	else if (ext.CompareIC(UnicodeString(L".txt")) == 0) {
		FFileType = TXTFile;
	}
   // open file and get header info
	if (!GetFileInfo()) return;

	if (FNumChans > 0) {
		FSelectedChannel = 0;
	}
	// read a channel to get number NumPoints and FSampleRate if not found in GetFileInfo()
	if (!ReadChannel()) return;

	// File and waveform duration are the same until user changes
	FFileDuration = FNumPoints * FSampleRate;
	FWaveformDuration = FFileDuration;
	FPlaybackRate = FSampleRate;

	PopulateEditForm();
}

void __fastcall         TPlaybackWaveform::SetChannel(int channel)
{
	FSelectedChannel = channel; // no error checking yet
	UpdatePlot();
}

void __fastcall         TPlaybackWaveform::SetScaleFactor(double factor)
{
	FScaleFactor = factor;
	UpdatePlot();
}

void __fastcall			TPlaybackWaveform::SetUnits(std::wstring units)
{
	FUnits = units;
	TPlaybackWaveformForm *form = (TPlaybackWaveformForm * )GetEditForm();
	form->ConversionLabel->Caption = UnicodeString(FUnits.c_str());
	form->PlotPanel1->YUnits = UnicodeString(FUnits.c_str());
}

void __fastcall			TPlaybackWaveform::SetOffset(double offset)
{
	FOffset = offset;
	UpdatePlot();
}

void __fastcall			TPlaybackWaveform::SetSampleRate(double rate)
{
	FSampleRate = rate;
	UpdatePlot();
}

void __fastcall			TPlaybackWaveform::SetWfmDuration(double time)
{
   if (time > 0) {
      FWaveformDuration = time;
      double timegain = FWaveformDuration / FFileDuration;
		FPlaybackRate = FSampleRate * timegain;
   }
	else FPlaybackRate = FSampleRate;

	UpdatePlot();
}

double __fastcall			TPlaybackWaveform::GetFileDuration()
{
	return FFileDuration;
}

double __fastcall			TPlaybackWaveform::GetPlaybackRate()
{
   return FPlaybackRate;
}

int __fastcall				TPlaybackWaveform::GetNumEpisodes()
{
	return FNumEpisodes;
}

double __fastcall			TPlaybackWaveform::GetEpisodeSpacing()
{
	return FEpisodeSpacing;
}

const std::vector<std::string> & __fastcall
								TPlaybackWaveform::GetChannelNames()
{
	return FChannelNames;
}

const std::wstring & __fastcall TPlaybackWaveform::GetFileName()
{
   return FFileName;
}

long __fastcall			TPlaybackWaveform::GetNumPoints()
{
	return FNumPoints;
}

int __fastcall				TPlaybackWaveform::GetNumChans()
{
	return FNumChans;
}

bool __fastcall			TPlaybackWaveform::GetChannelData(int chan, std::vector<double> &chandata)
{
	chandata.clear();

	if (FNumPoints == 0){
		throw ("File not open yet");
		return false;
	}
	if (chan >= 0 && chan < FNumChans) {
		int oldchan = FSelectedChannel;
		FSelectedChannel = chan;
		if (!ReadChannel()) return false;
		chandata.reserve(FNumPoints);
		double *dataptr = FPlotData;
		for (long i=0; i < FNumPoints; i++) {
			chandata.push_back(*dataptr++);
		}
		FSelectedChannel = oldchan;
	}

	return true;
}

void __fastcall			TPlaybackWaveform::SetConcatenateEpisodes(bool set)
{
	FConcatenateEpisodes = set;
	UpdatePlot();
}

void __fastcall			TPlaybackWaveform::SetTXTTimeColumn(bool set)
{
	FTXTTimeColumn = set;
//	UpdatePlot();
}

void __fastcall			TPlaybackWaveform::SetTXTHeaderRow(bool set)
{
	FTXTHeaderRow = set;
//	UpdatePlot();
}

void __fastcall			TPlaybackWaveform::SetRepeatNumber(int num)
{
	FRepeatNumber = num;
}

void __fastcall			TPlaybackWaveform::SetDelayBeforeRepeat(double delay)
{
	if (delay >= 0.0) FDelayBeforeRepeat = delay;
}

bool __fastcall         TPlaybackWaveform::Initialize(bool Reset)
{
	// make sure plotdata array is up to date
	if (!ReadChannel()) {
		return false;
	}

	ElapsedTime = 0.0;
	whichrep = 1;

	if (terp != NULL) {
		delete terp;
	}

	terp = new MonotCubicInterpolator(FPlaybackRate, FPlotData, FNumPoints);

	return true;
}

// implement pure virtual
#define TPlaybackWaveform_KEY L"Playback Waveform"
const std::wstring & __fastcall TPlaybackWaveform::ClassKey() const
{
	static std::wstring Key(TPlaybackWaveform_KEY);
	return Key;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
