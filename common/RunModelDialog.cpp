#include "RunModelDialog.h"
#include "RT_Network_wx.h"
#include "RT_Cell_wx.h"
#include "PlotDialog.h"

enum {
    ID_RUN_BUTTON = 1000,
    ID_STOP_BUTTON,
    ID_CLOSE_BUTTON
};

wxBEGIN_EVENT_TABLE(RunModelDialog, wxDialog)
    EVT_BUTTON(ID_RUN_BUTTON, RunModelDialog::OnRun)
    EVT_BUTTON(ID_STOP_BUTTON, RunModelDialog::OnStop)
    EVT_BUTTON(ID_CLOSE_BUTTON, RunModelDialog::OnClose)
    EVT_CLOSE(RunModelDialog::OnClose)
wxEND_EVENT_TABLE()

RunModelDialog::RunModelDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Run Model", wxDefaultPosition, wxSize(400, 300))
    , m_running(false)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Parameters section
    wxStaticBoxSizer* paramsSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Parameters");
    
    // Sample Rate
    wxBoxSizer* rateSizer = new wxBoxSizer(wxHORIZONTAL);
    rateSizer->Add(new wxStaticText(this, wxID_ANY, "Sample Rate (Hz):"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_sampleRateCtrl = new wxSpinCtrlDouble(this, wxID_ANY, "10000", wxDefaultPosition, wxDefaultSize, 
                                           wxSP_ARROW_KEYS, 1.0, 100000.0, 10000.0, 1.0);
    rateSizer->Add(m_sampleRateCtrl, 1, wxALL, 5);
    paramsSizer->Add(rateSizer, 0, wxEXPAND);
    
    // Duration
    wxBoxSizer* durSizer = new wxBoxSizer(wxHORIZONTAL);
    durSizer->Add(new wxStaticText(this, wxID_ANY, "Duration (s):"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_durationCtrl = new wxSpinCtrlDouble(this, wxID_ANY, "1.0", wxDefaultPosition, wxDefaultSize,
                                         wxSP_ARROW_KEYS, 0.001, 1000.0, 1.0, 0.001);
    durSizer->Add(m_durationCtrl, 1, wxALL, 5);
    paramsSizer->Add(durSizer, 0, wxEXPAND);
    
    // Repeats
    wxBoxSizer* repSizer = new wxBoxSizer(wxHORIZONTAL);
    repSizer->Add(new wxStaticText(this, wxID_ANY, "Repeats:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_repeatsCtrl = new wxSpinCtrl(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize,
                                  wxSP_ARROW_KEYS, 1, 1000, 1);
    repSizer->Add(m_repeatsCtrl, 1, wxALL, 5);
    paramsSizer->Add(repSizer, 0, wxEXPAND);
    
    mainSizer->Add(paramsSizer, 0, wxEXPAND | wxALL, 10);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_runButton = new wxButton(this, ID_RUN_BUTTON, "Run Model");
    m_stopButton = new wxButton(this, ID_STOP_BUTTON, "Stop");
    m_stopButton->Enable(false);
    
    buttonSizer->Add(m_runButton, 0, wxALL, 5);
    buttonSizer->Add(m_stopButton, 0, wxALL, 5);
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(new wxButton(this, ID_CLOSE_BUTTON, "Close"), 0, wxALL, 5);
    
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);
    
    SetSizer(mainSizer);
}

void RunModelDialog::OnRun(wxCommandEvent& event)
{
    if (m_running) return;
    
    m_running = true;
    m_runButton->Enable(false);
    m_stopButton->Enable(true);
    
    RunModel();
    
    m_running = false;
    m_runButton->Enable(true);
    m_stopButton->Enable(false);
}

void RunModelDialog::OnStop(wxCommandEvent& event)
{
    m_running = false;
    m_runButton->Enable(true);
    m_stopButton->Enable(false);
}

void RunModelDialog::OnClose(wxCommandEvent& event)
{
    if (m_running) {
        m_running = false;
    }
    EndModal(wxID_CLOSE);
}

void RunModelDialog::OnClose(wxCloseEvent& event)
{
    if (m_running) {
        m_running = false;
    }
    EndModal(wxID_CLOSE);
}

void RunModelDialog::RunModel()
{
    TNetwork* network = GetNet();
    if (!network) {
        wxMessageBox("No network loaded", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    double sampleRate = m_sampleRateCtrl->GetValue();
    double duration = m_durationCtrl->GetValue();
    int repeats = m_repeatsCtrl->GetValue();
    
    // Initialize network for simulation
    if (!network->Initialize(true)) {
        wxMessageBox("Failed to initialize network", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Get network description
    auto netDesc = network->DescribeNetwork();
    int totalCells = netDesc.NumVDepCells + netDesc.NumTimeCells;
    
    if (totalCells == 0) {
        wxMessageBox("No active cells in network", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    unsigned long numSamples = ceil(duration * sampleRate);
    double stepMs = 1000.0 / sampleRate;  // ms per step
    
    // Allocate arrays for simulation
    double* vmOut = new double[totalCells];
    double* iOut = netDesc.NumVDepCells > 0 ? new double[netDesc.NumVDepCells] : nullptr;
    
    // Data storage for plotting (only store last repeat)
    std::vector<std::vector<double>> plotData(totalCells);
    for (int i = 0; i < totalCells; i++) {
        plotData[i].reserve(numSamples);
    }
    
    wxProgressDialog progress("Running Model", "Simulating network...", 
                             numSamples * repeats, this,
                             wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_APP_MODAL);
    
    for (int rep = 0; rep < repeats && m_running; rep++) {
        // Reset network for each repeat
        if (rep > 0) {
            network->Initialize(true);
        }
        
        // Clear data for new repeat
        if (rep == repeats - 1) { // Only store data for last repeat
            for (auto& trace : plotData) {
                trace.clear();
            }
        }
        
        // Run simulation
        for (unsigned long i = 0; i < numSamples && m_running; i++) {
            // Call network update - this is the real simulation!
            network->Update(stepMs, nullptr, vmOut, iOut);
            
            // Store data for plotting (last repeat only)
            if (rep == repeats - 1) {
                for (int j = 0; j < totalCells; j++) {
                    plotData[j].push_back(vmOut[j]);
                }
            }
            
            if (i % 1000 == 0) {  // Update progress every 1000 samples
                if (!progress.Update(rep * numSamples + i)) {
                    m_running = false;  // User cancelled
                    break;
                }
            }
        }
    }
    
    // Show results if simulation completed
    if (m_running && !plotData.empty() && !plotData[0].empty()) {
        // Create cell labels
        std::vector<wxString> labels;
        int cellIndex = 0;
        for (const auto& pair : network->GetCells()) {
            if (pair.second->IsActive()) {
                labels.push_back(wxString(pair.first));
                cellIndex++;
            }
        }
        
        wxMessageBox(wxString::Format("Showing plot with %zu traces, %zu samples each", 
                                     plotData.size(), plotData[0].size()), "Debug");
        
        // Show plot dialog
        PlotDialog plotDlg(this, plotData, labels, stepMs / 1000.0); // convert to seconds
        plotDlg.ShowModal();
    } else {
        wxMessageBox(wxString::Format("No plot data: running=%d, traces=%zu, samples=%zu", 
                                     m_running, plotData.size(), 
                                     plotData.empty() ? 0 : plotData[0].size()), "Debug");
    }
    
    // Clean up
    delete[] vmOut;
    if (iOut) delete[] iOut;
    
    if (m_running) {
        wxMessageBox(wxString::Format("Model run complete!\n%d repeats of %.3f seconds\n%d cells simulated", 
                                     repeats, duration, totalCells), "Success", wxOK | wxICON_INFORMATION);
    }
}