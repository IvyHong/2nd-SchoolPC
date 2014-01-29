#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class DetectorConstruction;
class G4ParticleGun;
class G4ParticleDefinition;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(DetectorConstruction* );
    virtual ~PrimaryGeneratorAction();

public:
   //method from the base class
    virtual void GeneratePrimaries(G4Event* );
    static G4ParticleGun* Gun() {return fParticleGun;}

private:

// Date member
    static G4ParticleGun* fParticleGun;
    DetectorConstruction* fDetector;
    G4ParticleDefinition* fProton;
    G4ParticleDefinition* fNeutron;
    G4double fParticleGunZPos;
};

#endif // PRIMARYGENERATORACTION_H
