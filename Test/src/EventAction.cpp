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

#include "MonitorHit.h"
#include "EmCalorimeterHit.h"


EventAction::EventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* )
{
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    // Getting code for HitsCollection of EMCalorimeter
    //
    fHitsCollectionID = SDman->GetCollectionID("EMcalorimeterCollection");
    // Getting code for HitsCollection of Silicon Monitor
    //
    fHitsCollectionID_monitor = SDman->GetCollectionID("MonitorCollection");
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
    HistoManager* myAnalysis = HistoManager::GetAnalysis();

    // HCE : HitsCollectionOfThisEvent
    G4HCofThisEvent * HCE = evt->GetHCofThisEvent();

    if(fHitsCollectionID >=0)
    {
        EmCalorimeterHitsCollection* hitsCollection =
          dynamic_cast<EmCalorimeterHitsCollection*>(HCE->GetHC(fHitsCollectionID));

        G4double totalEnergy = 0.;

        if ( 0 != hitsCollection ) {
           G4int i(0);

           for ( i=0 ; i<100 ; i++ ) {
              EmCalorimeterHit* aHit = (*hitsCollection)[i];

              myAnalysis->FillHisto(1,aHit->GetKineticEnergy());
              myAnalysis->FillHisto(2,aHit->GetEdep());
              totalEnergy += aHit->GetEdep();

              aHit->Print();
           }
        }
        G4cout<<"Energy deposited in calorimeter "<<totalEnergy/MeV<<" MeV"<<G4endl;
     }


     ////////////////////////////////////////////////////////////////////////
     // HandsOn4: Output code of Silicon Monitor Hits
     if ( fHitsCollectionID_monitor >= 0 )
     {
        MonitorHitsCollection* hitsCollection_monitor =
           dynamic_cast<MonitorHitsCollection*>(HCE->GetHC(fHitsCollectionID_monitor));

         G4int numberOfHits = hitsCollection_monitor->GetSize();
         for ( int i = 0 ; i < numberOfHits ; i++ )
         {
            MonitorHit* aHit = (*hitsCollection_monitor)[i];
            G4cout << "Information of " << i+1 << " Monitor Hit of this event." << G4endl;
            aHit->Print();
         }
      }

}


