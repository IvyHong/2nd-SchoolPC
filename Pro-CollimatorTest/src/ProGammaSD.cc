#include "ProGammaSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4Material.hh"
#include "G4TouchableHandle.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProGammaSD::ProGammaSD(const G4String &name)
:G4VSensitiveDetector(name),
  gHitsCollection(0)
{
    collectionName.insert("GAMMAHitsCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProGammaSD::~ProGammaSD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProGammaSD::Initialize(G4HCofThisEvent* HCE)
{
  gHitsCollection = new ProGammaHitsCollection
                          (SensitiveDetectorName,collectionName[0]);
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, gHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ProGammaSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  // Get Material

  G4String thisVolume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();


  if (thisVolume != "GammaSD") return false;
  if (particleName != "gamma" ) return false;

  if(particleName == "gamma"&& thisVolume=="GammaSD") aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  ProGammaHit* gammaNewHit = new ProGammaHit();

  gammaNewHit->SetPosition (aStep->GetPostStepPoint()->GetPosition());  //global coordinates
  gammaNewHit->SetMomentum (aStep->GetTrack()->GetMomentum());
  gHitsCollection->insert  ( gammaNewHit );

  //gammaNewHit->Print();
  //gammaNewHit->Draw();

//  G4cout << "*******************************" << G4endl;
//  G4cout << "             Gamma HIT          " << G4endl;
//  G4cout << "  Volume:                      " << thisVolume << G4endl;
//  G4cout << "  POSITION (m) :              "
//         << aStep->GetPostStepPoint()->GetPosition().x()/m << " " << aStep->GetPostStepPoint()->GetPosition().y()/m
//         << " " << aStep->GetPostStepPoint()->GetPosition().z()/m << G4endl;
//  G4cout << "  Momentum (MeV) : "
//         << aStep->GetTrack()->GetMomentum()/MeV  << G4endl;
//  G4cout << "***********************************" << G4endl;

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProGammaSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  static G4int HCID = -1;
  if(HCID<0)
    {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
  HCE->AddHitsCollection(HCID,gHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
