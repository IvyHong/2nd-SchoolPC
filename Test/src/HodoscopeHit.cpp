#include "HodoscopeHit.h"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4Allocator<HodoscopeHit> HodoscopeHitAllocator;

HodoscopeHit::HodoscopeHit(G4int i,G4double t)
{
  fId = i;
  fTime = t;
  fPLogV = 0;
}

HodoscopeHit::~HodoscopeHit()
{;}

HodoscopeHit::HodoscopeHit(const HodoscopeHit &right)
    : G4VHit() {
  fId = right.fId;
  fTime = right.fTime;
  fPos = right.fPos;
  fRot = right.fRot;
  fPLogV = right.fPLogV;
}

const HodoscopeHit& HodoscopeHit::operator=(const HodoscopeHit &right)
{
  fId = right.fId;
  fTime = right.fTime;
  fPos = right.fPos;
  fRot = right.fRot;
  fPLogV = right.fPLogV;
  return *this;
}

int HodoscopeHit::operator==(const HodoscopeHit &/*right*/) const
{
  return 0;
}

void HodoscopeHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Transform3D trans(fRot.inverse(),fPos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = fPLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4Colour colour(0.,1.,1.);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*fPLogV,attribs,trans);
  }
}

const std::map<G4String,G4AttDef>* HodoscopeHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("HodoscopeHit",isNew);
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

    G4String LVol("LVol");
    (*store)[LVol] = G4AttDef(LVol,"Logical Volume","Physics","","G4String");
  }
  return store;
}

std::vector<G4AttValue>* HodoscopeHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","HodoscopeHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(fId),""));

  values->push_back
    (G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));

  if (fPLogV)
    values->push_back
      (G4AttValue("LVol",fPLogV->GetName(),""));
  else
    values->push_back
      (G4AttValue("LVol"," ",""));

  return values;
}

void HodoscopeHit::Print()
{
    G4cout << " Hodoscope[" << fId << "] " << fTime/ns << " (nsec)" << G4endl;
}
