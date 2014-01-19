#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, HistoManager*);
  virtual ~EventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  void AddAbs(G4double de, G4double dl) {fEnergyAbs += de; fTrackLAbs += dl;};
  void AddGap(G4double de, G4double dl) {fEnergyGap += de; fTrackLGap += dl;};

private:
   RunAction* fRunAct;
   HistoManager* fHistoManager;

   G4double fEnergyAbs, fEnergyGap;
   G4double fTrackLAbs, fTrackLGap;

   G4int fPrintModulo;
};



#endif // EVENTACTION_H
