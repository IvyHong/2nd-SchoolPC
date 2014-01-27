#ifndef PRONEUTRONSD_HH
#define PRONEUTRONSD_HH

#include "G4VSensitiveDetector.hh"
#include "ProNeutronHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ProNeutronSD : public G4VSensitiveDetector
{
  public:
      ProNeutronSD(const G4String& name);
      ~ProNeutronSD();

      virtual void    Initialize(G4HCofThisEvent*);
      virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      virtual void    EndOfEvent(G4HCofThisEvent*);

  private:
      ProNeutronHitsCollection* fHitsCollection;
      G4int     fHCID;

};

#endif // PRONEUTRONSD_HH
