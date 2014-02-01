#include "MonitorHit.h"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"


G4Allocator<MonitorHit> MonitorHitAllocator;

MonitorHit::MonitorHit()
{
  fNoID = -1;
  fTime = 0.;
  fPD   = 0;
  fKinEnergy = 0.;
  fWorldPos  = G4ThreeVector(0);
  fMomentumD = G4ThreeVector(0);

}

MonitorHit::MonitorHit(G4int z)
{
  fNoID = z;
  fTime = 0.;
  fPD   = 0;
  fKinEnergy = 0.;
  fWorldPos  = G4ThreeVector(0);
  fMomentumD = G4ThreeVector(0);
}

MonitorHit::~MonitorHit()
{;}

MonitorHit::MonitorHit(const MonitorHit &right)
    : G4VHit() {
  fNoID     = right.fNoID;
  fWorldPos = right.fWorldPos;
  fTime     = right.fTime;
  fPD       = right.fPD;
  fKinEnergy= right.fKinEnergy;
  fMomentumD= right.fMomentumD;

}

const MonitorHit& MonitorHit::operator=(const MonitorHit &right)
{
  fNoID     = right.fNoID;
  fWorldPos = right.fWorldPos;
  fTime     = right.fTime;
  fPD       = right.fPD;
  fKinEnergy= right.fKinEnergy;
  fMomentumD= right.fMomentumD;

  return *this;
}

int MonitorHit::operator==(const MonitorHit &/*right*/) const
{
  return 0;
}

void MonitorHit::Draw()
{}

const std::map<G4String,G4AttDef>* MonitorHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("MonitorHit",isNew);
  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type","Physics","","G4String");

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"ID","Physics","","G4int");

    G4String Time("Time");
    (*store)[Time] = G4AttDef(Time,"Time","Physics","G4BestUnit","G4double");

    G4String Energy("Energy");
    (*store)[Energy] = G4AttDef(Energy, "Kinetic Energy", "Physics",
                "G4BestUnit", "G4double");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position",
                      "Physics","G4BestUnit","G4ThreeVector");

  }
  return store;
}

std::vector<G4AttValue>* MonitorHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","MonitorHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(fNoID),""));

  values->push_back
    (G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

  values->push_back
    (G4AttValue("Energy", G4BestUnit(fKinEnergy, "Energy"), ""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(fWorldPos,"Length"),""));

  return values;
}

void MonitorHit::Print()
{
   G4cout << "*********************************************************" << G4endl;
   G4cout << "Time : " << fTime/ns << '\n'
          << "Incidence Particle Name and Kinetic Energy " << fPD->GetParticleName()
          << " " << fKinEnergy/MeV << " MeV" << G4endl;
   G4cout << "Insidence position in monitor [" << fNoID << "] : " << fWorldPos/mm
          << " in mm" << G4endl;
   G4cout << "Incidence Direction " << fMomentumD << G4endl;
   G4cout << "*********************************************************" << G4endl;
}

