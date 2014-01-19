#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class DetectorConstruction;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(DetectorConstruction* );
    virtual ~PrimaryGeneratorAction();

public:
   //method from the base class
    virtual void GeneratePrimaries(G4Event* );

private:

// Date member
    G4ParticleGun* fParticleGun;
    DetectorConstruction* fDetector;
};

#endif // PRIMARYGENERATORACTION_H
