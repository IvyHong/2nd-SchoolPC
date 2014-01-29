#include "CellParameterisation.h"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

CellParameterisation::CellParameterisation()
{
    // Initialise
    G4int copyNo(0);

    for(copyNo=0;copyNo<100;copyNo++)
    {
      G4int column = copyNo / 10;
      G4int row = copyNo % 10;
      fXCell.push_back( (column-4)*10.*cm - 5*cm);
      fYCell.push_back( (row-4)*10.*cm - 5*cm);
    }
}

CellParameterisation::~CellParameterisation()
{;}

void CellParameterisation::ComputeTransformation
(const G4int copyNo,G4VPhysicalVolume *physVol) const
{
  physVol->SetTranslation(G4ThreeVector(fXCell[copyNo],fYCell[copyNo],0.));
}
