#include "globals.hh"
#include "TSLCollimator.h"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

// visualisation
#include "G4VisAttributes.hh"
#include "G4Colour.hh"


TSLCollimator::TSLCollimator()
{
   fApertureDiameter=102*mm;
   fCollimatorWidth =1200*mm;
   fCollimatorLength=1000*mm;
   fCollimatorHeight=2402*mm;
}

// Constructor with parameters
TSLCollimator::TSLCollimator(G4double ApertureDiameter,
                             G4double CollimatorWidth,
                             G4double CollimatorHeight,
                             G4double CollimatorLength,
                             G4Material * CollimatorMat,
                             G4VPhysicalVolume * MotherPV,
                             G4ThreeVector CollimatorPos,
                             G4RotationMatrix* Rotation)
{
    fApertureDiameter = ApertureDiameter;
    fCollimatorWidth = CollimatorWidth;
    fCollimatorHeight = CollimatorHeight;
    fCollimatorLength = CollimatorLength;
    fCollimatorMat = CollimatorMat;
    fCollimatorPos = CollimatorPos;
    fCollimatorRot = Rotation;

    if(fApertureDiameter<0||fApertureDiameter>fCollimatorWidth||fApertureDiameter>fCollimatorHeight)
    {
        G4Exception("Check Aperture, Collimator Width and Collimator Heighth","",FatalException,"");
    }

    TSLCollimatorSetup(MotherPV);
}

TSLCollimator::~TSLCollimator()
{;}

void TSLCollimator::TSLCollimatorSetup(G4VPhysicalVolume *MotherPV)
{
//-------------------------------- Collimator Box with aperture -------------------------//
    //--------------------------collimator volume (shape: box) ------------------------//
      G4Tubs* CylinderTube = new G4Tubs
                           ("CylinderTube",
                           0.*mm,fApertureDiameter/2,
                           fCollimatorLength/2+0.1*mm,
                           0,2*M_PI);
      G4Box* CollimatorBox = new G4Box
                           ("CollimatorBox",
                           fCollimatorWidth/2,
                           fCollimatorHeight/2,
                           fCollimatorLength/2);

      G4SubtractionSolid* CollimatorSolid= new G4SubtractionSolid("CollimatorBox-CylinderTube",
                                                                  CollimatorBox,
                                                                  CylinderTube,
                                                                  0,
                                                                  fApertureMov); //Moving aperture towards +Y direction
      fCollimatorLog = new G4LogicalVolume (CollimatorSolid,fCollimatorMat,"Collimator",0,0,0);
      fCollimatorPhy = new G4PVPlacement
                        (fCollimatorRot,
                         fCollimatorPos,
                         "Collimator",
                         fCollimatorLog,
                         MotherPV,
                         false,
                         0);

      fCollimatorLog->SetVisAttributes(G4VisAttributes::Invisible);

}
