#include "ProPrimaryGeneratorAction.hh"
#include "ProDetectorConstruction.hh"

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

/// *******************************************************************

ProPrimaryGeneratorAction::ProPrimaryGeneratorAction(ProDetectorConstruction* detector)
    :G4VUserPrimaryGeneratorAction(),
      fParticleGun(0),fDetector(detector)
{
  fMomentum = 180.*MeV;
  fSigmaMomentum = 9.*MeV;
  fSigmaAngle = 2.*deg;
  fRandomizePrimary = true;

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fProton = particleTable->FindParticle(particleName="proton");

  // default particle information
  //
  fParticleGun->SetParticleDefinition(fProton);

/// *******************************************************************
  // Method 1
  G4double ParticleGunZPos1 = 0*mm, ParticleGunZPos2 = 0.*mm, ParticleGunZPos =0.*mm;
  G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = NULL;
  if(worldLV) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if(worldBox) ParticleGunZPos1 = worldBox->GetZHalfLength();
  else {
      G4cerr << "World volume of box not found. " << '\n'
             << "Perhaps you changed geometry."   << '\n'
             << "The gun will be place in the center. " << G4endl;
  }

  // Method 2
  ParticleGunZPos2 = 0.5*(fDetector->GetWorldFullLength());
  ParticleGunZPos = 0.5*(fDetector->GetTargetLength());
  if(ParticleGunZPos1==ParticleGunZPos2)
  {
      fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-ParticleGunZPos));
  }
  else
  {  G4cerr << "Method1 Position : " << ParticleGunZPos1 << " mm" << '\t'
            << " != " << '\t'
            << "Method2 Position : " << ParticleGunZPos2 << " mm" << G4endl;
  }

 /// ******************************************************************


}

ProPrimaryGeneratorAction::~ProPrimaryGeneratorAction()
{
    delete fParticleGun;
}

void ProPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4ParticleDefinition* particle;

    if(fRandomizePrimary)
    {
        // G4UniformRand() is between 0 and 1
        //
        particle = fParticleGun->GetParticleDefinition();
    }

    /// Energy and Momentum Direction are randomly
    /// relation between momentum and energy
    ///
    G4double pp = fMomentum + (G4UniformRand()-0.5)*fSigmaMomentum;
    G4double mass = particle->GetPDGMass();
    G4double Ekin = std::sqrt(pp*pp+mass*mass)-mass;
    fParticleGun->SetParticleEnergy(Ekin);

    G4double angle = (G4UniformRand()-0.5)*fSigmaAngle;
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(angle),0.,std::cos(angle)));



    fParticleGun->GeneratePrimaryVertex(anEvent);
}


