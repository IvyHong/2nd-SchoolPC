#include "CellParameterisation.h"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

CellParameterisation::CellParameterisation()
{
  for(G4int copyNo=0;copyNo<100;copyNo++)
  {
    G4int column = copyNo / 10;
    G4int row = copyNo % 10;
    fXCell[copyNo] = (column-5)*20.4*mm + 10.2*mm;
    fYCell[copyNo] = (row-5)*20.4*mm + 10.2*mm;
  }
}

CellParameterisation::~CellParameterisation()
{;}

void CellParameterisation::ComputeTransformation
(const G4int copyNo,G4VPhysicalVolume *physVol) const
{
  physVol->SetTranslation(G4ThreeVector(fXCell[copyNo],fYCell[copyNo],0.));
}
