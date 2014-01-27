#ifndef PRORUNACTION_HH
#define PRORUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "G4Timer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class	ProAnalysis;
class	G4Run;

class	ProRunAction	:	public	G4UserRunAction
{
public:
    ProRunAction(ProAnalysis*);
    virtual ~ProRunAction();
public:
    void	BeginOfRunAction(const	G4Run*);
    void	  EndOfRunAction(const	G4Run*);

    void fillPerEvent(G4double KinESd1, G4double KinESd2, G4double KinESd3);

private:
    ProAnalysis* fanalysis;

    G4double fSumEKin_SD1, fSum2EKin_SD1;
    G4double fSumEKin_SD2, fSum2EKin_SD2;
    G4double fSumEKin_SD3, fSum2EKin_SD3;

    G4Timer Mytime;
};


#endif // PRORUNACTION_HH
