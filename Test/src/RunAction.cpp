#include        "RunAction.h"
#include        "HistoManager.h"

#include        "G4Run.hh"
#include        "G4RunManager.hh"
#include        "G4UnitsTable.hh"

#include        "G4Timer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo)
    :fanalysis(histo),
      myTimer(0)
{
    myTimer = new G4Timer;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction()
{
    delete myTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout <<"### Run "<< aRun->GetRunID() <<" start." << G4endl;
    myTimer->Start();

    //initialize cumulative quantities
    //
    fSumEAbs = fSum2EAbs =fSumEGap = fSum2EGap = 0.;
    fSumLAbs = fSum2LAbs =fSumLGap = fSum2LGap = 0.;

    //histograms
    //
    fanalysis->book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::fillPerEvent(G4double EAbs, G4double EGap,
                             G4double LAbs, G4double LGap)
{
  //accumulate statistic
  //
  fSumEAbs += EAbs; fSum2EAbs += EAbs*EAbs;
  fSumEGap += EGap; fSum2EGap += EGap*EGap;

  fSumLAbs += LAbs; fSum2LAbs += LAbs*LAbs;
  fSumLGap += LGap; fSum2LGap += LGap*LGap;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    G4int NbOfEvents = aRun->GetNumberOfEvent();

    if (NbOfEvents == 0) return;

    //compute statistics: mean and rms
    //
    fSumEAbs /= NbOfEvents; fSum2EAbs /= NbOfEvents;
    G4double rmsEAbs = fSum2EAbs - fSumEAbs*fSumEAbs;
    if (rmsEAbs >0.) rmsEAbs = std::sqrt(rmsEAbs); else rmsEAbs = 0.;

    fSumEGap /= NbOfEvents; fSum2EGap /= NbOfEvents;
    G4double rmsEGap = fSum2EGap - fSumEGap*fSumEGap;
    if (rmsEGap >0.) rmsEGap = std::sqrt(rmsEGap); else rmsEGap = 0.;

    fSumLAbs /= NbOfEvents; fSum2LAbs /= NbOfEvents;
    G4double rmsLAbs = fSum2LAbs - fSumLAbs*fSumLAbs;
    if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.;

    fSumLGap /= NbOfEvents; fSum2LGap /= NbOfEvents;
    G4double rmsLGap = fSum2LGap - fSumLGap*fSumLGap;
    if (rmsLGap >0.) rmsLGap = std::sqrt(rmsLGap); else rmsLGap = 0.;

    //print
    //
    G4cout
       << "\n--------------------End of Run------------------------------\n"
       << "\n mean Energy in Absorber : " << G4BestUnit(fSumEAbs,"Energy")
       << " +- " << G4BestUnit(rmsEAbs,"Energy")
       << "\n mean Energy in Gap : " << G4BestUnit(fSumEGap,"Energy")
       << " +- " << G4BestUnit(rmsEGap,"Energy")
       << G4endl;

    G4cout
       << "\n mean trackLength in Absorber : " << G4BestUnit(fSumLAbs,"Length")
       << " +- " << G4BestUnit(rmsLAbs,"Length")
       << "\n mean trackLength in Gap : " << G4BestUnit(fSumLGap,"Length")
       << " +- " << G4BestUnit(rmsLGap,"Length")
       << "\n------------------------------------------------------------\n"
       << G4endl;

    //save histograms
    //

    fanalysis->PrintStatistic();
    fanalysis->save();

    myTimer->Stop();
    G4cout << "The number of event = " << NbOfEvents
           << " " << *myTimer << G4endl;
}
