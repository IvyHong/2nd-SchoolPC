#include "Monitor.h"
#include "MonitorHit.h"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4ios.hh"

Monitor::Monitor(G4String name)
    : G4VSensitiveDetector(name),
      fHitsCollectionID (-1)
{
  G4String HCname;
  collectionName.insert(HCname="MonitorCollection");

}

Monitor::~Monitor()
{

}

void Monitor::Initialize(G4HCofThisEvent*HCE)
{
  // Creating hit collection
  // Create a new collection
  fHitsCollection = new MonitorHitsCollection
                   (SensitiveDetectorName,collectionName[0]);
  if(fHitsCollectionID<0)
  { fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }

  // Add collection to the event
  //
  HCE->AddHitsCollection(fHitsCollectionID,fHitsCollection);

}

G4bool Monitor::ProcessHits(G4Step* aStep,G4TouchableHistory* /*ROhist*/)
{
    // Get Material
    //
    G4String thisVolume = aStep->GetTrack()->GetVolume()->GetName() ;
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
    G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();

    if (thisVolume != "Monitor_Physical") return false;
    if (particleName != "neutron" ) return false;
    if (charge != 0.) return false;

    G4double edep = aStep->GetTotalEnergyDeposit();
    if(edep<=0.) return false;

    // Limited by Geometry Boundary
    //
    if(aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
    {

        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

        // Get Transportation Matrix
        //
        G4TouchableHistory* theTouchable
                = (G4TouchableHistory*)(preStepPoint->GetTouchable());
        G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
        G4VPhysicalVolume* theMotherPhysical = theTouchable->GetVolume(); // here 0 means world is mother
        aTrans.Invert();

        G4ThreeVector Position = aTrans.NetRotation() * (aStep->GetPreStepPoint()->GetPosition() - aTrans.NetTranslation());
        G4ThreeVector momentumD = aTrans.NetRotation() * aStep->GetPreStepPoint()->GetMomentumDirection();
        G4int copyNo = theMotherPhysical->GetCopyNo();
        G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
        G4double kEnergy = preStepPoint->GetKineticEnergy();


        // Create Hit
        //
        MonitorHit* aHit = new MonitorHit(copyNo);
        aHit->SetWorldPos(Position);
        aHit->SetTime(preStepPoint->GetGlobalTime());
        aHit->SetIncidenceDefinition(pd);
        aHit->SetIncidenceKinEnergy(kEnergy);
        aHit->SetIncidenceMomentumDirection(momentumD);

        fHitsCollection->insert(aHit);
    }
    else
    {
        return true;
    }

    return true;
}

void Monitor::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}

