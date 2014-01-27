#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H


#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class TSLCollimator;
//class DetectorConstMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    void ConstructMaterials();
    void DestroyMaterials();
    void DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth=0);

  private:
//    DetectorConstMessenger* fMessenger;
//  Li has a relatively high neutron absorption cross section via
//  reaction : Li + n -> He + H
    G4Material* fAir;
    G4Material* fArgonGas;
    G4Material* fScintillator;
    G4Material* fCsI;
    G4Material* fLead;
    G4Material* fTungsten;
    G4Material* fSteel;

    G4VisAttributes* fWorldVisAtt;
    G4VisAttributes* fArmVisAtt;
    G4VisAttributes* fHodoscopeVisAtt;
    G4VisAttributes* fChamberVisAtt;
    G4VisAttributes* fWirePlaneVisAtt;
    G4VisAttributes* fEMcalorimeterVisAtt;
    G4VisAttributes* fCellVisAtt;
    G4VisAttributes* fHadCalorimeterVisAtt;
    G4VisAttributes* fHadCalorimeterCellVisAtt;

    TSLCollimator*     fCollimatorGeometry;
    G4VPhysicalVolume* fSecondArmPhys;
};


#endif // DETECTORCONSTRUCTION_H
