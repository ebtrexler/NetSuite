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


// ---------------------------------------------------------------------------

// 02-09-2011 original

// this code borrowed from user phalanger on
// http://www.codeproject.com/KB/architecture/all_kinds_of_factories.aspx?msg=437918#xx437918xx
// which was submitted as part of a discussion thread on the article:
// http://www.codeproject.com/KB/architecture/all_kinds_of_factories.aspx
// from user Patje
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


// 02-15-2011 added GetKeys() function to return a vector of keys (taken from
//  papafactory       http://sourceforge.net/projects/papafactory/ )

// 02-28 thru 03-01-2011 added factories for 1, 2, and 3 param constructor classes

#ifndef factory_H
#define factory_H

#include <map>
#include <vector>
#include <functional>
#include <exception>
#include <stdexcept>

template<class TSrcType>
class TypeID {
public:
   typedef TSrcType objTypeId;
};

//===========================================================================
//===========================================================================
//===========================================================================
//
// Zero parameters in constructor
//
//===========================================================================
//===========================================================================
//===========================================================================
template < class TKeyType, class TBaseType>
class TObjFactory
{
public:
   typedef TBaseType * value_type;

   TObjFactory(void) {
   }

   ~TObjFactory(void) {
      typename typeMapKeyToBuilder::iterator it(m_mapKeyToBuilder.begin()),
      itend(m_mapKeyToBuilder.end());
      for (; it != itend; ++it)
         delete it->second;
   }

   template<typename TSubType>
   void registerBuilder(const TKeyType & key, TypeID<TSubType>obj) {
      typedef typename TypeID<TSubType>::objTypeId srcType;
      typename typeMapKeyToBuilder::iterator it = m_mapKeyToBuilder.find(key);
      if (it != m_mapKeyToBuilder.end())
         throw std::runtime_error("duplicate");
      m_mapKeyToBuilder[key] = new TObjBuilder<srcType>();
   }

   value_type buildObj(const TKeyType & key) {
      typename typeMapKeyToBuilder::iterator it = m_mapKeyToBuilder.find(key);
      if (it == m_mapKeyToBuilder.end())
         throw std::runtime_error("not found");
      return it->second->buildObject();
   }

   // added 02-15-2011
   std::vector<TKeyType>GetKeys() {
      std::vector<TKeyType>theVector;
      typedef typename typeMapKeyToBuilder::const_iterator const_iterator;
      const_iterator theEnd(m_mapKeyToBuilder.end());
      for (const_iterator theIterator(m_mapKeyToBuilder.begin());
         theIterator != theEnd; ++theIterator) {
         theVector.push_back(theIterator->first);
      }

      return theVector;
   }

protected:
   class TObjBuilderBase {
   public:
      virtual value_type buildObject() = 0;
   };

   template<class TSubType>
   class TObjBuilder : public TObjBuilderBase {
   public:
      virtual value_type buildObject() {
            return new TSubType();
      }
   };

   typedef TObjBuilderBase * typeBuilderPtr;

   typedef std::map<TKeyType, typeBuilderPtr>typeMapKeyToBuilder;

   typeMapKeyToBuilder m_mapKeyToBuilder;
};

//===========================================================================
//===========================================================================
//===========================================================================
//
// One parameter in constructor
//
//===========================================================================
//===========================================================================
//===========================================================================

template < class TKeyType, class TBaseType, class TParamType>
class TObjFactory1P
{
public:
   typedef TBaseType * value_type;

   TObjFactory1P(void) {
   }

   ~TObjFactory1P(void) {
      typename typeMapKeyToBuilder1P::iterator it(m_mapKeyToBuilder1P.begin()),
      itend(m_mapKeyToBuilder1P.end());
      for (; it != itend; ++it)
         delete it->second;
   }

   template<typename TSubType, typename TParam1Type>
   void registerBuilder(const TKeyType & key, TypeID<TSubType>obj, TypeID<TParam1Type>param) {
      typedef typename TypeID<TSubType>::objTypeId srcType;
      typedef typename TypeID<TParam1Type>::objTypeId srcParamType;
      typename typeMapKeyToBuilder1P::iterator it = m_mapKeyToBuilder1P.find(key);
      if (it != m_mapKeyToBuilder1P.end())
         throw std::runtime_error("duplicate");
      m_mapKeyToBuilder1P[key] = new TObjBuilder1P<srcType, srcParamType>();
   }

   value_type buildObj(const TKeyType & key, TParamType &param) {
      typename typeMapKeyToBuilder1P::iterator it = m_mapKeyToBuilder1P.find(key);
      if (it == m_mapKeyToBuilder1P.end())
         throw std::runtime_error("not found");
      return it->second->buildObject(param);
   }

   // added 02-15-2011
   std::vector<TKeyType>GetKeys() {
      std::vector<TKeyType>theVector;
      typedef typename typeMapKeyToBuilder1P::const_iterator const_iterator;
      const_iterator theEnd(m_mapKeyToBuilder1P.end());
      for (const_iterator theIterator(m_mapKeyToBuilder1P.begin());
         theIterator != theEnd; ++theIterator) {
         theVector.push_back(theIterator->first);
      }

      return theVector;
   }

protected:
//   template<class TParamType>
   class TObjBuilder1PBase {
   public:
      virtual value_type buildObject(TParamType &param) = 0;
   };

   template<class TSubType, class TParam1>
   class TObjBuilder1P : public TObjBuilder1PBase {
   public:
      virtual value_type buildObject(TParamType &param) {
            return new TSubType(param);
      }
   };

   typedef TObjBuilder1PBase * typeBuilder1PPtr;

   typedef std::map<TKeyType, typeBuilder1PPtr> typeMapKeyToBuilder1P;

   typeMapKeyToBuilder1P m_mapKeyToBuilder1P;
};



//===========================================================================
//===========================================================================
//===========================================================================
//
// Two parameters in constructor
//
//===========================================================================
//===========================================================================
//===========================================================================

template < class TKeyType, class TBaseType, class TParam1Type, class TParam2Type>
class TObjFactory2P
{
public:
   typedef TBaseType * value_type;

   TObjFactory2P(void) {
   }

   ~TObjFactory2P(void) {
      typename typeMapKeyToBuilder2P::iterator it(m_mapKeyToBuilder2P.begin()),
      itend(m_mapKeyToBuilder2P.end());
      for (; it != itend; ++it)
         delete it->second;
   }

   template<typename TSubType, typename TArg1Type, typename TArg2Type>
   void registerBuilder(const TKeyType & key, TypeID<TSubType>obj, TypeID<TArg1Type>param1, TypeID<TArg2Type>param2) {
      typedef typename TypeID<TSubType>::objTypeId srcType;
      typedef typename TypeID<TArg1Type>::objTypeId srcParam1Type;
      typedef typename TypeID<TArg2Type>::objTypeId srcParam2Type;
      typename typeMapKeyToBuilder2P::iterator it = m_mapKeyToBuilder2P.find(key);
      if (it != m_mapKeyToBuilder2P.end())
         throw std::runtime_error("duplicate");
      m_mapKeyToBuilder2P[key] = new TObjBuilder2P<srcType, srcParam1Type, srcParam2Type>();
   }

   value_type buildObj(const TKeyType & key, TParam1Type &param1, TParam2Type &param2) {
      typename typeMapKeyToBuilder2P::iterator it = m_mapKeyToBuilder2P.find(key);
      if (it == m_mapKeyToBuilder2P.end())
         throw std::runtime_error("not found");
      return it->second->buildObject(param1, param2);
   }

   // added 02-15-2011
   std::vector<TKeyType>GetKeys() {
      std::vector<TKeyType>theVector;
      typedef typename typeMapKeyToBuilder2P::const_iterator const_iterator;
      const_iterator theEnd(m_mapKeyToBuilder2P.end());
      for (const_iterator theIterator(m_mapKeyToBuilder2P.begin());
         theIterator != theEnd; ++theIterator) {
         theVector.push_back(theIterator->first);
      }

      return theVector;
   }

protected:
   class TObjBuilder2PBase {
   public:
      virtual value_type buildObject(TParam1Type &param1, TParam2Type &param2) = 0;
   };

   template<class TSubType, class TParam1, class TParam2>
   class TObjBuilder2P : public TObjBuilder2PBase {
   public:
      virtual value_type buildObject(TParam1Type &param1, TParam2Type &param2) {
            return new TSubType(param1, param2);
      }
   };

   typedef TObjBuilder2PBase * typeBuilder2PPtr;

   typedef std::map<TKeyType, typeBuilder2PPtr> typeMapKeyToBuilder2P;

   typeMapKeyToBuilder2P m_mapKeyToBuilder2P;

};

//===========================================================================
//===========================================================================
//===========================================================================
//
// Three parameters in constructor
//
//===========================================================================
//===========================================================================
//===========================================================================

template <  class TKeyType,
            class TBaseType,
            class TParam1Type,
            class TParam2Type,
            class TParam3Type>
class TObjFactory3P
{
public:
   typedef TBaseType * value_type;

   TObjFactory3P(void) {
   }

   ~TObjFactory3P(void) {
      typename typeMapKeyToBuilder3P::iterator it(m_mapKeyToBuilder3P.begin()),
      itend(m_mapKeyToBuilder3P.end());
      for (; it != itend; ++it)
         delete it->second;
   }

   template<typename TSubType,
            typename TArg1Type,
            typename TArg2Type,
            typename TArg3Type>
   void registerBuilder(const TKeyType & key,
                        TypeID<TSubType>obj,
                        TypeID<TArg1Type>param1,
                        TypeID<TArg2Type>param2,
                        TypeID<TArg3Type>param3)
   {
      typedef typename TypeID<TSubType>::objTypeId srcType;
      typedef typename TypeID<TArg1Type>::objTypeId srcParam1Type;
      typedef typename TypeID<TArg2Type>::objTypeId srcParam2Type;
      typedef typename TypeID<TArg3Type>::objTypeId srcParam3Type;
      typename typeMapKeyToBuilder3P::iterator it = m_mapKeyToBuilder3P.find(key);
      if (it != m_mapKeyToBuilder3P.end())
         throw std::runtime_error("duplicate");
      m_mapKeyToBuilder3P[key] =
         new TObjBuilder3P<srcType,
                           srcParam1Type,
                           srcParam2Type,
                           srcParam3Type>();
   }

   value_type buildObj(const TKeyType & key,
                        TParam1Type &param1,
                        TParam2Type &param2,
                        TParam3Type &param3) {
      typename typeMapKeyToBuilder3P::iterator it = m_mapKeyToBuilder3P.find(key);
      if (it == m_mapKeyToBuilder3P.end())
         throw std::runtime_error("not found");
      return it->second->buildObject(param1, param2, param3);
   }

   // added 02-15-2011
   std::vector<TKeyType>GetKeys() {
      std::vector<TKeyType>theVector;
      typedef typename typeMapKeyToBuilder3P::const_iterator const_iterator;
      const_iterator theEnd(m_mapKeyToBuilder3P.end());
      for (const_iterator theIterator(m_mapKeyToBuilder3P.begin());
         theIterator != theEnd; ++theIterator) {
         theVector.push_back(theIterator->first);
      }

      return theVector;
   }

protected:
   class TObjBuilder3PBase {
   public:
      virtual value_type buildObject(TParam1Type &param1, TParam2Type &param2, TParam3Type &param3) = 0;
   };

   template<class TSubType, class TParam1, class TParam2, class TParam3>
   class TObjBuilder3P : public TObjBuilder3PBase {
   public:
      virtual value_type buildObject(TParam1Type &param1, TParam2Type &param2, TParam3Type &param3) {
            return new TSubType(param1, param2, param3);
      }
   };

   typedef TObjBuilder3PBase * typeBuilder3PPtr;

   typedef std::map<TKeyType, typeBuilder3PPtr> typeMapKeyToBuilder3P;

   typeMapKeyToBuilder3P m_mapKeyToBuilder3P;

};
// ---------------------------------------------------------------------------
#endif
