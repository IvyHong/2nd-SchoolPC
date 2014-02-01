#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

public:
    void GeneratePrimaries(G4Event *anEvent);

private:
    G4ParticleGun* fparticleGun;
};

#endif // PRIMARYGENERATORACTION_H
