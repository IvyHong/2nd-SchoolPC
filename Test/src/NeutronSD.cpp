#include "NeutronSD.h"
#include "NeutronHit.h"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronSD::NeutronSD(const G4String &name)
    :G4VSensitiveDetector(name),
      nHitsCollection(0)
{
    collectionName.insert("NEUTRONHitsCollection");
    nHCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronSD::~NeutronSD(){ ;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronSD::Initialize(G4HCofThisEvent* HCE)
{
  nHitsCollection = new NeutronHitsCollection
                          (SensitiveDetectorName,collectionName[0]);

  if(nHCID<0)
  { nHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( nHCID, nHitsCollection );

  // fill each neutron cell hits with zero energy deposition
  for(G4int i=0;i<100;i++)
  {
    NeutronHit* aHit = new NeutronHit(i);
    nHitsCollection->insert( aHit );
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NeutronSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

   G4double edep = aStep->GetTotalEnergyDeposit();
   if(edep==0.) return true;

   G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
   G4TouchableHistory* theTouchable
        = (G4TouchableHistory*)(preStepPoint->GetTouchable());
   G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
   G4int copyNo = thePhysical->GetCopyNo();

   NeutronHit* aHit = (*nHitsCollection)[copyNo];

   // check if it is first touch
   if(!(aHit->GetLogV()))
   {
     // fill volume information
     aHit->SetLogV(thePhysical->GetLogicalVolume());
     G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
     aTrans.Invert();
     aHit->SetRot(aTrans.NetRotation());
     aHit->SetPosition(aTrans.NetTranslation());
   }
   // add energy deposition
   aHit->SetEnergyDeposite(edep);
   aHit->SetPosition (preStepPoint->GetPosition());
   aHit->SetMomentum (aStep->GetTrack()->GetMomentum());


/*
    G4double edep = aStep->GetTotalEnergyDeposit();
    if(edep==0.) return true;

    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHistory* theTouchable
      = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
    G4int copyNo = thePhysical->GetCopyNo();

  if(particleName == "neutron"&& thisVolume=="NeutronSD1")
  {
     /// aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  // check if this finger already has a hit
  G4int ix = -1;
  for(G4int i=0;i<nHitsCollection->entries();i++)
  {
    if((*nHitsCollection)[i]->GetID()==copyNo)
    {
      ix = i;
      break;
    }
  }

  NeutronHit* neutronNewHit = new NeutronHit(copyNo);

  neutronNewHit->SetPosition (aStep->GetPostStepPoint()->GetPosition()); //global coordinates
  neutronNewHit->SetMomentum (aStep->GetTrack()->GetMomentum());
  nHitsCollection->insert ( neutronNewHit );}

  //neutronNewHit->Print();
  //neutronNewHit->Draw();

// G4cout << "*******************************" << G4endl;
// G4cout << " neutron HIT " << G4endl;
// G4cout << " Volume: " << thisVolume << G4endl;
// G4cout << " POSITION (m) : "
// << aStep->GetPostStepPoint()->GetPosition().x()/m << " " << aStep->GetPostStepPoint()->GetPosition().y()/m
// << " " << aStep->GetPostStepPoint()->GetPosition().z()/m << G4endl;
// G4cout << " Momentum (MeV): "
// << aStep->GetTrack()->GetMomentum()/MeV << G4endl;
// G4cout << "*******************************" << G4endl;

*/

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronSD::EndOfEvent(G4HCofThisEvent*/* HCE*/)
{
    ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
