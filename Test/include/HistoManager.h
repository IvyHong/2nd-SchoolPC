/** @file

@brief Give a brief description of this file, here.
*/

#ifndef HISTOMANAGER_H
#define HISTOMANAGER_H

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"

// Root classes
class TFile;
class TTree;
class TH1D;

const G4int MaxHisto = 5;

class HistoManager
{

private:
  HistoManager();

public:

  virtual ~HistoManager();
  static HistoManager* getInstance();

  void Book();
  void Save();

  void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
  void Normalize(G4int id, G4double fac);

  void FillNtuple(G4double KinE, G4double EDep);

  void PrintStatistic();

private:

  static HistoManager* fInstance;

  TFile* rootFile;
  TH1D*  histo[MaxHisto];
  TTree* ntupl;

  G4double fECalorimeter;
  G4double fEMonitor;

};



#endif // HISTOMANAGER_H

