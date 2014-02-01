// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.h"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

HistoManager* HistoManager::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
    :rootFile(0),ntupl(0), fECalorimeter(0), fEMonitor(0)
{
    // histograms
    for (G4int k=0; k<MaxHisto; k++) histo[k] = 0;

    // ntuple
    ntupl = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
    if(rootFile) delete rootFile;
}


HistoManager* HistoManager::getInstance()
{
  if (fInstance == 0) fInstance = new HistoManager();
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
    // Creating a tree container to handle histograms and ntuples.
    // This tree is associated to an output file.
    //
    G4String fileName = "Test.root";
    rootFile = new TFile(fileName,"RECREATE");
    if(!rootFile) {
      G4cout << " HistoManager::book :"
             << " problem creating the ROOT TFile "
             << G4endl;
      return;
    }

    histo[1] = new TH1D("1", "Deposited Energy in EmCalorimeter", 100, 0., 180*CLHEP::MeV);
    if (!histo[1]) G4cout << "\n can't create histo 1" << G4endl;
    histo[2] = new TH1D("2", "Deposited Energy in Monitor", 100, 0., 180*CLHEP::MeV);
    if (!histo[2]) G4cout << "\n can't create histo 2" << G4endl;

//    histo[3] = new TH1D("3", "Number of Detected Neutrons in EmCalorimeter", 100, 0., 10000);
//    if (!histo[3]) G4cout << "\n can't create histo 3" << G4endl;
//    histo[4] = new TH1D("4", "Number of Detected Neutrons in Monitor", 100, 0., 10000);
//    if (!histo[4]) G4cout << "\n can't create histo 4" << G4endl;


    // create 1 ntuple in subdirectory "tuples"
    //
    ntupl = new TTree("101", "EKin");
    ntupl->Branch("ECalorimeter", &fECalorimeter, "ECalorimeter/D");
    ntupl->Branch("EMonitor", &fEMonitor, "EMonitor/D");



    G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
    if (rootFile) {
      rootFile->Write(); // Writing the histograms to the file
      rootFile->Close(); // and closing the tree (and the file)
      G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
 if (histo[ih]) { histo[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (histo[ih]) histo[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(G4double Ecal, G4double Emon)
{
 fECalorimeter = Ecal;
 fEMonitor = Emon;

  if (ntupl) ntupl->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  if(histo[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;

    G4cout
    << " ECalorimeter : mean = " << G4BestUnit(histo[1]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[1]->GetRMS(), "Energy") << G4endl;
    G4cout
    << " EMonitor : mean = " << G4BestUnit(histo[2]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[2]->GetRMS(), "Energy") << G4endl;

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

