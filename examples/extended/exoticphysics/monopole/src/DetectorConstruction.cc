//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file exoticphysics/monopole/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 66994 2013-01-29 14:34:08Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4UserLimits.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4NistManager.hh"

#include "G4MonopoleFieldSetup.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh" 
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  // default parameter values
  fAbsorSizeX = fAbsorSizeYZ = 10 * cm;
  fWorldSizeX = fWorldSizeYZ = 1.2 * fAbsorSizeX;
  fMaxStepSize = 5 * mm;

  fAbsorMaterial = 0;
  fMagField = 0;
  fLogAbsor   = 0;
  fMonFieldSetup = G4MonopoleFieldSetup::GetMonopoleFieldSetup();

  SetMaterial("G4_Al");
  fWorldMaterial = 
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  // create commands for interactive definition of the detector
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{ 
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  /****************************    World   *****************************/
  G4Box * sWorld = new G4Box("world",                                
               fWorldSizeX / 2, fWorldSizeYZ / 2, fWorldSizeYZ / 2);

  G4LogicalVolume * lWorld = new G4LogicalVolume(sWorld,
                                                 fWorldMaterial,
                                                 "world");        

  G4VPhysicalVolume * pWorld = new G4PVPlacement(0,        //no rotation
                             G4ThreeVector(),                //at (0,0,0)
                           lWorld,                        //logical volume
                           "world",                        //name
                           0,                                       //mother  volume
                           false,                        //no boolean operation
                           0);                                //copy number


  /**************************    Absorber    ***************************/
  G4Box * sAbsor = new G4Box("Absorber",                   
          fAbsorSizeX / 2, fAbsorSizeYZ / 2, fAbsorSizeYZ / 2);        

  fLogAbsor = new G4LogicalVolume(sAbsor,        
                                  fAbsorMaterial,
                                  "Absorber");
  
  new G4PVPlacement(0,                                //no rotation
                    G4ThreeVector(),                //at (0,0,0)
                    fLogAbsor,                        //logical volume
                    "Absorber",                        //name
                    lWorld,                               //mother  volume
                    false,                        //no boolean operation
                    0);                                //copy number
  fLogAbsor->SetUserLimits(new G4UserLimits(fMaxStepSize));

  PrintParameters();

  /************     always return the World volume     *****************/
  return pWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n---------------------------------------------------------\n";
  G4cout << "---> The Absorber is " << G4BestUnit(fAbsorSizeX, "Length")
         << " of " << fAbsorMaterial->GetName() << G4endl;
  G4cout << "\n---------------------------------------------------------\n";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSizeX(G4double value)
{
  if(value > 0.0) {
    fAbsorSizeX = value; 
    fWorldSizeX = 1.2 * fAbsorSizeX;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSizeYZ(G4double value)
{
  if(value > 0.0) {
    fAbsorSizeYZ = value; 
    fWorldSizeYZ = 1.2 * fAbsorSizeYZ;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaterial(const G4String& namemat)
{
  // search the material by its name   
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(namemat);
  if(!mat) {
    G4cout << "!!! DetectorConstruction::SetMaterial: WARNING Material <"
           << namemat << "> does not exist in DB" << G4endl;
    return;
  }
  // new material is found out
  if (mat != fAbsorMaterial) {
    fAbsorMaterial = mat;
    if(fLogAbsor) { fLogAbsor->SetMaterial(mat); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMagField(G4double fieldValue)
{
  fMonFieldSetup->SetMagField(fieldValue);

  //apply a global uniform magnetic field along Z axis
  G4FieldManager * fieldMgr = 
    G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
  if (fMagField) { delete fMagField; }        //delete the existing magn field

  if (fieldValue != 0.)                        // create a new one if non nul
    {
      fMagField = new G4UniformMagField(G4ThreeVector(0., 0., fieldValue));
      fieldMgr->SetDetectorField(fMagField);
      fieldMgr->CreateChordFinder(fMagField);
    }
   else
    {
      fMagField = 0;
      fieldMgr->SetDetectorField(fMagField);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaxStepSize(G4double step)
{
  fMaxStepSize = step;
  if(fLogAbsor) { fLogAbsor->SetUserLimits(new G4UserLimits(fMaxStepSize)); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
