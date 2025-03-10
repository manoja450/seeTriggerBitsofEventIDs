//This code prints the triggerBits and nsTime (converted to microseconds) along with eventID in the terminal.
// We can give multiple eventIDs from terminal.
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"

using namespace std;

void displayTriggerBits(const char *fileName, const vector<int> &eventIDs) {
    // Open the ROOT file
    TFile *file = TFile::Open(fileName, "READ");
    if (!file || file->IsZombie()) {
        cerr << "Error: Cannot open file " << fileName << endl;
        return;
    }

    // Get the tree
    TTree *tree = (TTree*)file->Get("tree"); // Replace "tree" with actual tree name if different
    if (!tree) {
        cerr << "Error: Cannot find TTree in file." << endl;
        file->Close();
        return;
    }

    // Define variables to hold data
    Int_t eventID;        // Change this to match ROOT file
    Int_t triggerBits;    // Change to match ROOT file
    Long64_t nsTime;      // Nanoseconds time

    tree->SetBranchAddress("eventID", &eventID);
    tree->SetBranchAddress("triggerBits", &triggerBits);
    tree->SetBranchAddress("nsTime", &nsTime);

    cout << "EventID\tTriggerBits\tTime (µs)" << endl;

    // Loop over the tree to find the specified events
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        
        // Debugging: Check which event IDs exist
        // cout << "Checking Event ID: " << eventID << endl;

        if (find(eventIDs.begin(), eventIDs.end(), eventID) != eventIDs.end()) {
            cout << eventID << "\t" << triggerBits << "\t" << nsTime / 1000.0 << endl; // Convert ns to µs
        }
    }

    // Close the file
    file->Close();
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <ROOT file path> <eventID1> <eventID2> ..." << endl;
        return 1;
    }

    const char *fileName = argv[1];
    vector<int> eventIDs;  // Change to int instead of Long64_t

    for (int i = 2; i < argc; i++) {
        eventIDs.push_back(atoi(argv[i])); // Convert argument to int
    }

    displayTriggerBits(fileName, eventIDs);
    return 0;
}
