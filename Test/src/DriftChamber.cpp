#include "DriftChamber.h"
#include "DriftChamberHit.h"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4ios.hh"

DriftChamber::DriftChamber(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="driftChamberColl");
  fHCID = -1;
}

DriftChamber::~DriftChamber(){;}

void DriftChamber::Initialize(G4HCofThisEvent*HCE)
{
  fHitsCollection = new DriftChamberHitsCollection
                   (SensitiveDetectorName,collectionName[0]);
  if(fHCID<0)
  { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  HCE->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool DriftChamber::ProcessHits(G4Step*aStep,G4TouchableHistory* /*ROhist*/)
{
  G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
  if(charge==0.) return true;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4VPhysicalVolume* theMotherPhysical = theTouchable->GetVolume(1); // mother
  G4int copyNo = theMotherPhysical->GetCopyNo();
  G4ThreeVector worldPos = preStepPoint->GetPosition();
  G4ThreeVector localPos
    = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);

  DriftChamberHit* aHit = new DriftChamberHit(copyNo);
  aHit->SetWorldPos(worldPos);
  aHit->SetLocalPos(localPos);
  aHit->SetTime(preStepPoint->GetGlobalTime());

  fHitsCollection->insert(aHit);

  return true;
}

void DriftChamber::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}

