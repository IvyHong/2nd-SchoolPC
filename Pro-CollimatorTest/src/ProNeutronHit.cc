#include "ProNeutronHit.hh"

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


G4Allocator<ProNeutronHit> ProNeutronHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProNeutronHit::ProNeutronHit()
{
    fID   = -1;
    fTime = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProNeutronHit::ProNeutronHit(G4int i)
{
    fID   = i;
    fTime = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProNeutronHit::~ProNeutronHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProNeutronHit::ProNeutronHit(const ProNeutronHit& right)
  : G4VHit()
{
  fID       = right.fID;
  fWorldPos = right.fWorldPos;
  fLocalPos = right.fLocalPos;
  fTime     = right.fTime;
  fKinE     = right.fKinE;
  fMomentum = right.fMomentum;
  fDirection= right.fDirection;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ProNeutronHit& ProNeutronHit::operator=(const ProNeutronHit& right)
{
    fID       = right.fID;
    fWorldPos = right.fWorldPos;
    fLocalPos = right.fLocalPos;
    fTime     = right.fTime;
    fKinE     = right.fKinE;
    fMomentum = right.fMomentum;
    fDirection= right.fDirection;

    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ProNeutronHit::operator==(const ProNeutronHit& /*right*/) const
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProNeutronHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fWorldPos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const std::map<G4String,G4AttDef>* ProNeutronHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("ProNeutronHit",isNew);
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

    G4String KinE("KinE");
    (*store)[KinE] = G4AttDef(KinE,"Kinetic Energy",
                              "Physics","G4BestUnit","G4double");

    G4String iMom("IMom");
    (*store)[iMom] = G4AttDef(iMom,"Momentum",
                              "Physics","G4BestUnit","G4ThreeVector");

    G4String iMag("IMag");
    (*store)[iMag] = G4AttDef(iMag,"Momentum magnitude",
                              "Physics","G4BestUnit","G4double");

  }
  return store;
}

std::vector<G4AttValue>* ProNeutronHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","ProNeutronHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(fID),""));

  values->push_back
    (G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(fWorldPos,"Length"),""));

  values->push_back
          (G4AttValue("KinE",G4BestUnit(fKinE,"Energy"),""));

  values->push_back
          (G4AttValue("IMom",G4BestUnit(fMomentum,"Energy"),""));

  values->push_back
          (G4AttValue("IMag",G4BestUnit(fMomentum.mag(),"Energy"),""));


  return values;
}




void ProNeutronHit::Print()
{
    G4cout << " ******************************************************* \n"
           << " Detector [ " << fID << "] : time " << fTime/ns << " (nsec) \n"
           << " Local (x,y,z) : " << fLocalPos.x() << ", "<< fLocalPos.y()
           << ", " << fLocalPos.z() << " \n"

           << " Momentum (u,v,w) : " << fMomentum.x() << ", "
           << fMomentum.y() << ", " << fMomentum.z() << " \n"

           << " Energy : " << fKinE/MeV << " (MeV) \n"

           << " Direction(X,Y,Z) " << fDirection
           << "***************************************************** " <<  G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

