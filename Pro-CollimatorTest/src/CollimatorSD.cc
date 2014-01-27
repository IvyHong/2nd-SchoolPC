#include "CollimatorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4Material.hh"
#include "G4TouchableHandle.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CollimatorSD::CollimatorSD(const G4String &name)
:G4VSensitiveDetector(name),
  pHitsCollection(0)
{
    collectionName.insert("NEUTRONHitsCollection");
    pHCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CollimatorSD::~CollimatorSD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CollimatorSD::Initialize(G4HCofThisEvent* HCE)
{
  pHitsCollection = new CollimatorDetectorHitsCollection
                          (SensitiveDetectorName,collectionName[0]);

  if(pHCID<0)
  { pHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( pHCID, pHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CollimatorSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  // Get Material

  G4String thisVolume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

  if (thisVolume != "CollimatorSD") return false;
  if (particleName != "neutron" ) return false;
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
   G4TouchableHistory* theTouchable
     = (G4TouchableHistory*)(preStepPoint->GetTouchable());
   G4int copyNo = theTouchable->GetVolume()->GetCopyNo();

  if(particleName == "neutron"&& thisVolume=="CollimatorSD")

  {
// aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  // check if this finger already has a hit
  G4int ix = -1;
  for(G4int i=0;i<pHitsCollection->entries();i++)
  {
    if((*pHitsCollection)[i]->GetID()==copyNo)
    {
      ix = i;
      break;
    }
  }

  CollimatorDetectorHit* protonNewHit = new CollimatorDetectorHit(copyNo);

  protonNewHit->SetPosition (aStep->GetPostStepPoint()->GetPosition()); //global coordinates
  protonNewHit->SetMomentum (aStep->GetTrack()->GetMomentum());
  pHitsCollection->insert ( protonNewHit );

// protonNewHit->Print();
// protonNewHit->Draw();
  }

// G4cout << "*******************************" << G4endl;
// G4cout << " neutron HIT " << G4endl;
// G4cout << " Volume: " << thisVolume << G4endl;
// G4cout << " POSITION (m) : "
// << aStep->GetPostStepPoint()->GetPosition().x()/m << " " << aStep->GetPostStepPoint()->GetPosition().y()/m
// << " " << aStep->GetPostStepPoint()->GetPosition().z()/m << G4endl;
// G4cout << " Momentum (MeV): "
// << aStep->GetTrack()->GetMomentum()/MeV << G4endl;
// G4cout << "*******************************" << G4endl;

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CollimatorSD::EndOfEvent(G4HCofThisEvent*/* HCE*/)
{
// static G4int HCID = -1;
// if(HCID<0)
// {
// HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
// }
// HCE->AddHitsCollection(HCID,nHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
