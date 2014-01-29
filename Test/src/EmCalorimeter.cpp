#include "EmCalorimeter.h"
#include "EmCalorimeterHit.h"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

EmCalorimeter::EmCalorimeter(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="EMcalorimeterCollection");
  fHitsCollectionID = -1;
}

EmCalorimeter::~EmCalorimeter(){;}

void EmCalorimeter::Initialize(G4HCofThisEvent*HCE)
{
  // Creating hit collection
  // Create a new collection
  fHitsCollection = new EmCalorimeterHitsCollection
                   (SensitiveDetectorName,collectionName[0]);

  if(fHitsCollectionID<0)
  {
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }

  HCE->AddHitsCollection(fHitsCollectionID,fHitsCollection);

  // fill calorimeter hits with zero energy deposition
  for(G4int i=0;i<100;i++)
  {
    EmCalorimeterHit* aHit = new EmCalorimeterHit(i);
    fHitsCollection->insert( aHit );
  }
}

G4bool EmCalorimeter::ProcessHits(G4Step*aStep,G4TouchableHistory* /*ROhist*/)
{
  // Accumulating hit data
  // Get energy deposited in this step
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(0.==edep) return true;

  // Get volume and copy number
  //
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());

  G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
  G4int copyNo = thePhysical->GetCopyNo();

  // Get corresponding hit
  //
  EmCalorimeterHit* aHit = (*fHitsCollection)[copyNo];

  // check to see if this is first time the hit has updated
  if(!(aHit->GetLogV()))
  {
    // fill volume information
    aHit->SetLogV(thePhysical->GetLogicalVolume());

    G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
    aTrans.Invert();

    aHit->SetRot(aTrans.NetRotation());
    aHit->SetPos(aTrans.NetTranslation());

  }

  // Kinetic energy
  //
  aHit->SetKineticEnergy(preStepPoint->GetKineticEnergy());

  // Accumulate energy deposition
  aHit->AddEdep(edep);

  return true;
}

void EmCalorimeter::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}
