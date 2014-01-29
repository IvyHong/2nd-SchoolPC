#include "DetectorConstruction.h"
#include "CellParameterisation.h"
#include "TSLCollimator.h"

#include "EmCalorimeter.h"
#include "Monitor.h"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4PVReplica.hh"


DetectorConstruction::DetectorConstruction()
    : fAir(0),fScintillator(0),fTungsten(0),fSteel(0),fSilicon(0),
      fWorldVisAtt(0),fEMcalorimeterVisAtt(0), fCellVisAtt(0),
      fTargetVisAtt(0),fMonitorVisAtt(0),
      fCollimatorGeometry(0),fWorldLog(0),fWorldPhy(0)

{}

DetectorConstruction::~DetectorConstruction()
{
  DestroyMaterials();

  delete fWorldVisAtt;
  delete fEMcalorimeterVisAtt;
  delete fCellVisAtt;
  delete fTargetVisAtt;

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  ConstructMaterials();

  // geometries --------------------------------------------------------------
  // experimental hall (world volume)
  G4VSolid* worldSolid = new G4Box("worldBox",10.*m,3.*m,10.*m);
  G4LogicalVolume* fWorldLog
    = new G4LogicalVolume(worldSolid,fAir,"fWorldLog",0,0,0);
  G4VPhysicalVolume* fWorldPhy
    = new G4PVPlacement(0,G4ThreeVector(),fWorldLog,"fWorldPhy",0,0,0);

  // target Tube --------------------------------------------------------------
  //
  const G4double TargetInR=0.*cm;
  const G4double TargetOutR=2.5*cm;
  const G4double TargetHeight=2.4*cm;

  G4VSolid* targetSolid = new G4Tubs("targetTubs",
                                     TargetInR,
                                     TargetOutR,
                                     TargetHeight/2,
                                     0.,360.*deg);
  G4LogicalVolume* targetLogical
    = new G4LogicalVolume(targetSolid,fAir,"targetLogical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,TargetHeight/2),
                    targetLogical,"targetPhysical",fWorldLog,0,0);

  //---- Collimator wtih a hole inside--------------------------//
  // Collimator, rectangular solid,full length z= 1 m, x=1.2 m, y=2.402 m;
  // Standard hole or aperture, diamater =10.2cm, length =1 m, Distance =867mm;
  // Distance between Collimator and target +z (World) : 867mm
  // The hole's position is moved to +y : 296mm
  // G4double apertureYMov = 296*mm;
  const G4double apertureDia=10.2*cm;
  const G4double collimatorW=120*cm;
  const G4double collimatorH=240.2*cm;
  const G4double collimatorL=100*cm;

  const G4double collimatorYMov = 29.6*cm;
  const G4double collimatorZMov = 86.7*cm+TargetHeight+collimatorL/2;
  G4ThreeVector collimatorPos (0,-collimatorYMov,collimatorZMov);
  G4RotationMatrix* collimatorRot =0;
  //-------------collimator volume (shape: box)
  fCollimatorGeometry = new TSLCollimator(apertureDia,
                                          collimatorW,
                                          collimatorH,
                                          collimatorL,
                                          fSteel,
                                          fWorldPhy,
                                          collimatorPos,
                                          collimatorRot);


//  // set "user limits" for drawing smooth curve
//  G4UserLimits* userLimits = new G4UserLimits(5.0*cm);
//  targetLogical->SetUserLimits(userLimits);

  ////////////////////////////////////////////////////////////////////////
  // Create silicon calorimeter
  // Mother volume
  //
  G4VSolid* calorimeterSolid = new G4Box("Calorimeter_Solid", // Name
                                         .5*m,                // x half length
                                         .5*m,                // y half length
                                         .25*m) ;             // z half length

  G4LogicalVolume* calorimeterLogical =
    new G4LogicalVolume(calorimeterSolid,       // Solid
                        fSilicon,               // Material
                        "Calorimeter_Logical"); // Name

  const G4double CalorimeterZMov = collimatorZMov+collimatorL/2+ 25*cm;

  new G4PVPlacement(0,                          // Rotation matrix pointer
                    G4ThreeVector(0.,0.,CalorimeterZMov), // Translation vector
                    calorimeterLogical,         // Logical volume
                    "Calorimeter_Physical",     // Name
                    fWorldLog,                  // Mother volume
                    false,                      // Unused boolean
                    0);                         // Copy number

  // 100 rectangular CsI cells

  G4VSolid* cellSolid = new G4Box("Cell_Solid", // Name
                                  5*cm,         // x half length
                                  5*cm,         // y half length
                                  25.*cm);      // z half length

  G4LogicalVolume* cellLogical
    = new G4LogicalVolume(cellSolid,       // Solid
                          fSilicon,        // Material
                          "Cell_Logical"); // Name

  G4VPVParameterisation* cellParam = new CellParameterisation();

  new G4PVParameterised("Cell_Physical",    // Name
                        cellLogical,        // Logical volume
                        calorimeterLogical, // Mother volume
                        kXAxis,             // Axis
                        100,                // Number of replicas
                        cellParam);         // Parameterisation


  ////////////////////////////////////////////////////////////////////////
  // Silicon Monitor

  G4VSolid* MonitorSolid = new G4Tubs("Monitor_Solid", // Name
                                             0.*cm,                  // Inner radius
                                             5.1*cm,                 // Outer radius
                                             0.005*cm,               // Half length in z
                                             0.*deg,                 // Starting phi angle
                                             360.*deg);              // Segment angle

  G4LogicalVolume* MonitorLogical =
    new G4LogicalVolume(MonitorSolid,       // Solid
                        fAir,                      // Material
                        "Monitor_Logical"); // Name


  for(G4int k=0;k<2;k++)
  {
      G4double MonitorZMov = 0.*cm;
      if(k==0) MonitorZMov = -0.005*cm;
      if(k==1) MonitorZMov = 250.*cm;
      new G4PVPlacement(0,                                   // Rotation matrix pointer
                    G4ThreeVector(0.,0.,MonitorZMov),        // Translation vector
                    MonitorLogical,                   // Logical volume
                    "Monitor_Physical",               // Name
                    fWorldLog,                               // Mother volume
                    false,                                   // Unused boolean
                    k);                                      // Copy number
  }


  ////////////////////////////////////////////////////////////////////////
  // Defining sensitive detector
  // Create a new Monitor & EMcalorimeter sensitive detector

  // Get pointer to detector manager
  //
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // Create a new EMcalorimeter sensitive detector
  G4VSensitiveDetector* EMcalorimeter;

  EMcalorimeter = new EmCalorimeter("EMcalorimeter");
  // Register detector with manager
  //
  SDman->AddNewDetector(EMcalorimeter);
  // Attach detector to volume defining calorimeter cells
  //
  cellLogical->SetSensitiveDetector(EMcalorimeter);

  // Create a new Monitor sensitive detector
  G4VSensitiveDetector* monitor = new Monitor("SiMonitor");

  SDman->AddNewDetector(monitor);

  // Register detector with manager
  G4SDManager::GetSDMpointer()->AddNewDetector(monitor);

  // Attach detector to volume defining calorimeter cells
  MonitorLogical->SetSensitiveDetector(monitor);


  // visualization attributes ------------------------------------------------

  fWorldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  fWorldVisAtt->SetVisibility(false);
  fWorldLog->SetVisAttributes(fWorldVisAtt);

  fEMcalorimeterVisAtt = new G4VisAttributes(G4Colour(0.8888,0.8888,0.0));
  fEMcalorimeterVisAtt->SetVisibility(false);
  calorimeterLogical->SetVisAttributes(fEMcalorimeterVisAtt);

  // Collimator volume Gray with transparancy
  G4VisAttributes* collimatorAttributes = new G4VisAttributes(G4Color(0.5,0.5,0.5));
  collimatorAttributes->SetForceSolid(true);
  if(fCollimatorGeometry)
  {
      fCollimatorGeometry->GetPhysicalVolume()->GetLogicalVolume()->SetVisAttributes(collimatorAttributes);
  }

  fCellVisAtt = new G4VisAttributes(G4Colour(0.9,0.9,0.0));
  cellLogical->SetVisAttributes(fCellVisAtt);

  fTargetVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
  fTargetVisAtt->SetForceSolid(true);
  targetLogical->SetVisAttributes(fTargetVisAtt);

  // Silicon Monitor  - cyan
  fMonitorVisAtt = new G4VisAttributes(G4Colour::Cyan());
  fMonitorVisAtt->SetForceSolid(true);
  MonitorLogical->SetVisAttributes(fMonitorVisAtt);


  // return the world physical volume ----------------------------------------

//  G4cout << G4endl << "The geometrical tree defined are : " << G4endl << G4endl;
//  DumpGeometricalTree(fWorldPhy);

  return fWorldPhy;
}

void DetectorConstruction::ConstructMaterials()
{
  G4double a;
  G4double z;
  G4double density;
  G4double weightRatio;
  G4String name;
  G4String symbol;
  G4int nElem;

  // Define simple materials
  //
//  fSilicon = new G4Material("Silicon",z=14.,a=28.0855*g/mole,  density=2.330*g/cm3);

  // Argon gas
  a = 39.95*g/mole;
  density = 1.782e-03*g/cm3;

  // elements for mixtures and compounds
  a = 1.01*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a = 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
  a = 14.01*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N", z=7., a);
  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxigen", symbol="O", z=8., a);


  // Air
  density = 1.29*mg/cm3;
  fAir = new G4Material(name="Air", density, nElem=2);
  fAir->AddElement(elN, weightRatio=.7);
  fAir->AddElement(elO, weightRatio=.3);

  // Scintillator
  density = 1.032*g/cm3;
  fScintillator = new G4Material(name="Scintillator", density, nElem=2);
  fScintillator->AddElement(elC, 9);
  fScintillator->AddElement(elH, 10);


  //  use G4-NIST materials data base
  //
  G4NistManager* MAN = G4NistManager::Instance();

  // Tungsten
  fTungsten = MAN->FindOrBuildMaterial("G4_W");

  // Silicon
  fSilicon = MAN->FindOrBuildMaterial("G4_Si");

  // Steel

  // collimator material Steel/iron, not in NIST
  G4Element* C =MAN->FindOrBuildElement("C");
  G4Element* Si=MAN->FindOrBuildElement("Si");
  G4Element* Cr=MAN->FindOrBuildElement("Cr");
  G4Element* Mn=MAN->FindOrBuildElement("Mn");
  G4Element* Fe=MAN->FindOrBuildElement("Fe");
  G4Element* Ni=MAN->FindOrBuildElement("Ni");

  G4double Density;
  G4int ncomponents;
  G4double fractionmass;
  /// @ CollimatorMat : StainlessSteel
  fSteel=new G4Material("StainlessSteel",Density=8.06*g/cm3,ncomponents=6);
  fSteel->AddElement(C, fractionmass=0.001);
  fSteel->AddElement(Si,fractionmass=0.007);
  fSteel->AddElement(Cr,fractionmass=0.18);
  fSteel->AddElement(Mn,fractionmass=0.01);
  fSteel->AddElement(Fe,fractionmass=0.712);
  fSteel->AddElement(Ni,fractionmass=0.09);


  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void DetectorConstruction::DestroyMaterials()
{
  // Destroy all allocated elements and materials
  size_t i;
  G4MaterialTable* matTable = (G4MaterialTable*)G4Material::GetMaterialTable();
  for(i=0;i<matTable->size();i++)
  { delete (*(matTable))[i]; }
  matTable->clear();
  G4ElementTable* elemTable = (G4ElementTable*)G4Element::GetElementTable();
  for(i=0;i<elemTable->size();i++)
  { delete (*(elemTable))[i]; }
  elemTable->clear();
}



