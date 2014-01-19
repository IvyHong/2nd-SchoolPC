#include "NeutronHit.h"

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

G4Allocator<NeutronHit> NeutronHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHit::NeutronHit()
{
    nID = -1;
    nEnergyDeposit = 0.;
    nPLogV = 0;
}


NeutronHit::NeutronHit(G4int z)
{
  nID = z;
  nEnergyDeposit = 0.;
  nPLogV = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHit::~NeutronHit() {;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHit::NeutronHit(const NeutronHit& right)
  : G4VHit()
{
  nID = right.nID;
  nPosition = right.nPosition;
  nMomentum = right.nMomentum;
  nEnergyDeposit = right.nEnergyDeposit;
  nRot = right.nRot;
  nPLogV = right.nPLogV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const NeutronHit& NeutronHit::operator=(const NeutronHit& right)
{
    nID = right.nID;
    nPosition = right.nPosition;
    nMomentum = right.nMomentum;
    nEnergyDeposit = right.nEnergyDeposit;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int NeutronHit::operator==(const NeutronHit& right) const
{
  return (nID==right.nID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager&&(nEnergyDeposit>0.))
  {
    // Draw a calorimeter cell with a color corresponding to its energy deposit
    G4Transform3D trans(nRot.inverse(),nPosition);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = nPLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4double rcol = nEnergyDeposit/(0.7*GeV);
    if(rcol>1.) rcol = 1.;
    if(rcol<0.4) rcol = 0.4;
    G4Colour colour(rcol,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*nPLogV,attribs,trans);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronHit::Print()
{
    G4cout << "  Cell[" << nID << "] "
           << nEnergyDeposit/MeV << " (MeV)" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


const std::map<G4String,G4AttDef>* NeutronHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("NeutronHit",isNew);
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

std::vector<G4AttValue>* NeutronHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","NeutronHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(nID),""));

  values->push_back
    (G4AttValue("Energy",G4BestUnit(nEnergyDeposit,"Energy"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(nPosition,"Length"),""));

  if (nPLogV)
    values->push_back
      (G4AttValue("LVol",nPLogV->GetName(),""));
  else
    values->push_back
      (G4AttValue("LVol"," ",""));

   return values;
}
