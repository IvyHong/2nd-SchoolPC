#ifndef PHYSICSLIST_H
#define PHYSICSLIST_H

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();

  virtual void SetCuts();
};

#endif // PHYSICSLIST_H
