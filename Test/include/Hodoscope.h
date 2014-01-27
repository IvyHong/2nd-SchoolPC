#ifndef HODOSCOPE_H
#define HODOSCOPE_H

#include "G4VSensitiveDetector.hh"
#include "HodoscopeHit.h"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class Hodoscope : public G4VSensitiveDetector
{
  public:
      Hodoscope(G4String name);
      virtual ~Hodoscope();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      HodoscopeHitsCollection* fHitsCollection;
      G4int fHCID;
};



#endif // HODOSCOPE_H
