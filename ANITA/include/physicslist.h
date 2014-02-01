#ifndef PHYSICSLIST_H
#define PHYSICSLIST_H

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsList : public G4VUserPhysicsList
{
public:
    PhysicsList();
    ~PhysicsList();

protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
};

#endif // PHYSICSLIST_H
