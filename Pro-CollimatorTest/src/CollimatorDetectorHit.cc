#include "CollimatorDetectorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<CollimatorDetectorHit> CollimatorDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CollimatorDetectorHit::CollimatorDetectorHit(G4int i)
{
    pID = i;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CollimatorDetectorHit::~CollimatorDetectorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CollimatorDetectorHit::CollimatorDetectorHit(const CollimatorDetectorHit& right)
  : G4VHit()
{
  pID = right.pID;
  pPosition = right.pPosition;
  pMomentum = right.pMomentum;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const CollimatorDetectorHit& CollimatorDetectorHit::operator=(const CollimatorDetectorHit& right)
{
    pID = right.pID;
    pPosition = right.pPosition;
    pMomentum = right.pMomentum;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int CollimatorDetectorHit::operator==(const CollimatorDetectorHit& right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CollimatorDetectorHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pPosition);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CollimatorDetectorHit::Print()
{
    G4cout << "****************************************************"
           << "\n Collimator Detector: " << pID << '\n'
           << " position: " << G4BestUnit(pPosition,"Length") << '\n'
           << " Momentum: " << G4BestUnit(pMomentum,"Energy") << '\n'
           << "***************************************************** " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
