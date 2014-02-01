#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

class G4LogicalVolume;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    // Constructor
    DetectorConstruction();
    // Destructor
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

private:

    // Helper methods
    void DefineMaterials();
    void SetupGeometry();

private:
    // Logical volume
    //
    G4LogicalVolume* fWorldLog;
    G4LogicalVolume* fTargetLog;
    G4LogicalVolume* fCalorimeterLog;
    G4LogicalVolume* fCollimatorLog;

    // Physical volume
    //
    G4VPhysicalVolume* fWorldPhys;
    G4VPhysicalVolume* fTargetPhys;
    G4VPhysicalVolume* fCalorimeterPhys;
    G4VPhysicalVolume* fCollimatorPhys;

};

#endif // DETECTORCONSTRUCTION_H
