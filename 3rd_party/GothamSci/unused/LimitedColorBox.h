//---------------------------------------------------------------------------

#ifndef LimitedColorBoxH
#define LimitedColorBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TLimitedColorBox : public TCustomColorBox
{
private:
protected:
	void __fastcall PopulateList();
public:
	__fastcall TLimitedColorBox(TComponent* Owner);
__published:
	__property Align  = {default=0};
	__property AutoComplete  = {default=1};
	__property AutoDropDown  = {default=0};
	__property DefaultColorColor  = {default=0};
	__property NoneColorColor  = {default=0};
	__property Selected  = {default=0};
	__property Anchors  = {default=3};
	__property BevelEdges  = {default=15};
	__property BevelInner  = {index=0, default=2};
	__property BevelKind  = {default=0};
	__property BevelOuter  = {index=1, default=1};
	__property BiDiMode ;
	__property Color  = {default=-16777211};
	__property Constraints ;
	__property Ctl3D ;
	__property DropDownCount  = {default=8};
	__property Enabled  = {default=1};
	__property Font ;
	__property ItemHeight ;
	__property ParentBiDiMode  = {default=1};
	__property ParentColor  = {default=0};
	__property ParentCtl3D  = {default=1};
	__property ParentFont  = {default=1};
	__property ParentShowHint  = {default=1};
	__property PopupMenu ;
	__property ShowHint ;
	__property TabOrder  = {default=-1};
	__property TabStop  = {default=1};
	__property Visible  = {default=1};
	__property OnChange ;
	__property OnCloseUp ;
	__property OnClick ;
	__property OnContextPopup ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnDropDown ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnGetColors ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnMouseEnter ;
	__property OnMouseLeave ;
	__property OnSelect ;
	__property OnStartDock ;
	__property OnStartDrag ;
};
//---------------------------------------------------------------------------
#endif
