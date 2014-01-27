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

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detector)
    :G4VUserPrimaryGeneratorAction(),
      fParticleGun(0),fDetector(detector)
{
  fMomentum = 180.*MeV;
  fSigmaMomentum = fMomentum/20;
  /// fSigmaAngle = std::atan(apertureDiameter*0.5/867*mm);
  fSigmaAngle = 3.36*deg;
  fRandomizePrimary = true;


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

  fParticleGunZPos=-1000.*mm;

  ///@@
  //
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fProton = particleTable->FindParticle(particleName="proton");
  fNeutron= particleTable->FindParticle(particleName="neutron");

  fParticleGun->SetParticleDefinition(fProton);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,fParticleGunZPos));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4ParticleDefinition* particle;

    if(fRandomizePrimary)
    {
        particle = fProton;
        fParticleGun->SetParticleDefinition(particle );
    }

    // G4UniformRand() function is given random number between(0,1)
    // G4int i = (int)(5.*G4UniformRand());


    G4double pp = fMomentum + (G4UniformRand()-0.5)*fSigmaMomentum;
    G4double mass = particle->GetPDGMass();
    G4double Ekin = std::sqrt(pp*pp+mass*mass)-mass;
    fParticleGun->SetParticleEnergy(Ekin);

    G4double angle = (G4UniformRand()-0.5)*fSigmaAngle;
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(angle),0.,std::cos(angle)));

    fParticleGun->GeneratePrimaryVertex(anEvent);

}


