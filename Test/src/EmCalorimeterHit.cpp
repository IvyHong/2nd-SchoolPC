#include "EmCalorimeterHit.h"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4Allocator<EmCalorimeterHit> EmCalorimeterHitAllocator;

EmCalorimeterHit::EmCalorimeterHit()
{
  fCellID = -1;
  fEdep = 0.;
  fPLogV = 0;
}

EmCalorimeterHit::EmCalorimeterHit(G4int z)
{
  fCellID = z;
  fEdep = 0.;
  fPLogV = 0;
}

EmCalorimeterHit::~EmCalorimeterHit()
{;}

EmCalorimeterHit::EmCalorimeterHit(const EmCalorimeterHit &right)
    : G4VHit() {
  fCellID = right.fCellID;
  fEdep = right.fEdep;
  fPos = right.fPos;
  fRot = right.fRot;
  fPLogV = right.fPLogV;
}

const EmCalorimeterHit& EmCalorimeterHit::operator=(const EmCalorimeterHit &right)
{
  fCellID = right.fCellID;
  fEdep = right.fEdep;
  fPos = right.fPos;
  fRot = right.fRot;
  fPLogV = right.fPLogV;
  return *this;
}

int EmCalorimeterHit::operator==(const EmCalorimeterHit &right) const
{
  return (fCellID==right.fCellID);
}

void EmCalorimeterHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager&&(fEdep>0.))
  {
    // Draw a calorimeter cell with a color corresponding to its energy deposit
    G4Transform3D trans(fRot.inverse(),fPos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = fPLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4double rcol = fEdep/(0.7*GeV);
    if(rcol>1.) rcol = 1.;
    if(rcol<0.4) rcol = 0.4;
    G4Colour colour(rcol,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*fPLogV,attribs,trans);
  }
}

const std::map<G4String,G4AttDef>* EmCalorimeterHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("EmCalorimeterHit",isNew);
  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type","Physics","","G4String");

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"ID","Physics","","G4int");

    G4String Energy("Energy");
    (*store)[Energy] = G4AttDef(Energy,"Energy Deposited","Physics","G4BestUnit","G4double");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position",
                      "Physics","G4BestUnit","G4ThreeVector");

    G4String LVol("LVol");
    (*store)[LVol] = G4AttDef(LVol,"Logical Volume","Physics","","G4String");
  }
  return store;
}

std::vector<G4AttValue>* EmCalorimeterHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","EmCalorimeterHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(fCellID),""));

  values->push_back
    (G4AttValue("Energy",G4BestUnit(fEdep,"Energy"),""));

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

void EmCalorimeterHit::Print()
{
  G4cout << " Cell[" << fCellID << "] " << fEdep/MeV << " (MeV)" << G4endl;
}


