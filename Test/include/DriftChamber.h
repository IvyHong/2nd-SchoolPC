#ifndef DRIFTCHAMBER_H
#define DRIFTCHAMBER_H

#include "G4VSensitiveDetector.hh"
#include "DriftChamberHit.h"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class DriftChamber : public G4VSensitiveDetector
{
  public:
      DriftChamber(G4String name);
      virtual ~DriftChamber();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      DriftChamberHitsCollection * fHitsCollection;
      G4int fHCID;
};



#endif // DRIFTCHAMBER_H
