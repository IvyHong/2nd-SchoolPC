#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class HistoManager;

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

  public:
public:
  inline void SetVerbose(G4int val) { fVerboseLevel = val; }
  inline G4int GetVerbose() const { return fVerboseLevel; }

  private:
    RunAction* fRunAct;
    HistoManager* fHistoMan;

    G4int fVerboseLevel;
    G4int fPrintModulo;

    G4int fHHC1ID;
    G4int fHHC2ID;
    G4int fDHC1ID;
    G4int fDHC2ID;
    G4int fECHCID;
    G4int fHCHCID;

};


#endif // EVENTACTION_H
