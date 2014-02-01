#include "detectorconstruction.h"
#include "cellparameterisation.h"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4RunManager.hh"

// Defining sensitive detector
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction()
    :fWorldLog(0),fTargetLog(0),
     fCalorimeterLog(0),fCollimatorLog(0),
     fWorldPhys(0),fTargetPhys(0),
     fCalorimeterPhys(0),fCollimatorPhys(0)
{

}


DetectorConstruction::~DetectorConstruction(){}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Material Definition
    DefineMaterials();

    // Geometry Definition
    SetupGeometry();

    // Return world volume
    return fWorldPhys;
}

void DetectorConstruction::DefineMaterials()
{
    G4double a, z, density;
    G4int ncomponents;
    G4double fractionmass;

    // Define materials
    //
    new G4Material("Silicon", z=14., a=28.0855*g/mole, density=2.330*g/cm3);
    new G4Material("Iron", z=26., a=55.845*g/mole, density=7.87*g/cm3);
    new G4Material("Tungsten", z=74., a=183.85*g/mole, density= 19.30*g/cm3);

    // Define air
    //G4Material* air = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
    G4Material* air = new G4Material("Air", density= 1.290e-9*mg/cm3, ncomponents=2);
    air->AddElement(N, fractionmass=0.7);
    air->AddElement(O, fractionmass=0.3);

    // Define vacuum
    G4Material* vacuum = new G4Material("Vacuum", density= 1.e-5*g/cm3,
                        ncomponents=1, kStateGas, STP_Temperature,
                        2.e-2*bar);

    vacuum->AddMaterial(air, fractionmass=1.);

    // Dump material information
    G4cout << G4endl << "The materials defined are :" << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void DetectorConstruction::SetupGeometry()
{
    G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    G4Material* TargetMat = G4Material::GetMaterial("Tungsten");
    G4Material* CollimatorMat = G4Material::GetMaterial("Iron");
    G4Material* DetectorMat = G4Material::GetMaterial("Silicon");

    // World volume
    G4Box* worldSolid = new G4Box("World_Solid",           // Name
                                  5.0*m, 5.0*m, 5.0*m);    // Half lengths

    fWorldLog = new G4LogicalVolume(worldSolid,         // Solid
                                    air,                // Material
                                    "World_Logical");   // Name

    fWorldPhys = new G4PVPlacement(0,                 // Rotation matrix pointer
                                   G4ThreeVector(),   // Translation vector
                                   fWorldLog,         // Logical volume
                                   "World_Physical",  // Name
                                   0,                 // Mother volume
                                   false,             // Unused boolean parameter
                                   0);                // Copy number

    ////////////////////////////////////////////////////////////////////
    const G4double TargetInR    = 0.*cm;
    const G4double TargetOutR   =2.5*cm;
    const G4double TargetHeight =2.4*cm;

    G4Tubs* targetTubs = new G4Tubs("Target_Solid", // Name
                                    TargetInR,      // Inner radius
                                    TargetOutR,     // Outer radius
                                    TargetHeight/2, // Half length in z
                                    0.*deg,         // Starting phi angle
                                    360.*deg);      // Segment angle

    fTargetLog =
            new G4LogicalVolume(targetTubs,        // Solid
                                TargetMat,         // Material
                                "Target_Logical"); // Name

    fTargetPhys = new G4PVPlacement(0,                     // Rotation matrix pointer
                      G4ThreeVector(0.,0.,TargetHeight/2), // Translation vector
                      fTargetLog,                          // Logical volume
                      "Target_Physical",                   // Name
                      fWorldLog,                           // Mother volume
                      false,                               // Unused boolean
                      0);                                  // Copy number

    ////////////////////////////////////////////////////////////////////
    // Iron Collimator

    const G4double collimatorW=120*cm;
    const G4double collimatorH=240.2*cm;
    const G4double collimatorL=100*cm;

    const G4double collimatorYMov = 29.6*cm;
    const G4double collimatorZMov = collimatorL/2+86.7*cm+TargetHeight;
    const G4ThreeVector collimatorPos (0,-collimatorYMov,collimatorZMov);
    G4RotationMatrix* collimatorRot =0;

    // collimator volume (shape: box)
    G4Box* collimatorSolid = new G4Box("Collimator_Solid",
                                       collimatorW/2,
                                       collimatorH/2,
                                       collimatorL/2);
    fCollimatorLog
            = new G4LogicalVolume(collimatorSolid,        // Solid
                                  collimatorMat,          // Material
                                  "Collimator_Logical");  // Name
    fCollimatorPhys
            = new G4PVPlacement(collimatorRot,            // Rotation matrix pointer
                                collimatorPos,            // Translation vector
                                fCollimatorLog,           // Logical volume
                                "Collimator_Physical",    // Name
                                fWorldLog,                // Mother volume
                                false,                    // Unused boolean
                                0);
    /// update collimator
    const G4double ApertureInR  =  0.*cm;
    const G4double ApertureOutR =  5.1*cm;
    const G4double ApertureLength =100.0*cm;

    G4Tubs* ApertureTubs = new G4Tubs("Aperture_Solid",// Name
                                       ApertureInR,       // Inner radius
                                       ApertureOutR,      // Outer radius
                                       ApertureLength/2,  // Half length in z
                                       0.*deg,            // Starting phi angle
                                       360.*deg);         // Segment angle

    G4LogicalVolume* fApertureLog =
            new G4LogicalVolume(ApertureTubs,        // Solid
                                air,                 // Material
                                "Aperture_Logical"); // Name

    new G4PVPlacement(0,                     // Rotation matrix pointer
                      G4ThreeVector(0.,collimatorYMov,0.), // Translation vector
                      fApertureLog,                        // Logical volume
                      "Aperture_Physical",                 // Name
                      fCollimatorLog,                      // Mother volume
                      false,                               // Unused boolean
                      0);                                  // Copy number



    ////////////////////////////////////////////////////////////////////
    // EmCalorimeter
    // Mother volume
    const G4ThreeVector calorimeterPos (0.,0.,collimatorL+86.7*cm+TargetHeight+50*cm);

    G4VSolid* calorimeterSolid = new G4Box("Calorimeter_Solid", // Name
                                           .5*m,                // x half length
                                           .5*m,                // y half length
                                           .25*m) ;             // z half length

    fCalorimeterLog =
      new G4LogicalVolume(calorimeterSolid,       // Solid
                          air,                    // Material
                          "Calorimeter_Logical"); // Name

    new G4PVPlacement(0,                          // Rotation matrix pointer
                      G4ThreeVector(0.,0.,0.5*m), // Translation vector
                      fCalorimeterLog,            // Logical volume
                      "Calorimeter_Physical",     // Name
                      fWorldLog,                  // Mother volume
                      false,                      // Unused boolean
                      0);                         // Copy number

    // 100 rectangular Si cells

    G4VSolid* cellSolid = new G4Box("Cell_Solid", // NameS
                                    5*cm,         // x half length
                                    5*cm,         // y half length
                                    25.*cm);      // z half length

    G4LogicalVolume* cellLogical
      = new G4LogicalVolume(cellSolid,       // Solid
                            DetectorMat,     // Material
                            "Cell_Logical"); // Name

    G4VPVParameterisation* cellParam = new CellParameterisation();

    new G4PVParameterised("Cell_Physical",    // Name
                          cellLogical,        // Logical volume
                          fCalorimeterLog,    // Mother volume
                          kXAxis,             // Axis
                          100,                // Number of replicas
                          cellParam);         // Parameterisation


    ////////////////////////////////////////////////////////////////////
    // Defining sensitive detector
    // Create a new Calorimeter sensitive detector
    G4VSensitiveDetector* detector = new Calorimeter("Calorimeter");

    // Get pointer to detector manager
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

   // Register detector with manager
    SDman->AddNewDetector(detector);

    // Attach detector to volume defining calorimeter cells
    cellLogical->SetSensitiveDetector(detector);


    ////////////////////////////////////////////////////////////////////
    // Visualisation attributes
    // Invisible world volume.
    fWorldLog->SetVisAttributes(G4VisAttributes::Invisible);

    // Calorimeter attributes
    // Invisible calorimeter mother volume
    fCalorimeterLog->SetVisAttributes(G4VisAttributes::Invisible);

    // Calorimeter cells - green with transparency
    G4VisAttributes* calorimeterAttributes =
       new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.1));
    calorimeterAttributes->SetForceSolid(true);
    cellLogical->SetVisAttributes(calorimeterAttributes);

    // Target Volume - light blue
    G4VisAttributes* targetAttributes = new G4VisAttributes(G4Colour(0.0,0.5,0.5,1.0));
    targetAttributes->SetForceSolid(true);
    fTargetLog->SetVisAttributes(targetAttributes);

    // Collimator Volume - Yellow
    G4VisAttributes* collimatorAttributes = new G4VisAttributes(G4Colour::Yellow());
    collimatorAttributes->SetForceSolid(true);
    fCollimatorLog->SetVisAttributes(collimatorAttributes);
}
