#include "ProNeutronSD.hh"
#include "ProNeutronHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProNeutronSD::ProNeutronSD(const G4String &name)
:G4VSensitiveDetector(name),
  fHitsCollection(0)
{
    collectionName.insert("NEUTRONHitsCollection");
    fHCID = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProNeutronSD::~ProNeutronSD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProNeutronSD::Initialize(G4HCofThisEvent* HCE)
{
  fHitsCollection = new ProNeutronHitsCollection
                          (SensitiveDetectorName,collectionName[0]);

  if(fHCID<0)
  {
      fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  }
  HCE->AddHitsCollection( fHCID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ProNeutronSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
    // Get particle name
   G4String particleType;
   particleType = aStep->GetTrack()->GetDynamicParticle()
           ->GetDefinition()->GetParticleName();

   G4String thisVolume = aStep->GetPreStepPoint()
           ->GetTouchableHandle()->GetVolume()->GetName();

   G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();

   if(charge==0.) return true;
   if(particleType=="neutron") return true;
   if(thisVolume=="ChamberSD") return true;

   G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
   G4TouchableHistory* theTouchable
     = (G4TouchableHistory*)(preStepPoint->GetTouchable());
   G4VPhysicalVolume* theMotherPhysical = theTouchable->GetVolume(0); // mother
   G4int copyNo = theMotherPhysical->GetCopyNo();

   G4ThreeVector worldPos = preStepPoint->GetPosition();
   G4ThreeVector localPos
     = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);

   ProNeutronHit* aHit = new ProNeutronHit(copyNo);
   aHit->SetWorldPos(worldPos);
   aHit->SetLocalPos(localPos);
   aHit->SetTime(preStepPoint->GetGlobalTime());
   aHit->SetKinE(aStep->GetTrack()->GetKineticEnergy());
   aHit->SetMomentum(aStep->GetTrack()->GetDynamicParticle()->GetMomentum());
   aHit->SetDirection(aStep->GetTrack()->GetVertexMomentumDirection());

   fHitsCollection->insert(aHit);

/// ********************************************************************
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProNeutronSD::EndOfEvent(G4HCofThisEvent*/* HCE*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
