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
class TH2D;

class HistoManager
{
private:
     HistoManager();  //protected

public:

  virtual ~HistoManager();
  static HistoManager* getInstance();

  void Update();
  void Clear();
//  void save(const G4String& fname);
  void Save();


  void FillIncident(const G4ThreeVector& p);
  void FillDose(const G4ThreeVector& p, G4double dedx);

  void ClearIncidentFlag();

private:

  static HistoManager* fInstance;

  TH2D* incident_map;
  TH1D* incident_x_hist;

  TH2D* dose_map;
  TH1D* dose_hist;

  G4bool incidentFlag;

};

inline void HistoManager::ClearIncidentFlag()
{
    incidentFlag = false;
}

#endif // HISTOMANAGER_H

