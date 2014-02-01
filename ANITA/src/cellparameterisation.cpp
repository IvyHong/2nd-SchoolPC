#include "cellparameterisation.h"

#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

CellParameterisation::CellParameterisation()
{
    // Initialise
    G4int i(0);

    for(i=0;i<100;i++)
    {
        G4int column = i/10;
        G4int row   = i%10;
        xCell.push_back((column-4)*10.*cm - 5.*cm);
        yCell.push_back((row-4)*10.*cm - 5.*cm);
    }
}

CellParameterisation::~CellParameterisation()
{}


void CellParameterisation::ComputeTransformation
        (const G4int copyNo, G4VPhysicalVolume *physVol) const
{
    physVol->SetTranslation(G4ThreeVector(xCell[copyNo],yCell[copyNo],0.));
}
