#include "HadCalorimeter.h"
#include "HadCalorimeterHit.h"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

HadCalorimeter::HadCalorimeter(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="HadCalorimeterColl");
  fHCID = -1;
}

HadCalorimeter::~HadCalorimeter(){;}

void HadCalorimeter::Initialize(G4HCofThisEvent*HCE)
{
  fHitsCollection = new HadCalorimeterHitsCollection
                   (SensitiveDetectorName,collectionName[0]);
  if(fHCID<0)
  { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  HCE->AddHitsCollection(fHCID,fHitsCollection);

  // fill calorimeter hits with zero energy deposition
  for(int iColumn=0;iColumn<10;iColumn++)
  for(int iRow=0;iRow<2;iRow++)
  {
    HadCalorimeterHit* aHit = new HadCalorimeterHit();
    fHitsCollection->insert( aHit );
  }
}

G4bool HadCalorimeter::ProcessHits(G4Step*aStep,G4TouchableHistory* /*ROhist*/)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return true;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4VPhysicalVolume* theCellPhysical = theTouchable->GetVolume(2);
  G4int rowNo = theCellPhysical->GetCopyNo();
  G4VPhysicalVolume* theColumnPhysical = theTouchable->GetVolume(3);
  G4int columnNo = theColumnPhysical->GetCopyNo();
  G4int hitID = 2*columnNo+rowNo;
  HadCalorimeterHit* aHit = (*fHitsCollection)[hitID];

  // check if it is first touch
  if(aHit->GetColumnID()<0)
  {
    aHit->SetColumnID(columnNo);
    aHit->SetRowID(rowNo);
    G4int depth = theTouchable->GetHistory()->GetDepth();
    G4AffineTransform aTrans = theTouchable->GetHistory()->GetTransform(depth-2);
    aTrans.Invert();
    aHit->SetRot(aTrans.NetRotation());
    aHit->SetPos(aTrans.NetTranslation());
  }
  // add energy deposition
  aHit->AddEdep(edep);

  return true;
}

void HadCalorimeter::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}
