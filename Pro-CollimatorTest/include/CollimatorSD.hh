#ifndef COLLIMATORSD_HH
#define COLLIMATORSD_HH

#include "G4VSensitiveDetector.hh"
#include "CollimatorDetectorHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CollimatorSD : public G4VSensitiveDetector
{
  public:
      CollimatorSD(const G4String& name);
      ~CollimatorSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      CollimatorDetectorHitsCollection* pHitsCollection;
      G4int pHCID;

};

#endif // COLLIMATORSD_HH
