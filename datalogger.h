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

#ifndef __DATALOGGER_H__
#define __DATALOGGER_H__

#include <sstream>
#include <string>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TDataLogger   ***************************
//
//
/**
   Class to facilitate logging of model variables to a file for analysis
   after a run/simulation

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
   @brief Simulation data logging class
*/
class TDataLogger
{
public:
   TDataLogger();
   virtual ~TDataLogger();
   std::ostringstream& GetStream();
   std::ostringstream& WriteTo(const wchar_t * filename);
   std::ostringstream& Reset();

   template <typename T> friend TDataLogger& operator<<(TDataLogger& logger, const T& t)
   { logger.os << t; return logger; }

   friend TDataLogger& operator<<(TDataLogger& logger, std::ostream& ( *pf )(std::ostream&))
   { logger.os << pf; return logger; }

protected:
   std::ostringstream os;
private:
   TDataLogger(const TDataLogger&);
   TDataLogger& operator =(const TDataLogger&);
};

inline TDataLogger::TDataLogger()
{
}

/// Returns the internal stream
inline std::ostringstream& TDataLogger::GetStream()
{
   return os;
}

inline TDataLogger::~TDataLogger()
{

}

inline std::ostringstream& TDataLogger::WriteTo(const wchar_t * filename)
{
	//fprintf(stderr, "%s", os.str().c_str());
	ofstream ofs(filename);
	ofs << os.str().c_str();
	return os;
}
inline std::ostringstream& TDataLogger::Reset()
{
   os.str("");
	os.clear();
	return os;
}

#endif //__DATALOGGER_H__
