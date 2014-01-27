#include "ProDetectorConstruction.hh"
#include "TSLCollimator.hh"
#include "G4PVParameterised.hh"

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

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

// Sensitivedetector
#include "G4SDManager.hh"
#include "ProNeutronSD.hh"

// visualisation
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"

ProDetectorConstruction::ProDetectorConstruction()
    : CollimatorGeometry(0),
      WorldLog(0),TargetLog(0),ChamberSDLog(0),
      WorldPhy(0),fCheckOverlap(true),
      Air(0),TargetMat(0),DefaultMat(0),CollimatorMat(0)
{
    fWorldLength = 5048*mm;
    collimatorW  = 1200*mm;
    collimatorL  = 1000*mm;
    collimatorH  = 2402*mm;
    apertureDia  = 102 *mm;
    Distance     = 867*mm;
    targetLength = 2.4*cm;
    Detthick     = 1*mm;
    NbOfDetector = 3;
    collimatorYMov = 296*mm;
}

ProDetectorConstruction::~ProDetectorConstruction()
{
    DestroyMaterials();
}



G4VPhysicalVolume* ProDetectorConstruction::Construct()
{
     DefineMaterials();
     return SetupGeometry();
}


void ProDetectorConstruction::DefineMaterials()
{
//---------All material being used in this project ------//
    //  use G4-NIST materials data base
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

    G4double      density;
    G4int     ncomponents;
    G4double fractionmass;
    /// @ CollimatorMat : StainlessSteel
    CollimatorMat=new G4Material("StainlessSteel",density=8.06*g/cm3,ncomponents=6);
    CollimatorMat->AddElement(C,fractionmass=0.001);
    CollimatorMat->AddElement(Si,fractionmass=0.007);
    CollimatorMat->AddElement(Cr,fractionmass=0.18);
    CollimatorMat->AddElement(Mn,fractionmass=0.01);
    CollimatorMat->AddElement(Fe,fractionmass=0.712);
    CollimatorMat->AddElement(Ni,fractionmass=0.09);

  //  Print out material so that we need to find that material G4_AIR
  G4cout<< G4endl
        << "******* World material : "  << DefaultMat << "\n"
        << "******* Target material : " << TargetMat << "\n"
        << "******* Sensitive detector material : " << DefaultMat << "\n"
        << "******* Collimator material :" << CollimatorMat << G4endl;
  G4cout<<*(G4Material::GetMaterialTable())<<G4endl;
}



G4VPhysicalVolume* ProDetectorConstruction::SetupGeometry()
{
    if(!DefaultMat||!TargetMat||!CollimatorMat){
             G4cerr<< "Cannot retrieve materials already defined. " << G4endl;
             G4cerr<< "Exiting application " << G4endl;
             exit(1);
    }

/// *******************************************
/// World, box,full length= 5.048 meter       *
/// ------world volume (shape: box)-----------*
/// *******************************************

      G4Box* WorldSolid = new G4Box("World_solid",     //Name
                                     fWorldLength/2,    //half length
                                     fWorldLength/2,
                                     fWorldLength/2);

      WorldLog = new G4LogicalVolume(WorldSolid,         // Solid
                                     DefaultMat,         // filled material
                                     "World");

      WorldPhy = new G4PVPlacement  (0,                    // Rotation matrix
                                     G4ThreeVector(),      // Position (0,0,0)
                                     WorldLog,             // Logical volume
                                     "World",              // Name
                                     0,                    // Mother volume
                                     false,                // unused boolean
                                     0,                    // copy number
                                     fCheckOverlap);       // checking overlap

/// *******************************************
/// Target cylinder build                     *
/// Target tube, diameter 5*cm,height 24mm    *
/// Target Centre position : (0,0,0)          *
/// *******************************************

      const G4double TargetInR=0.*cm;
      const G4double TargetOutR=2.5*cm;
      const G4double TargetHeight=targetLength;
      G4ThreeVector  TargetPos = G4ThreeVector(0.,0.,-TargetHeight/2);

      G4Tubs* Target_tubs = new G4Tubs ("TargetSolid",          //Name
                                      TargetInR,                //Inner radius
                                      TargetOutR,               //Outer radius
                                      TargetHeight/2,           // Half length in z
                                      0.*deg,                   // Starting phi angle
                                      360.*deg);                // Segment angle

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

/// *******************************************************************************************************
/// Collimator, rectangular solid,full length z= 1 m, x=1.2 m, y=2.402 m;                                 *
/// Standard hole or aperture, diamater =10.2cm, length =1 m, Distance =8.67 m;                           *
/// Collimator's position is moved in +z (World) : 867mm + TargetHeight/2 + CollimatorZ/2; in -y : 296 mm *
/// Hole's position is moved to +y 296mm: apertureYMov = 296*mm;                                          *
/// *******************************************************************************************************
  // collimator volume (shape: box)

      G4double CollimatorZMov = collimatorL/2+Distance;
      G4ThreeVector CollimatorPos (0,-collimatorYMov,CollimatorZMov);
      G4RotationMatrix* collimatorRot =0;

      CollimatorGeometry = new TSLCollimator(apertureDia,
                                             collimatorW,
                                             collimatorH,
                                             collimatorL,
                                             CollimatorMat,
                                             WorldPhy,
                                             CollimatorPos,
                                             collimatorRot);

  // print parameters
  //
      G4cout  <<"\n ******************************************************************"
              <<"\n--->	Target      Material:	"  << TargetMat->GetName()      <<	'\n'
              <<"\n--->	Collimator	Material: 	"  << CollimatorMat->GetName()	<<  '\n'
              <<"\n---> World space Material:   "  << DefaultMat->GetName()     <<  '\n'
              <<"\n ******************************************************************"<< G4endl;



/// *******************************************************
/// Sensitive Detector Geometry                           *
/// shape: box, full length z= 1 mm, x=1.2 m, y=2.402 m;  *
/// *******************************************************

// Sensitive Detector located in the front of collimator and behind of collimator
//
         G4double SDZPos = 0.;
         G4Box* Chambersol = new G4Box("Chambersolid", //Name
                                      collimatorW/2,     //Half length
                                      collimatorH/2,
                                      Detthick/2);
         ChamberSDLog = new G4LogicalVolume(Chambersol,        //its solid
                                               DefaultMat,     //its material
                                              "ChamberSD");    //its name
         for(G4int i=0;i<3;i++)
         {
             if(i<2)
             {
                 SDZPos = Distance - 0.5*Detthick + (collimatorL + Detthick)*i;
             }
             else
             {
                 SDZPos = 2500*mm;
             }
              new G4PVPlacement(0,
                                G4ThreeVector(0,-collimatorYMov,SDZPos),
                                ChamberSDLog,
                                "ChamberSD",
                                WorldLog,
                                false,
                                i,
                                fCheckOverlap);
         }

/// *****************************
///  Visualisation attributes   *
/// *****************************
    WorldLog->SetVisAttributes(G4VisAttributes::Invisible);

    // Target volumes Blue with transparancy
    G4VisAttributes* targetAttributes = new G4VisAttributes(G4Color(0.0,0.0,1.0));
    targetAttributes->SetVisibility(true);
    TargetLog->SetVisAttributes(targetAttributes);

    // Collimator volume Gray with transparancy
    G4VisAttributes* collimatorAttributes = new G4VisAttributes(G4Color(0.5,0.5,0.5));
    collimatorAttributes->SetForceSolid(true);
    if(CollimatorGeometry)
    {
        CollimatorGeometry->GetPhysicalVolume()->GetLogicalVolume()
                ->SetVisAttributes(collimatorAttributes);
    }

    // Detectors volumes white with transparancy
    G4VisAttributes* detectorAttributes = new G4VisAttributes(G4Color(1.0,1.0,1.0));
    detectorAttributes->SetVisibility(true);
    ChamberSDLog->SetVisAttributes(detectorAttributes);

/// ****************************
///  Sensitive Detectors       *
/// ****************************

    // Collimator Sensitive Detectors
    //
    G4VSensitiveDetector* collimatordet;

    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;

    collimatordet = new ProNeutronSD (SDname="CollimatorSDet");
    // Get pointer to detector manager
    SDman->AddNewDetector(collimatordet);
    // Attach detector to volume defining neutronSDLog
    ChamberSDLog->SetSensitiveDetector(collimatordet);

    // return the world physical volume ----------------------------------------

    G4cout << G4endl << "The geometrical tree defined are : " << G4endl << G4endl;
    DumpGeometricalTree(WorldPhy);

    return WorldPhy;
}

void ProDetectorConstruction::DestroyMaterials()
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

void ProDetectorConstruction::DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth)
{
  for(int isp=0;isp<depth;isp++)
  { G4cout << "  "; }
  G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
         << aVolume->GetLogicalVolume()->GetName() << " "
         << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
         << aVolume->GetLogicalVolume()->GetMaterial()->GetName();
  if(aVolume->GetLogicalVolume()->GetSensitiveDetector())
  {
    G4cout << " " << aVolume->GetLogicalVolume()->GetSensitiveDetector()
                            ->GetFullPathName();
  }
  G4cout << G4endl;
  for(int i=0;i<aVolume->GetLogicalVolume()->GetNoDaughters();i++)
  { DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i),depth+1); }
}

