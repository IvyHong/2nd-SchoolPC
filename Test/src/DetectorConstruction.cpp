#include "DetectorConstruction.h"
#include "TSLCollimator.h"
#include "CellParameterisation.h"


#include "globals.hh"
#include "G4RunManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"

#include "G4PVParameterised.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

// Sensitivedetector
#include "G4SDManager.hh"
#include "NeutronSD.h"

// visualisation
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"

DetectorConstruction::DetectorConstruction()
    : CollimatorGeometry(0),
      WorldLog(0),TargetLog(0),neutronSD1Log(0),neutronSD2Log(0),
      WorldPhy(0),fCheckOverlap(true),
      Air(0),TargetMat(0),DefaultMat(0),CollimatorMat(0)
{
    fWorldLength = 5048*mm;
    collimatorW = 1200*mm;
    collimatorL = 1000*mm;
    collimatorH = 2402*mm;
    apertureDia = 102 *mm;
    Distance = 867*mm;
    Detthick = 1*mm;
    NbOfDetector = 3;
    collimatorYMov = 296*mm;
}

DetectorConstruction::~DetectorConstruction()
{}



G4VPhysicalVolume* DetectorConstruction::Construct()
{
     DefineMaterials();
     return SetupGeometry();
}


void DetectorConstruction::DefineMaterials()
{
//---------All material being used in this project ------//
    // use G4-NIST materials data base
  G4NistManager* MAN=G4NistManager::Instance();

    Air=MAN->FindOrBuildMaterial("G4_AIR");
    TargetMat=MAN->FindOrBuildMaterial("G4_W");
    DefaultMat=MAN->FindOrBuildMaterial("G4_Galactic");

    // collimator material Steel/iron, not in NIST
    G4Element* C =MAN->FindOrBuildElement("C");
    G4Element* Si=MAN->FindOrBuildElement("Si");
    G4Element* Cr=MAN->FindOrBuildElement("Cr");
    G4Element* Mn=MAN->FindOrBuildElement("Mn");
    G4Element* Fe=MAN->FindOrBuildElement("Fe");
    G4Element* Ni=MAN->FindOrBuildElement("Ni");

    G4double density;
    G4int ncomponents;
    G4double fractionmass;
    /// @ CollimatorMat : StainlessSteel
    CollimatorMat=new G4Material("StainlessSteel",density=8.06*g/cm3,ncomponents=6);
    CollimatorMat->AddElement(C,fractionmass=0.001);
    CollimatorMat->AddElement(Si,fractionmass=0.007);
    CollimatorMat->AddElement(Cr,fractionmass=0.18);
    CollimatorMat->AddElement(Mn,fractionmass=0.01);
    CollimatorMat->AddElement(Fe,fractionmass=0.712);
    CollimatorMat->AddElement(Ni,fractionmass=0.09);

  // Print out material so that we need to find that material G4_AIR
  G4cout<< G4endl
        << "******* World material : " << DefaultMat << "\n"
        <<"******** Target material : " << TargetMat << "\n"
        << "******* Sensitive detector material : " << DefaultMat << "\n"
        << "******* Collimator material :" << CollimatorMat << G4endl;
  G4cout<<*(G4Material::GetMaterialTable())<<G4endl;
}

G4VPhysicalVolume* DetectorConstruction::SetupGeometry()
{
    if(!DefaultMat||!TargetMat){
             G4cerr<< "Cannot retrieve materials already defined. " << G4endl;
             G4cerr<< "Exiting application " << G4endl;
             exit(1);
    }

    //---------Geometry being used in this project ----------//
    // World, box,full length= 5.048 meter
    //------------------------------world volume (shape: box)
      G4Box* WorldSolid = new G4Box("World_solid", //Name
                                     fWorldLength/2, //half length
                                     fWorldLength/2,
                                     fWorldLength/2);

      WorldLog = new G4LogicalVolume(WorldSolid, // Solid
                                     DefaultMat, // filled material
                                     "World");

      WorldPhy = new G4PVPlacement (0, // Rotation matrix
                                     G4ThreeVector(), // Position (0,0,0)
                                     WorldLog, // Logical volume
                                     "World", // Name
                                     0, // Mother volume
                                     false, // unused boolean
                                     0, // copy number
                                     fCheckOverlap); // checking overlap


  //-------------Target cylindar--------------------------------------//
      // Target tube, diameter 5*cm,height 24mm
      // Target Centre position is moved in -z: (0,0,-144mm-12mm)
      const G4double TargetInR=0.*cm;
      const G4double TargetOutR=2.5*cm;
      const G4double TargetHeight=2.4*cm;
      G4ThreeVector TargetPos = G4ThreeVector(0.,0.,-TargetHeight/2);

      G4Tubs* Target_tubs = new G4Tubs ("TargetSolid", //Name
                                      TargetInR, //Inner radius
                                      TargetOutR, //Outer radius
                                      TargetHeight/2, // Half length in z
                                      0.*deg, // Starting phi angle
                                      360.*deg); // Segment angle

      TargetLog = new G4LogicalVolume (Target_tubs,
                                       DefaultMat,
                                       "Target");
      new G4PVPlacement (0,
                        TargetPos,
                        TargetLog,
                        "Target",
                        WorldLog,
                        false,
                        0,
                        fCheckOverlap);

//---- Collimator wtih a hole inside--------------------------//
// Collimator, rectangular solid,full length z= 1 m, x=1.2 m, y=2.402 m;
// Standard hole or aperture, diamater =10.2cm, length =1 m, Distance =867mm;
// Collimator's position is moved in +z (World) : 867mm + TargetHeight/2 + CollimatorZ/2; in -y : 296mm
// The hole's position is moved to +y : 296mm
// G4double apertureYMov = 296*mm;

      G4double CollimatorZMov = collimatorL/2+Distance;
      G4ThreeVector CollimatorPos (0,-collimatorYMov,CollimatorZMov);
      G4RotationMatrix* collimatorRot =0;
//    collimator volume (shape: box)
      CollimatorGeometry = new TSLCollimator(apertureDia,
                                             collimatorW,
                                             collimatorL,
                                             collimatorH,
                                             CollimatorMat,
                                             WorldPhy,
                                             CollimatorPos,
                                             collimatorRot);
// ***********************************************************************************
// print parameters
//

      G4cout << "\n---> Target Material : "     << TargetMat->GetName()     <<'\n'
             << "\n---> Collimator Material : " << CollimatorMat->GetName() <<'\n'
             << "\n---> World Space : "         << DefaultMat->GetName()    <<G4endl;

// ************************************************************************************

//******************************
// Sensitive Detector Geometry *
//******************************
// Sensitive detector (shape: box), rectangular solid,
// width x= 204 mm, height y= 204 mm, length z= 1mm;
// 2 Times of aperture´s diameter : DetectorLength = 2 * apertureDia;
//
// Sensitive detector (shape: box)
//

 G4Box* detectorsolid = new G4Box("SensitiveDetectorSolid",
                                   apertureDia,
                                   apertureDia,
                                   Detthick/2);
 neutronSD1Log = new G4LogicalVolume(detectorsolid,
                                     DefaultMat,
                                     "NeutronSD1",0,0,0);

 new G4PVPlacement(0,
     G4ThreeVector(0,-collimatorYMov,Distance-Detthick/2),
                   neutronSD1Log,
                   "NeutronSD1",
                    WorldLog,
                    false,
                    0,
                    fCheckOverlap);
 // NeutronDetector1 cells
 //
 G4VSolid* neutroncellSolid = new G4Box("neutroncellBox",apertureDia/10,apertureDia/10,Detthick/2);
 G4LogicalVolume* neutroncellLog
   = new G4LogicalVolume(neutroncellSolid,DefaultMat,"neutroncellLog",0,0,0);
 G4VPVParameterisation* cellParam = new CellParameterisation();
 new G4PVParameterised("neutroncellPhy",neutroncellLog,neutronSD1Log,
                        kXAxis,100,cellParam);




    // Visualisation attributes, Invisible world volume
    //
    WorldLog->SetVisAttributes(G4VisAttributes::Invisible);

    // Target volume with Blue (0.0,0.0,1.0)
    //
    G4VisAttributes* targetAttributes = new G4VisAttributes(G4Color(0.0,0.0,1.0));
    targetAttributes->SetVisibility(true);
    TargetLog->SetVisAttributes(targetAttributes);

    // Collimator volume with  Gray
    //
    G4VisAttributes* collimatorAttributes = new G4VisAttributes(G4Color(0.5,0.5,0.5));
    collimatorAttributes->SetForceSolid(true);
    if(CollimatorGeometry)
    {
        CollimatorGeometry->GetPhysicalVolume()->GetLogicalVolume()->SetVisAttributes(collimatorAttributes);
    }

    // detectors volumes with White (1.0,1.0,1.0)
    //
    G4VisAttributes* detectorAttributes = new G4VisAttributes(G4Color(1.0,1.0,1.0));
    detectorAttributes->SetVisibility(true);
//    collimatorSDLog->SetVisAttributes(detectorAttributes);
    neutronSD1Log->SetVisAttributes(detectorAttributes);

// ***********************************
// .......Sensitive detectors .......
// ***********************************
    G4VSensitiveDetector* neutrondet;

    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;

    neutrondet = new NeutronSD (SDname="NeutronSensitiveDetector");
    // Get pointer to detector manager
    SDman->AddNewDetector(neutrondet);
    // Attach detector to volume defining neutronSDLog
    neutronSD1Log->SetSensitiveDetector(neutrondet);

    return WorldPhy;
}

