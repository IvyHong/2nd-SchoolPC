#ifndef NEUTRONSD_H
#define NEUTRONSD_H

#include "G4VSensitiveDetector.hh"
#include "NeutronHit.h"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NeutronSD : public G4VSensitiveDetector
{
  public:
      NeutronSD(const G4String& name);
      ~NeutronSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      NeutronHitsCollection* nHitsCollection;
      G4int nHCID;

};

#endif // NEUTRONSD_H
