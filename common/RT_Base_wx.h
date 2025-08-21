#ifndef RT_BASE_WX_H
#define RT_BASE_WX_H

#include <wx/wx.h>
#include <string>

class TRTBase
{
private:
    int F_X, F_Y;
    std::wstring FName;
    bool FActive;

public:
    TRTBase() : F_X(0), F_Y(0), FActive(true) {}
    TRTBase(const std::wstring& name, bool active = true) 
        : F_X(0), F_Y(0), FName(name), FActive(active) {}
    virtual ~TRTBase() {}

    const std::wstring& Name() const { return FName; }
    void SetName(const std::wstring& name) { FName = name; }
    
    bool IsActive() const { return FActive; }
    void SetActive(bool active) { FActive = active; }
    
    int GetX() const { return F_X; }
    void SetX(int x) { F_X = x; }
    int GetY() const { return F_Y; }
    void SetY(int y) { F_Y = y; }

    virtual const std::wstring& ClassKey() const = 0;
    virtual wxPanel* CreateEditPanel(wxWindow* parent) = 0;
};

#endif