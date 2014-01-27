#include	"ProEventAction.hh"
#include	"ProRunAction.hh"
#include	"ProAnalysis.hh"
#include    "ProNeutronHit.hh"

#include	"G4Event.hh"
#include	"G4RunManager.hh"
#include    "G4EventManager.hh"

#include	"Randomize.hh"
#include	<iomanip>

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ProEventAction::ProEventAction(ProRunAction* runact, ProAnalysis* anl)
    : fRunAct(runact),
      fAnalysis(anl),
      fHitsCollID(-1)
{
    fPrintModulo = 100;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProEventAction::~ProEventAction()
{}


void ProEventAction::PrintEventStatistics(G4double Engy,
                                          G4ThreeVector Pos,
                                          G4ThreeVector Mom) const
{
  // print event statistics
  G4cout
     << " Energy : "
     << std::setw(7) << G4BestUnit(Engy, "Energy")
     << " Position : "
     << std::setw(7) << G4BestUnit(Pos, "Length")
     << " Momentum: "
     << std::setw(7) << G4BestUnit(Mom, "Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void	ProEventAction::BeginOfEventAction(const	G4Event*	evt)
{
    G4int evtNb = evt->GetEventID();
    if(evtNb%fPrintModulo == 0)
        G4cout<<"\n ---> Begin of Events :" << evtNb << G4endl;
    G4String colName;
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    if(-1==fHitsCollID)
    fHitsCollID = SDman->GetCollectionID(colName="NEUTRONHitsCollection");
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void    ProEventAction::EndOfEventAction(const	G4Event* evt)
{
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    ProNeutronHitsCollection* fHitsColl = 0;

    if(HCE)
    {
        fHitsColl = (ProNeutronHitsCollection*)(HCE->GetHC(fHitsCollID));
    }

    if(fAnalysis && fHitsColl)
    {
        G4int n_hit = fHitsColl->entries();
        G4cout << "Sensitive Detectors : " << n_hit << " hits." << G4endl;

        for(G4int k=0;k<n_hit;k++)
        {
            ProNeutronHit* aHit =(*fHitsColl)[k];
//            G4ThreeVector localPos = aHit->GetLocalPos();
//            G4ThreeVector Momentum = aHit->GetMomentum();
            G4double KEnergy = aHit->GetKinE();
            G4int ID = aHit->GetID();
            fAnalysis->FillHisto(ID,KEnergy);
        }
    }

}

