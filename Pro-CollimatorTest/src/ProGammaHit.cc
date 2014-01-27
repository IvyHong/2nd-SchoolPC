#include "ProGammaHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<ProGammaHit> ProGammaHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProGammaHit::ProGammaHit()
    : G4VHit(),
      gPosition(0.),
      gMomentum(0.)
//Initialize position and momentum
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProGammaHit::~ProGammaHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProGammaHit::ProGammaHit(const ProGammaHit& right)
    : G4VHit()
{
    gPosition       = right.gPosition;
    gMomentum       = right.gMomentum;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ProGammaHit& ProGammaHit::operator=(const ProGammaHit& right)
{
    gPosition       = right.gPosition;
    gMomentum       = right.gMomentum;

    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ProGammaHit::operator==(const ProGammaHit& right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProGammaHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(gPosition);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProGammaHit::Print()
{
    G4cout << "****************************************************"
           << "\n "
         << "  position: " << G4BestUnit(gPosition,"Length") << '\n'
         << "  Momentum: " << G4BestUnit(gMomentum,"Energy") << '\n'
         << "******************************************************" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

