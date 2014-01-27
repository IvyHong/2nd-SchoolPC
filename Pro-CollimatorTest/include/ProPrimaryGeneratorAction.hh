#ifndef PROPRIMARYGENERATIONACTION_HH
#define PROPRIMARYGENERATIONACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class ProDetectorConstruction;
class G4ParticleGun;
class G4Event;
class G4ParticleDefinition;

class ProPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    ProPrimaryGeneratorAction(ProDetectorConstruction* );
    virtual ~ProPrimaryGeneratorAction();

public:
    //method from the base class
    //
    virtual void GeneratePrimaries(G4Event* );

public:

    inline void SetMomentum(G4double val) { fMomentum = val; }
    inline G4double GetMomentum() const { return fMomentum; }
    inline void SetSigmaMomentum(G4double val) { fSigmaMomentum = val; }
    inline G4double GetSigmaMomentum() const { return fSigmaMomentum; }
    inline void SetSigmaAngle(G4double val) { fSigmaAngle = val; }
    inline G4double GetSigmaAngle() const { return fSigmaAngle; }
    inline void SetRandomize(G4bool val) { fRandomizePrimary = val; }
    inline G4bool GetRandomize() const { return fRandomizePrimary; }

private:

    // Date member
    //
    G4ParticleGun*            fParticleGun;
    ProDetectorConstruction*  fDetector;
    G4ParticleDefinition*     fProton;

    G4double fMomentum;
    G4double fSigmaMomentum;
    G4double fSigmaAngle;
    G4bool   fRandomizePrimary;

};

#endif // PROPRIMARYGENERATIONACTION_HH
