#include <wx/wx.h>
#include "NetworkFrame.h"

class NetSuiteApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(NetSuiteApp);

bool NetSuiteApp::OnInit()
{
    NetworkFrame *frame = new NetworkFrame("NetClamp - NN Modeling and Dynamic Clamp");
    frame->Show(true);
    return true;
}