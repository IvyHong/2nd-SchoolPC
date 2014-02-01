#ifndef MONITOR_H
#define MONITOR_H

#include "G4VSensitiveDetector.hh"
#include "MonitorHit.h"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class Monitor : public G4VSensitiveDetector
{
  public:
      Monitor(G4String name);
      virtual ~Monitor();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      MonitorHitsCollection * fHitsCollection;

      G4int fHitsCollectionID;

};



#endif // Monitor_H
