#ifndef PROGAMMASD_HH
#define PROGAMMASD_HH

#include "G4VSensitiveDetector.hh"
#include "ProGammaHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ProGammaSD : public G4VSensitiveDetector
{
  public:
      ProGammaSD(const G4String& name);
     ~ProGammaSD();

      void    Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void    EndOfEvent(G4HCofThisEvent*);

  private:
      ProGammaHitsCollection* gHitsCollection;

};

#endif // PROGAMMASD_HH
