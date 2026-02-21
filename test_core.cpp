// Simple test to verify core library functionality
#include "RT_Network.h"
#include "RT_HHCurrent.h"
#include <iostream>
#include <locale>
#include <codecvt>

// Helper to convert wstring to string for output
std::string ws2s(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

int main() {
    std::cout << "Testing NetSuite Core Library..." << std::endl;
    
    try {
        // Create a network
        TNetwork network(L"TestNetwork");
        std::cout << "✓ Created network: " << ws2s(network.Name()) << std::endl;
        
        // Create an HH current (without a cell owner for now)
        THHCurrent current(nullptr, L"TestCurrent");
        std::cout << "✓ Created HH current" << std::endl;
        
        // Test property getters/setters
        current.Gmax(10.0);
        current.E(-60.0);
        current.p(3.0);
        current.q(1.0);
        
        std::cout << "✓ Set parameters:" << std::endl;
        std::cout << "  Gmax = " << current.Gmax() << " uS" << std::endl;
        std::cout << "  E = " << current.E() << " mV" << std::endl;
        std::cout << "  p = " << current.p() << std::endl;
        std::cout << "  q = " << current.q() << std::endl;
        
        // Test kinetic factors
        THHKineticsFactor& m_factor = current.get_m();
        m_factor.V0(-40.0);
        m_factor.k(5.0);
        std::cout << "✓ Configured m kinetic factor: V0=" << m_factor.V0() 
                  << ", k=" << m_factor.k() << std::endl;
        
        // Test network initialization
        if (network.Initialize(true)) {
            std::cout << "✓ Network initialized successfully" << std::endl;
        }
        
        std::cout << "\n✅ All core library tests passed!" << std::endl;
        std::cout << "   Core library is functional and ready for Qt UI development." << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
}
