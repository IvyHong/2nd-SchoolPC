#include "EmCalorimeterHit.h"
#include "PrimaryGeneratorAction.h"

#include "G4ParticleGun.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4Allocator<EmCalorimeterHit> EmCalorimeterHitAllocator;

EmCalorimeterHit::EmCalorimeterHit()
    : fCellID(-1),
      fKinE(0.),
      fEdep(0.),
      fPD(0),
      fPos(),
      fRot(),
      fPLogV(0)
{}

EmCalorimeterHit::EmCalorimeterHit(G4int z)
    : fCellID(z),
      fKinE(0.),
      fEdep(0.),
      fPD(0),
      fPos(),
      fRot(),
      fPLogV(0)
{}

EmCalorimeterHit::~EmCalorimeterHit()
{;}

EmCalorimeterHit::EmCalorimeterHit(const EmCalorimeterHit &right)
    : G4VHit() {
  fCellID = right.fCellID;
  fKinE = right.fKinE;
  fEdep = right.fEdep;
  fPD   = right.fPD;
  fPos = right.fPos;
  fRot = right.fRot;
  fPLogV = right.fPLogV;
}

const EmCalorimeterHit& EmCalorimeterHit::operator=(const EmCalorimeterHit &right)
{
  fCellID = right.fCellID;
  fKinE = right.fKinE;
  fEdep = right.fEdep;
  fPD   = right.fPD;
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

/// Geant4 Hands-on 4 Storing Hits

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager&&(fEdep>0.))
  {
    // Draw a box with depth propotional to the energy deposition
    G4double scaleE = PrimaryGeneratorAction::Gun()->GetParticleEnergy();
    G4double depth = (50.*cm)*(fEdep*MeV/scaleE*MeV);

    // Back face of box is flat against front face of calorimeter cell
    //
    G4double z = fPos.z() + 25.*cm;
    G4ThreeVector currentPos (fPos.x(),fPos.y(),z+depth);

    G4Transform3D trans (fRot.inverse(), currentPos);
    G4VisAttributes attribs;

    // Magenta with transparency
    G4Colour colour (1.,0.,1.,0.6);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);

    G4Box box("MyBox",5.*cm,5.*cm,depth);
    pVVisManager->Draw(box,attribs,trans);
  }

/// Examples/extended/analysis/A01
/*
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
*/
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

    G4String Column("Column");
    (*store)[Column] = G4AttDef(Column,"Column ID", "Physics","","G4int");

    G4String Row("Row");
    (*store)[Row] = G4AttDef(Row,"Row ID", "Physics","","G4int");

    G4String DepEnergy("DepEnergy");
    (*store)[DepEnergy] = G4AttDef(DepEnergy,"Energy Deposited","Physics","G4BestUnit","G4double");

    G4String Energy("Energy");
    (*store)[Energy] = G4AttDef(Energy,"Kinetic Energy","Physics","G4BestUnit","G4double");

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
    (G4AttValue("DepEnergy",G4BestUnit(fEdep,"DepEnergy"),""));

  values->push_back
    (G4AttValue("Energy",G4BestUnit(fKinE,"Energy"),""));

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

  G4cout << " Kinetic Energy __ Cell[" << fCellID << "]"  << fKinE/MeV << " (MeV)" << G4endl;
}


