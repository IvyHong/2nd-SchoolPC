#include "DriftChamberHit.h"
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

G4Allocator<DriftChamberHit> DriftChamberHitAllocator;

DriftChamberHit::DriftChamberHit()
{
  fLayerID = -1;
  fTime = 0.;
}

DriftChamberHit::DriftChamberHit(G4int z)
{
  fLayerID = z;
  fTime = 0.;
}

DriftChamberHit::~DriftChamberHit()
{;}

DriftChamberHit::DriftChamberHit(const DriftChamberHit &right)
    : G4VHit() {
  fLayerID = right.fLayerID;
  fWorldPos = right.fWorldPos;
  fLocalPos = right.fLocalPos;
  fTime = right.fTime;
}

const DriftChamberHit& DriftChamberHit::operator=(const DriftChamberHit &right)
{
  fLayerID = right.fLayerID;
  fWorldPos = right.fWorldPos;
  fLocalPos = right.fLocalPos;
  fTime = right.fTime;
  return *this;
}

int DriftChamberHit::operator==(const DriftChamberHit &/*right*/) const
{
  return 0;
}

void DriftChamberHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fWorldPos);
    circle.SetScreenSize(2);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

const std::map<G4String,G4AttDef>* DriftChamberHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("DriftChamberHit",isNew);
  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type","Physics","","G4String");

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"ID","Physics","","G4int");

    G4String Time("Time");
    (*store)[Time] = G4AttDef(Time,"Time","Physics","G4BestUnit","G4double");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position",
                      "Physics","G4BestUnit","G4ThreeVector");
  }
  return store;
}

std::vector<G4AttValue>* DriftChamberHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","DriftChamberHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(fLayerID),""));

  values->push_back
    (G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(fWorldPos,"Length"),""));

  return values;
}

void DriftChamberHit::Print()
{
  G4cout << " Layer[" << fLayerID << "] : time " << fTime/ns
         << " (nsec) --- local (x,y) " << fLocalPos.x()
         << ", " << fLocalPos.y() << G4endl;
}
