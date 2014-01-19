#ifndef CELLPARAMETERISATION_H
#define CELLPARAMETERISATION_H

#include "globals.hh"
#include "G4VPVParameterisation.hh"
class G4VPhysicalVolume;

class CellParameterisation : public G4VPVParameterisation
{
  public:
    CellParameterisation();
    virtual ~CellParameterisation();
    virtual void ComputeTransformation
                   (const G4int copyNo,G4VPhysicalVolume *physVol) const;

  private:
    G4double fXCell[100];
    G4double fYCell[100];
};

#endif // CELLPARAMETERISATION_H
