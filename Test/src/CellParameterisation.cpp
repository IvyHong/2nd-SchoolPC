#include "CellParameterisation.h"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

CellParameterisation::CellParameterisation()
{
    for(int copyNo=0;copyNo<80;copyNo++)
    {
      G4int column = copyNo / 4;
      G4int row = copyNo % 4;
      fXCell[copyNo] = (column-9)*15.*cm - 7.5*cm;
      fYCell[copyNo] = (row-1)*15*cm - 7.5*cm;
    }
}

CellParameterisation::~CellParameterisation()
{;}

void CellParameterisation::ComputeTransformation
(const G4int copyNo,G4VPhysicalVolume *physVol) const
{
  physVol->SetTranslation(G4ThreeVector(fXCell[copyNo],fYCell[copyNo],0.));
}
