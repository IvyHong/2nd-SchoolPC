#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;

class TSLCollimator;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    // Default Constructor
    //
    DetectorConstruction();

    // Default Destructor
    //
    virtual ~DetectorConstruction();

    // Method
    //
    virtual G4VPhysicalVolume* Construct();

  private:

    // Helper Methods
    //
    void ConstructMaterials();
    void DestroyMaterials();


//  Li has a relatively high neutron absorption cross section via
//  reaction : Li + n -> He + H
    G4Material* fAir;
    G4Material* fScintillator;
    G4Material* fTungsten;
    G4Material* fSteel;
    G4Material* fSilicon;

    G4VisAttributes* fWorldVisAtt;
    G4VisAttributes* fEMcalorimeterVisAtt;
    G4VisAttributes* fCellVisAtt;
    G4VisAttributes* fTargetVisAtt;
    G4VisAttributes* fMonitorVisAtt;

    TSLCollimator*     fCollimatorGeometry;
    G4LogicalVolume*   fWorldLog;
    G4VPhysicalVolume* fWorldPhy;

};


#endif // DETECTORCONSTRUCTION_H
