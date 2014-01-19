#ifndef RUNACTION_H
#define RUNACTION_H

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Timer.hh"

class HistoManager;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
public:
       RunAction(HistoManager*);
       virtual ~RunAction();
public:
       void  BeginOfRunAction(const G4Run*);
       void    EndOfRunAction(const G4Run*);

       void fillPerEvent(G4double, G4double, G4double, G4double);

private:
       HistoManager* fanalysis;
       G4Timer*      myTimer;

       G4double fSumEAbs, fSum2EAbs;
       G4double fSumEGap, fSum2EGap;

       G4double fSumLAbs, fSum2LAbs;
       G4double fSumLGap, fSum2LGap;
};

#endif // RUNACTION_H
