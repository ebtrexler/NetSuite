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

#ifndef RT_BaseH
#define RT_BaseH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8




#define SERIALIZE

#include <typeinfo> // compile with -RT switch for RTTI
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <math.h>

#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <xstring>
#include <sstream>

// tested with Boost libraries version 1_39_0 that ships with RAD Studio C++ 2010
// probably would work with newer libraries, but not tested with Borland's IDE.

#define BOOST_NO_INT64_T  // to avoid int overflow error in collection_traits.hpp
#include <boost/shared_ptr.hpp>


#ifdef SERIALIZE

#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#endif //SERIALIZE


#include <Forms.hpp>


//forward declarations
class TNetwork;
class TCell;
class TElectrode;
class TSynapse;
class TCurrent;

// helper class
//template< class T >
//class VectorContainer
//{
//private:
//   std::vector< T > vt;
//
//public:
//   VectorContainer() : vt() { }
//   VectorContainer(const VectorContainer& copy_)
//   {
//      vt = copy_.vt; // or std::swap
//   }
//   ~VectorContainer() { }
//
//   /* member functions */
//   void push_back(T t)
//   {
//      vt.push_back(t);
//   }
//
//   /* friends */
//   friend std::ostream& operator<<(std::ostream&, const VectorContainer&);
//
//}; // class Container
//
//template< class T > std::ostream& operator<<(std::ostream& os, const VectorContainer< T >& cont)
//{
//   std::copy( cont.vt.begin(),
//   cont.vt.end(),
//   std:stream_iterator< T >(os);//, " ") );
//   return os;
//}

typedef boost::shared_ptr<TCurrent> TCurrentPtr;
typedef std::vector< TCurrent * > TCurrentsArray;
typedef TCurrentsArray::iterator TCurrentsArrayIterator;
typedef TCurrentsArray::const_iterator TCurrentsArrayConstIterator;
typedef std::map< std::wstring, TCurrentPtr > TCurrentsMap;
typedef TCurrentsMap::iterator TCurrentsMapIterator;
typedef TCurrentsMap::const_iterator TCurrentsMapConstIterator;

typedef boost::shared_ptr<TElectrode> TElectrodePtr;
typedef std::vector < TElectrode * > TElectrodesArray;
typedef TElectrodesArray::iterator TElectrodesArrayIterator;
typedef TElectrodesArray::const_iterator TElectrodesArrayConstIterator;
typedef std::map< std::wstring, TElectrodePtr > TElectrodesMap;
typedef TElectrodesMap::iterator TElectrodesMapIterator;
typedef TElectrodesMap::const_iterator TElectrodesMapConstIterator;

typedef boost::shared_ptr<TSynapse> TSynapsePtr;
typedef std::vector < TSynapse * > TSynapsesArray;
typedef TSynapsesArray::iterator TSynapsesArrayIterator;
typedef TSynapsesArray::const_iterator TSynapsesArrayConstIterator;
typedef std::map< std::wstring, TSynapsePtr > TSynapsesMap;
typedef TSynapsesMap::iterator TSynapsesMapIterator;
typedef TSynapsesMap::const_iterator TSynapsesMapConstIterator;

typedef boost::shared_ptr<TCell> TCellPtr;
typedef std::vector < TCell * > TCellsArray;
typedef TCellsArray::iterator TCellsArrayIterator;
typedef TCellsArray::const_iterator TCellsArrayConstIterator;
typedef std::map< std::wstring, TCellPtr > TCellsMap;
typedef TCellsMap::iterator TCellsMapIterator;
typedef TCellsMap::const_iterator TCellsMapConstIterator;

// these are for "non deleting" shared_ptr creation
//struct null_deleter
//{
//    void operator()(void const *) const
//    {
//    }
//};
//
//template <class T>
//boost::shared_ptr<T> createSharedPtr(T *ptrT)
//{
//    boost::shared_ptr<T> px(ptrT, null_deleter());
//    return px;
//}


#ifdef SERIALIZE
BOOST_SERIALIZATION_SHARED_PTR(TCurrentPtr)
BOOST_SERIALIZATION_SHARED_PTR(TElectrodePtr)
BOOST_SERIALIZATION_SHARED_PTR(TSynapsePtr)
BOOST_SERIALIZATION_SHARED_PTR(TCellPtr)
BOOST_SERIALIZATION_SHARED_PTR(TCurrentsMap)
BOOST_SERIALIZATION_SHARED_PTR(TElectrodesMap)
BOOST_SERIALIZATION_SHARED_PTR(TSynapsesMap)
BOOST_SERIALIZATION_SHARED_PTR(TCellsMap)
#endif



////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TRTBase   ***************************
//
//
/**
   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
   @brief Base class for all derived classes used in the network
*/
class TRTBase
{
#ifdef SERIALIZE
///  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
///  Required for serialization and saving networks to disk
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(FName);
      ar & BOOST_SERIALIZATION_NVP(FActive);
      ar & BOOST_SERIALIZATION_NVP(F_X);
      ar & BOOST_SERIALIZATION_NVP(F_Y);
	}
#endif //SERIALIZE

   /// insertion << operator that calls pure virtual method WriteToStream
   friend ostream &operator<<(ostream &stream, const TRTBase &rtbase);
   /// extraction >> operator that calls pure virtual method ReadFromStream
   friend istream &operator>>(istream &stream, TRTBase &rtbase);

private:
   // Fields holding position
   int                           F_X, F_Y;
   // Field holding Name property
	std::wstring					  	FName;
   // Field holding Active property -- if active then include in model
   bool                          FActive;

protected:
   /// pure virtual function for derived classes to write to a stream
   virtual void const __fastcall WriteToStream(ostream &stream) const = 0;
   /// pure virtual function for derived classes to read from a stream
   virtual void const __fastcall ReadFromStream(istream &stream) = 0;

public:
   /// Returns RTTI on class type
	const std::wstring __fastcall ClassType() const;

   /// Returns string used to register class with factory
   /*!
      Users of class factories must also tell the class the key they used
      when registering the class.  See factory.h
   */
  	virtual const std::wstring & __fastcall ClassKey() const = 0;
   #pragma message( "Howdy from RTBase.h" )

   /// Returns name of object (should be unique in network)
  	const std::wstring __fastcall Name() const;

   /// User supplied name of object -- must be unique in network
   void __fastcall               SetName(const std::wstring & name);


   /// Determines if object is included in model
   const bool __fastcall         IsActive() const;
   /// Determines if object is included in model
   void __fastcall               SetActive(bool active);

   /// Position of item on GUI representation of network
   const int __fastcall          GetX() const;
   /// Position of item on GUI representation of network
   void __fastcall               SetX(int x);
   /// Position of item on GUI representation of network
   const int __fastcall          GetY() const;
   /// Position of item on GUI representation of network
   void __fastcall               SetY(int y);

   /// For use with GUI -- tests  X and Y for proximity to stored values
   bool __fastcall               HitTest(int X, int Y, int tol=0) const;

   /// GUI method -- GUI calls this to get the VCL form to show in the GUI.
   /*! This is the only platform dependent method in the network class framework.
       Other implementations should alter this function to match GUI requirements.
   */
   const bool __fastcall         AttachEditFormTo(TComponent *Owner, TWinControl *Parent);

   /// Interface for derived classes, called by GUI to update VCL form components
   virtual void __fastcall       PopulateEditForm() = 0;
   /// Interface for derived classes, called by GUI to read VCL form values
   virtual bool __fastcall       ValidateEditForm() = 0;

   /// pure virtual function for resetting before networking run
   virtual bool __fastcall       Initialize(bool Reset) = 0;

   /// Interface for derived classes, returns pointer to GUI object for editing members.
   /// Callers must cast pointer to the correct object.
   virtual void * const __fastcall GetEditForm() = 0;

   /// Default constructor
   __fastcall TRTBase();
   /// Specialized constructor
   __fastcall TRTBase(const std::wstring & name, const bool active = true);
   /// Copy constructor
   __fastcall TRTBase(const TRTBase &source);
   /// Overloaded assignment operator
   TRTBase& operator = (const TRTBase & source);

   virtual ~TRTBase();
};
//---------------------------------------------------------------------------
#endif
