#ifndef CELLPARAMETERISATION_H
#define CELLPARAMETERISATION_H

#include "globals.hh"
#include "G4VPVParameterisation.hh"
#include <vector>

class G4VPhysicalVolume;

class CellParameterisation : public G4VPVParameterisation
{
  public:
    // Constructor
    //
    CellParameterisation();
    // Destructor
    //
    virtual ~CellParameterisation();
    virtual void ComputeTransformation
                   (const G4int copyNo,G4VPhysicalVolume *physVol) const;

  private:
    // Data members
    //
    std::vector<G4double> fXCell;
    std::vector<G4double> fYCell;
};

#endif // CELLPARAMETERISATION_H
