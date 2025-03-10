//It helps to see the events which satisfies triggerBit==? creteria. Events satisfying certain trigger types. 
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TAxis.h>
#include <vector>
#include <algorithm>
#include <cmath>

// Function prototype
void PlotCombinedChartAndIndividual(const char *fileName);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <root_filename>" << std::endl;
        return 1;
    }

    const char* fileName = argv[1];
    PlotCombinedChartAndIndividual(fileName);
    return 0;
}

// Function definition
void PlotCombinedChartAndIndividual(const char *fileName) {
    TFile *file = TFile::Open(fileName);
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    TTree *tree = (TTree*)file->Get("tree");
    if (!tree) {
        std::cerr << "Error accessing TTree 'tree'!" << std::endl;
        file->Close();
        return;
    }

    Short_t adcVal[23][45]; // ADC values
    double_t pulseH[23];    // Pulse height values
    Int_t triggerBits;      // Trigger bits
    Int_t eventID;          // Event ID

    tree->SetBranchAddress("adcVal", adcVal);
    tree->SetBranchAddress("pulseH", pulseH);
    tree->SetBranchAddress("triggerBits", &triggerBits);
    tree->SetBranchAddress("eventID", &eventID);  // Bind eventID

    Long64_t nEntries = tree->GetEntries();
    Long64_t maxEntries = std::min(nEntries, (Long64_t)1000); // Limit to 1000 entries

    std::vector<int> eventIDs;

    for (Long64_t j = 0; j < maxEntries; j++) {
        tree->GetEntry(j); // Load the current entry

        if (triggerBits == 34) { 
            eventIDs.push_back(eventID);
        }
    }

    file->Close();

    // Print output in array format
    std::cout << "[";
    for (size_t i = 0; i < eventIDs.size(); i++) {
        std::cout << eventIDs[i];
        if (i != eventIDs.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
