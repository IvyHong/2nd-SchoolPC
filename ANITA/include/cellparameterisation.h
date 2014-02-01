#ifndef CELLPARAMETERISATION_H
#define CELLPARAMETERISATION_H

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicalVolume;

class CellParameterisation : public G4VPVParameterisation
{
public:
    // Constructor
    CellParameterisation();
    // Destructor
    virtual ~CellParameterisation();

    // Method
    virtual ComputeTransformation(const G4int, G4VPhysicalVolume *) const;

private:

    // Data members
    std::vector<G4double> xCell;
    std::vector<G4double> yCell;
};

#endif // CELLPARAMETERISATION_H
