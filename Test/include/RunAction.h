#ifndef RUNACTION_H
#define RUNACTION_H

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Timer.hh"

// forward declarations
class HistoManager;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
public:
       RunAction();
       ~RunAction();
public:
       void  BeginOfRunAction(const G4Run*);
       void    EndOfRunAction(const G4Run*);

private:

       G4Timer*      myTimer;

};

#endif // RUNACTION_H
