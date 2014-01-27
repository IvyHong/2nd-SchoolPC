#include "ProAnalysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TROOT.h"
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProAnalysis::ProAnalysis()
    :rootFile(0),ntupl(0), EKinSD1(0), EKinSD2(0) ,EKinSD3(0)
{
    // histograms
    for (G4int k=0; k<MaxHisto; k++) histo[k] = 0;

    // ntuple
    ntupl = 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ProAnalysis::~ProAnalysis()
{
    if(rootFile) delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void	ProAnalysis::Book()
{
    // Creating a tree container to handle histograms and ntuples.
    // This tree is associated to an output file.
    //
    G4String fileName = "Test.root";
    rootFile = new TFile(fileName,"RECREATE");
    if(!rootFile) {
      G4cout << " ProAnalysis::Book :"
             << " problem creating the ROOT TFile "
             << G4endl;
      return;
    }

    histo[1] = new TH1D("1", "Kinetic Energy in ChamberSD1", 100, 0., 180*CLHEP::MeV);
    if (!histo[1]) G4cout << "\n can't create histo 1" << G4endl;
    histo[2] = new TH1D("2", "Kinetic Energy in ChamberSD2", 100, 0., 180*CLHEP::MeV);
    if (!histo[2]) G4cout << "\n can't create histo 2" << G4endl;
    histo[3] = new TH1D("3", "Kinetic Energy in ChamberSD3", 100, 0., 180*CLHEP::MeV);
    if (!histo[3]) G4cout << "\n can't create histo 3" << G4endl;


    // create 1 ntuple in
    //
    ntupl = new TTree("???", "Kinetic Energy");
    ntupl->Branch("EKin_SD1", &EKinSD1, "EKinSD1/D");
    ntupl->Branch("EKin_SD2", &EKinSD2, "EKinSD2/D");
    ntupl->Branch("EKin_SD3", &EKinSD3, "EKinSD3/D");


    G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProAnalysis::Save()
{
    if (rootFile) {
      rootFile->Write(); // Writing the histograms to the file
      rootFile->Close(); // and closing the tree (and the file)
      G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProAnalysis::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from ProAnalysis::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
 if (histo[ih]) { histo[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProAnalysis::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from ProAnalysis::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (histo[ih]) histo[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProAnalysis::FillNtuple(G4double energySD1, G4double energySD2,
                             G4double energySD3)
{
    EKinSD1 = energySD1;
    EKinSD2 = energySD2;
    EKinSD3 = energySD3;

  if (ntupl) ntupl->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProAnalysis::PrintStatistic()
{
  if(histo[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;

    G4cout
    << " KinE_SD1 : mean = " << G4BestUnit(histo[1]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[1]->GetRMS(), "Energy") << G4endl;
    G4cout
    << " KinE_SD2 : mean = " << G4BestUnit(histo[2]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[2]->GetRMS(), "Energy") << G4endl;
    G4cout
    << " KinE_SD3 : mean = " << G4BestUnit(histo[3]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[3]->GetRMS(), "Energy") << G4endl;

  }
}

