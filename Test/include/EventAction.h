#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.h"
#include "G4ThreeVector.hh"

class RunAction;
class HistoManager;
class G4HCofThisEvent;

class EventAction : public G4UserEventAction
{
  public:
    // Constructor
    EventAction();

    // Destructor
    virtual ~EventAction();

    // Methods
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);


  private:

    G4int fHitsCollectionID;
    G4int fHitsCollectionID_monitor;

    G4int fPrintModulo;

};


#endif // EVENTACTION_H
