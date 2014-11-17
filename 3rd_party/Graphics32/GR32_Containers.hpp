// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_containers.pas' rev: 21.00

#ifndef Gr32_containersHPP
#define Gr32_containersHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Rtlconsts.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32_lowlevel.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Typinfo.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_containers
{
//-- type declarations -------------------------------------------------------
typedef void * *PPItem;

typedef void * PItem;

typedef void * *PPData;

typedef void * PData;

struct TPointerBucketItem;
typedef TPointerBucketItem *PPointerBucketItem;

struct TPointerBucketItem
{
	
public:
	void *Item;
	void *Data;
};


typedef DynamicArray<TPointerBucketItem> TPointerBucketItemArray;

struct TPointerBucket
{
	
public:
	int Count;
	TPointerBucketItemArray Items;
};


typedef StaticArray<TPointerBucket, 256> TPointerBucketArray;

class DELPHICLASS TPointerMap;
class PASCALIMPLEMENTATION TPointerMap : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	void * operator[](void * Item) { return Data[Item]; }
	
private:
	TPointerBucketArray FBuckets;
	int FCount;
	
protected:
	void * __fastcall GetData(void * Item);
	void __fastcall SetData(void * Item, const void * Data);
	bool __fastcall Exists(void * Item, /* out */ int &BucketIndex, /* out */ int &ItemIndex);
	virtual void * __fastcall Delete(int BucketIndex, int ItemIndex);
	
public:
	__fastcall virtual ~TPointerMap(void);
	PPData __fastcall Add(void * NewItem)/* overload */;
	PPData __fastcall Add(void * NewItem, /* out */ bool &IsNew)/* overload */;
	PPData __fastcall Add(void * NewItem, void * NewData)/* overload */;
	PPData __fastcall Add(void * NewItem, void * NewData, /* out */ bool &IsNew)/* overload */;
	void * __fastcall Remove(void * Item);
	void __fastcall Clear(void);
	bool __fastcall Contains(void * Item);
	bool __fastcall Find(void * Item, /* out */ PPData &Data);
	__property void * Data[void * Item] = {read=GetData, write=SetData/*, default*/};
	__property int Count = {read=FCount, nodefault};
public:
	/* TObject.Create */ inline __fastcall TPointerMap(void) : System::TObject() { }
	
};


class DELPHICLASS TPointerMapIterator;
class PASCALIMPLEMENTATION TPointerMapIterator : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	TPointerMap* FSrcPointerMap;
	void *FItem;
	void *FData;
	int FCurBucketIndex;
	int FCurItemIndex;
	
public:
	__fastcall TPointerMapIterator(TPointerMap* SrcPointerMap);
	bool __fastcall Next(void);
	__property void * Item = {read=FItem};
	__property void * Data = {read=FData};
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TPointerMapIterator(void) { }
	
};


typedef StaticArray<Types::TRect, 67108863> TPolyRects;

typedef TPolyRects *PPolyRects;

class DELPHICLASS TRectList;
class PASCALIMPLEMENTATION TRectList : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	Types::PRect operator[](int Index) { return Items[Index]; }
	
private:
	TPolyRects *FList;
	int FCount;
	int FCapacity;
	
protected:
	Types::PRect __fastcall Get(int Index);
	virtual void __fastcall Grow(void);
	void __fastcall SetCapacity(int NewCapacity);
	void __fastcall SetCount(int NewCount);
	
public:
	__fastcall virtual ~TRectList(void);
	int __fastcall Add(const Types::TRect &Rect);
	virtual void __fastcall Clear(void);
	void __fastcall Delete(int Index);
	void __fastcall Exchange(int Index1, int Index2);
	int __fastcall IndexOf(const Types::TRect &Rect);
	void __fastcall Insert(int Index, const Types::TRect &Rect);
	void __fastcall Move(int CurIndex, int NewIndex);
	int __fastcall Remove(const Types::TRect &Rect);
	void __fastcall Pack(void);
	__property int Capacity = {read=FCapacity, write=SetCapacity, nodefault};
	__property int Count = {read=FCount, write=SetCount, nodefault};
	__property Types::PRect Items[int Index] = {read=Get/*, default*/};
	__property PPolyRects List = {read=FList};
public:
	/* TObject.Create */ inline __fastcall TRectList(void) : System::TObject() { }
	
};


class DELPHICLASS TClassList;
class PASCALIMPLEMENTATION TClassList : public Classes::TList
{
	typedef Classes::TList inherited;
	
public:
	System::TClass operator[](int Index) { return Items[Index]; }
	
protected:
	System::TClass __fastcall GetItems(int Index);
	void __fastcall SetItems(int Index, System::TClass AClass);
	
public:
	HIDESBASE int __fastcall Add(System::TClass AClass);
	HIDESBASE System::TClass __fastcall Extract(System::TClass Item);
	HIDESBASE int __fastcall Remove(System::TClass AClass);
	HIDESBASE int __fastcall IndexOf(System::TClass AClass);
	HIDESBASE System::TClass __fastcall First(void);
	HIDESBASE System::TClass __fastcall Last(void);
	System::TClass __fastcall Find(System::UnicodeString AClassName);
	void __fastcall GetClassNames(Classes::TStrings* Strings);
	HIDESBASE void __fastcall Insert(int Index, System::TClass AClass);
	__property System::TClass Items[int Index] = {read=GetItems, write=SetItems/*, default*/};
public:
	/* TList.Destroy */ inline __fastcall virtual ~TClassList(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TClassList(void) : Classes::TList() { }
	
};


struct TLinkedNode;
typedef TLinkedNode *PLinkedNode;

struct TLinkedNode
{
	
public:
	TLinkedNode *Prev;
	TLinkedNode *Next;
	void *Data;
};


typedef void __fastcall (*TIteratorProc)(PLinkedNode Node, int Index);

typedef void __fastcall (__closure *TFreeDataEvent)(void * Data);

class DELPHICLASS TLinkedList;
class PASCALIMPLEMENTATION TLinkedList : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	int FCount;
	TLinkedNode *FHead;
	TLinkedNode *FTail;
	TFreeDataEvent FOnFreeData;
	
protected:
	virtual void __fastcall DoFreeData(void * Data);
	
public:
	__fastcall virtual ~TLinkedList(void);
	PLinkedNode __fastcall Add(void);
	void __fastcall Remove(PLinkedNode Node);
	int __fastcall IndexOf(PLinkedNode Node);
	PLinkedNode __fastcall GetNode(int Index);
	void __fastcall Exchange(PLinkedNode Node1, PLinkedNode Node2);
	void __fastcall InsertBefore(PLinkedNode Node, PLinkedNode NewNode);
	void __fastcall InsertAfter(PLinkedNode Node, PLinkedNode NewNode);
	void __fastcall Clear(void);
	void __fastcall IterateList(TIteratorProc CallBack);
	__property PLinkedNode Head = {read=FHead, write=FHead};
	__property PLinkedNode Tail = {read=FTail, write=FTail};
	__property int Count = {read=FCount, write=FCount, nodefault};
	__property TFreeDataEvent OnFreeData = {read=FOnFreeData, write=FOnFreeData};
public:
	/* TObject.Create */ inline __fastcall TLinkedList(void) : System::TObject() { }
	
};


//-- var, const, procedure ---------------------------------------------------
static const Byte BUCKET_MASK = 0xff;
static const Word BUCKET_COUNT = 0x100;
extern PACKAGE void __fastcall SmartAssign(Classes::TPersistent* Src, Classes::TPersistent* Dst, Typinfo::TTypeKinds TypeKinds = (Typinfo::TTypeKinds() << tkInteger << tkChar << tkEnumeration << tkFloat << tkString << tkSet << tkClass << tkWChar << tkLString << tkWString << tkVariant << tkArray << tkRecord << tkInterface << tkInt64 << tkDynArray << tkUString << tkClassRef << tkPointer << tkProcedure ));
extern PACKAGE void __fastcall Advance(PLinkedNode &Node, int Steps = 0x1);

}	/* namespace Gr32_containers */
using namespace Gr32_containers;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_containersHPP
