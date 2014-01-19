#include "PrimaryGeneratorAction.h"
#include "DetectorConstruction.h"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"

/////////////////////////////////////////////////////////////////////////////////

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detector)
    :G4VUserPrimaryGeneratorAction(),
      fParticleGun(0),fDetector(detector)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="neutron");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(100.*MeV);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Method 1
    G4double ParticleGunZPos1 = 0, ParticleGunZPos2 = 0, apertureDiameter =0,gap=0;
    G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box* worldBox = NULL;
    if(worldLV) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    if(worldBox) ParticleGunZPos1 = worldBox->GetZHalfLength();
    else {
        G4cerr << "World volume of box not found. " << '\n'
               << "Perhaps you changed geometry." << '\n'
               << "The gun will be place in the center. " << G4endl;
    }

    // Method 2
    ParticleGunZPos2 = 0.5*(fDetector->GetWorldFullLength());
    apertureDiameter = fDetector->GetApertureDiameter();
    gap = apertureDiameter*2/10;
    if(ParticleGunZPos1==ParticleGunZPos2)
    {
        for(G4int i=0;i<11;i++)
        {
          for(G4int j=0;j<11;j++){
          fParticleGun->SetParticlePosition(G4ThreeVector(-apertureDiameter+j*gap,
                                                          apertureDiameter-i*gap,
                                                          -ParticleGunZPos1+1000*mm));
          fParticleGun->GeneratePrimaryVertex(anEvent);
          }
        }
    }
    else
    { G4cerr << "Method1 Position " << ParticleGunZPos1 << " mm" << '\t'
              << "Method2 Position " << ParticleGunZPos2 << " mm" << G4endl;}

}


