/** @file

@brief Give a brief description of this file, here.
*/
#ifndef HISTOMANAGER_H
#define HISTOMANAGER_H

#include "globals.hh"

class TFile;
class TTree;
class TH1D;

const G4int MaxHisto = 5;

/** Document this class here
*/
class HistoManager
{
  public:
    HistoManager();
   ~HistoManager();

    void book();
    void save();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
    void Normalize(G4int id, G4double fac);

    void FillNtuple(G4double energyAbs, G4double energyGap,
                    G4double trackLAbs, G4double trackLGap);

    void PrintStatistic();

  private:
    TFile* rootFile;            //TFile object is used to access ROOT files
    TH1D*  histo[MaxHisto];     //Creating histograms, an array of object pointers
    TTree* ntupl;               //In case, store large quantities of same-class object
    // TTree and TNtuple (TNtuple is a TTree that is limited to only hold floating number)
    // TTree holds all kind of data

    G4double Eabs;
    G4double Egap;
    G4double Labs;
    G4double Lgap;
};
#endif // HISTOMANAGER_H
