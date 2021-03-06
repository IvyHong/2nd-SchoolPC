#ifndef EMCALORIMETER_H
#define EMCALORIMETER_H

#include "G4VSensitiveDetector.hh"
#include "EmCalorimeterHit.h"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class EmCalorimeter : public G4VSensitiveDetector
{

  public:
      EmCalorimeter(const G4String& name);
      virtual ~EmCalorimeter();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      // Data members
      EmCalorimeterHitsCollection* fHitsCollection;
      G4int fHitsCollectionID;
};

#endif // EMCALORIMETER_H
