#include "EventAction.h"
#include "HistoManager.h"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "HodoscopeHit.h"
#include "DriftChamberHit.h"
#include "EmCalorimeterHit.h"

#include "HadCalorimeterHit.h"

EventAction::EventAction()
    :fPrintModulo(100)
{
    fHHC1ID=-1;
    fHHC2ID=-1;
    fDHC1ID=-1;
    fDHC2ID=-1;
    fECHCID=-1;
    fHCHCID=-1;
    fVerboseLevel = 1;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    G4int evtNb = evt->GetEventID();

    G4String colName;
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    if(fHHC1ID==-1&&fHHC2ID==-1&&fDHC1ID==-1&&fDHC2ID==-1&&fECHCID==-1&&fHCHCID==-1)
    {
    fHHC1ID = SDman->GetCollectionID(colName="hodoscope1/hodoscopeColl");
    fHHC2ID = SDman->GetCollectionID(colName="hodoscope2/hodoscopeColl");
    fDHC1ID = SDman->GetCollectionID(colName="chamber1/driftChamberColl");
    fDHC2ID = SDman->GetCollectionID(colName="chamber2/driftChamberColl");
    fECHCID = SDman->GetCollectionID(colName="EMcalorimeter/EMcalorimeterColl");
    fHCHCID = SDman->GetCollectionID(colName="HadCalorimeter/HadCalorimeterColl");
    }

    if (evtNb%fPrintModulo == 0)
      G4cout << "\n---> Begin of event: " << evtNb << G4endl;
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  HodoscopeHitsCollection* fHHC1 = 0;
  HodoscopeHitsCollection* fHHC2 = 0;
  DriftChamberHitsCollection* fDHC1 = 0;
  DriftChamberHitsCollection* fDHC2 = 0;
  EmCalorimeterHitsCollection* ECHC = 0;
  HadCalorimeterHitsCollection* HCHC = 0;
  if(HCE)
  {
    fHHC1 = (HodoscopeHitsCollection*)(HCE->GetHC(fHHC1ID));
    fHHC2 = (HodoscopeHitsCollection*)(HCE->GetHC(fHHC2ID));
    fDHC1 = (DriftChamberHitsCollection*)(HCE->GetHC(fDHC1ID));
    fDHC2 = (DriftChamberHitsCollection*)(HCE->GetHC(fDHC2ID));
    ECHC = (EmCalorimeterHitsCollection*)(HCE->GetHC(fECHCID));
    HCHC = (HadCalorimeterHitsCollection*)(HCE->GetHC(fHCHCID));
  }

  // DO SOME ANALYSIS
  HistoManager* analysis = HistoManager::getInstance();



  // Diagnostics

  if (fVerboseLevel==0 || evt->GetEventID() % fVerboseLevel != 0) return;

  G4PrimaryParticle* primary = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4cout << G4endl
         << ">>> Event " << evt->GetEventID() << " >>> Simulation truth : "
         << primary->GetG4code()->GetParticleName()
         << " " << primary->GetMomentum() << G4endl;

  if(fHHC1)
  {
    int n_hit = fHHC1->entries();
    G4cout << "Hodoscope 1 has " << n_hit << " hits." << G4endl;
    for(int i1=0;i1<n_hit;i1++)
    {
      HodoscopeHit* aHit = (*fHHC1)[i1];
      aHit->Print();
      analysis->FillIncident(aHit->GetPos());
    }
  }
  if(fHHC2)
  {
    int n_hit = fHHC2->entries();
    G4cout << "Hodoscope 2 has " << n_hit << " hits." << G4endl;
    for(int i1=0;i1<n_hit;i1++)
    {
      HodoscopeHit* aHit = (*fHHC2)[i1];
      aHit->Print();
    }
  }
  if(fDHC1)
  {
    int n_hit = fDHC1->entries();
    G4cout << "Drift Chamber 1 has " << n_hit << " hits." << G4endl;
    for(int i2=0;i2<5;i2++)
    {
      for(int i1=0;i1<n_hit;i1++)
      {
        DriftChamberHit* aHit = (*fDHC1)[i1];
        if(aHit->GetLayerID()==i2) aHit->Print();
      }
    }
  }
  if(fDHC2)
  {
    int n_hit = fDHC2->entries();
    G4cout << "Drift Chamber 2 has " << n_hit << " hits." << G4endl;
    for(int i2=0;i2<5;i2++)
    {
      for(int i1=0;i1<n_hit;i1++)
      {
        DriftChamberHit* aHit = (*fDHC2)[i1];
        if(aHit->GetLayerID()==i2) aHit->Print();
      }
    }
  }
  if(ECHC)
  {
    int iHit = 0;
    double totalE = 0.;
    for(int i1=0;i1<80;i1++)
    {
      EmCalorimeterHit* aHit = (*ECHC)[i1];
      double eDep = aHit->GetEdep();
      if(eDep>0.)
      {
        iHit++;
        totalE += eDep;
      }
    }
    G4cout << "EM Calorimeter has " << iHit << " hits. Total Edep is "
           << totalE/MeV << " (MeV)" << G4endl;
  }
  if(HCHC)
  {
    int iHit = 0;
    double totalE = 0.;
    for(int i1=0;i1<20;i1++)
    {
      HadCalorimeterHit* aHit = (*HCHC)[i1];
      double eDep = aHit->GetEdep();
      if(eDep>0.)
      {
        iHit++;
        totalE += eDep;
      }
    }
    G4cout << "Hadron Calorimeter has " << iHit << " hits. Total Edep is "
           << totalE/MeV << " (MeV)" << G4endl;
  }





}


