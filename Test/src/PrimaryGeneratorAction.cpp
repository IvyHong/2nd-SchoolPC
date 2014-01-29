#include "PrimaryGeneratorAction.h"
#include "DetectorConstruction.h"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"

/////////////////////////////////////////////////////////////////////////////////

G4ParticleGun* PrimaryGeneratorAction::fParticleGun(0);

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detector)
    :G4VUserPrimaryGeneratorAction(),
     fDetector(detector)
{

//  //*************************************************************************
//  G4double apertureDiameter =0;
//  apertureDiameter=fDetector->GetApertureDiameter();
//  // ParticleGun Position
//  // Method 1
//  G4double ParticleGunZPos1 = 0, ParticleGunZPos2 = 0;
//  G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
//  G4Box* worldBox = NULL;
//  if(worldLV) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
//  if(worldBox) ParticleGunZPos1 = worldBox->GetZHalfLength();
//  else {
//      G4cerr << "World volume of box not found. " << '\n'
//             << "Perhaps you changed geometry." << '\n'
//             << "The gun will be place in the center. " << G4endl;
//  }
//  // Method 2
//  ParticleGunZPos2 = 0.5*(fDetector->GetWorldFullLength());
//  if(ParticleGunZPos1==ParticleGunZPos2)
//  {
//      fParticleGunZPos=ParticleGunZPos2;
//  }
//  else
//  { G4cerr << "Method1 Position " << ParticleGunZPos1 << " mm" << '\t'
//           << "Method2 Position " << ParticleGunZPos2 << " mm" << G4endl;}
  //*************************************************************************

  fParticleGunZPos=-10.*mm;

  ///@@
  //
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fProton = particleTable->FindParticle(particleName="proton");
  fNeutron= particleTable->FindParticle(particleName="neutron");

  fParticleGun->SetParticleDefinition(fNeutron);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(180.*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,fParticleGunZPos));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}


