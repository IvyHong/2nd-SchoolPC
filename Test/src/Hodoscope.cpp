#include "Hodoscope.h"
#include "HodoscopeHit.h"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

Hodoscope::Hodoscope(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="hodoscopeColl");
  fHCID = -1;
}

Hodoscope::~Hodoscope(){;}

void Hodoscope::Initialize(G4HCofThisEvent*HCE)
{
  fHitsCollection = new HodoscopeHitsCollection
                   (SensitiveDetectorName,collectionName[0]);
  if(fHCID<0)
  { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  HCE->AddHitsCollection(fHCID,fHitsCollection);
}


G4bool Hodoscope::ProcessHits(G4Step*aStep,G4TouchableHistory* /*ROhist*/)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return true;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = theTouchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();

  // check if this finger already has a hit
  G4int ix = -1;
  for(G4int i=0;i<fHitsCollection->entries();i++)
  {
    if((*fHitsCollection)[i]->GetID()==copyNo)
    {
      ix = i;
      break;
    }
  }
  // if it has, then take the earlier time
  if(ix>=0)
  {
    if((*fHitsCollection)[ix]->GetTime()>hitTime)
    { (*fHitsCollection)[ix]->SetTime(hitTime); }
  }
  else
  // if not, create a new hit and set it to the collection
  {
    HodoscopeHit* aHit = new HodoscopeHit(copyNo,hitTime);
    G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
    aHit->SetLogV(thePhysical->GetLogicalVolume());
    G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
    aTrans.Invert();
    aHit->SetRot(aTrans.NetRotation());
    aHit->SetPos(aTrans.NetTranslation());
    fHitsCollection->insert( aHit );
  }

  return true;
}

void Hodoscope::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}
