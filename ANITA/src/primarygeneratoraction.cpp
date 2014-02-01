#include "primarygeneratoraction.h"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    G4int n_particle = 1;
    // "numberofparticles" is number of particles to be shoot at one invokation
    // of GeneratePrimaryVertex() method. All paricles are shot with the same physical
    // quantities.
    fparticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    fparticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="proton"));
    fparticleGun->SetParticleEnergy(180.*MeV);
    fparticleGun->SetParticlePosition(G4ThreeVector(-2.0*m,0.0,0.0));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fparticleGun;
}

PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    G4int i = anEvent->GetEventID() % 3;
    G4ThreeVector v(0.0,0.0,1.0);
    switch(i)
    {
      case 0:
        break;
      case 1:
        v.setX(0.1);
        break;
      case 2:
        v.setY(0.1);
        break;
    }
    fparticleGun->SetParticleMomentumDirection(v);
    fparticleGun->GeneratePrimaryVertex(anEvent);
}
