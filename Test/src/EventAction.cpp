#include "EventAction.h"

#include "RunAction.h"
#include "HistoManager.h"

#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run, HistoManager* histo)
:fRunAct(run),fHistoManager(histo)
{
 fPrintModulo = 100; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if (evtNb%fPrintModulo == 0)
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;

 // initialisation per event
 fEnergyAbs = fEnergyGap = 0.;
 fTrackLAbs = fTrackLGap = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  //accumulates statistic
  //
  fRunAct->fillPerEvent(fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap);

  //fill histograms
  //
  fHistoManager->FillHisto(1, fEnergyAbs);
  fHistoManager->FillHisto(2, fEnergyGap);
  fHistoManager->FillHisto(3, fTrackLAbs);
  fHistoManager->FillHisto(4, fTrackLGap);

  //fill ntuple
  //
  fHistoManager->FillNtuple(fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap);
}
