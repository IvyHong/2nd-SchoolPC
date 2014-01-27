#ifndef HADCALORIMETER_H
#define HADCALORIMETER_H

#include "G4VSensitiveDetector.hh"
#include "HadCalorimeterHit.h"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class HadCalorimeter : public G4VSensitiveDetector
{

  public:
      HadCalorimeter(G4String name);
      virtual ~HadCalorimeter();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      HadCalorimeterHitsCollection* fHitsCollection;
      G4int fHCID;
};

#endif // HADCALORIMETER_H
