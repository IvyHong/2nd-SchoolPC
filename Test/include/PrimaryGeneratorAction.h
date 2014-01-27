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

private:

// Date member
    G4ParticleGun* fParticleGun;
    DetectorConstruction* fDetector;
    G4ParticleDefinition* fProton;
    G4ParticleDefinition* fNeutron;
    G4double fParticleGunZPos;

    G4double fMomentum;
    G4double fSigmaMomentum;
    G4double fSigmaAngle;
    G4double fRandomizePrimary;

public:
    inline void SetMomentum(G4double val) { fMomentum = val; }
    inline G4double GetMomentum() const { return fMomentum; }
    inline void SetSigmaMomentum(G4double val) { fSigmaMomentum = val; }
    inline G4double GetSigmaMomentum() const { return fSigmaMomentum; }
    inline void SetSigmaAngle(G4double val) { fSigmaAngle = val; }
    inline G4double GetSigmaAngle() const { return fSigmaAngle; }
    inline void SetRandomize(G4bool val) { fRandomizePrimary = val; }
    inline G4bool GetRandomize() const { return fRandomizePrimary; }
};

#endif // PRIMARYGENERATORACTION_H
