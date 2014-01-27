#ifndef PROANALYSIS_HH
#define PROANALYSIS_HH

#include "globals.hh"
#include "G4ThreeVector.hh"

class TFile;
class TTree;
class TH1D;
class TH2D;

const G4int MaxHisto = 4;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class	ProAnalysis
{
public:
    ProAnalysis();
    ~ProAnalysis();

    void Book();
    void Save();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
    void Normalize(G4int id, G4double fac);

    void FillNtuple(G4double energySD1, G4double energySD2,
                    G4double energySD3);



    void PrintStatistic();

  private:

    TFile* rootFile;
    TH1D*  histo[MaxHisto];
    TTree* ntupl;

    G4double EKinSD1;
    G4double EKinSD2;
    G4double EKinSD3;

};

#endif // PROANALYSIS_HH
