#ifndef PROEVENTACTION_HH
#define PROEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class	ProRunAction;
class	G4HCofThisEvent;
class	ProAnalysis;

class	ProEventAction	:	public	G4UserEventAction
{
public:
    ProEventAction(ProRunAction*, ProAnalysis*);
    ~ProEventAction();

    void	BeginOfEventAction(const	G4Event*);
    void	   EndOfEventAction(const	G4Event*);

    void AddSD1(G4double de) {fEnergySD1 += de;}
    void AddSD2(G4double de) {fEnergySD2 += de;}
    void AddSD3(G4double de) {fEnergySD3 += de;}

private:

    void PrintEventStatistics(G4double Engy,
                              G4ThreeVector Pos,
                              G4ThreeVector Mom) const;

    //	data	members
    //
    ProRunAction*			    fRunAct;
    ProAnalysis*              fAnalysis;

    G4int                   fHitsCollID;
    G4int                  fPrintModulo;

    G4double fEnergySD1,fEnergySD2,fEnergySD3;
};

#endif // PROEVENTACTION_HH
