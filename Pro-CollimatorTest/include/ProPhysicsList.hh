#ifndef PROPHYSICLIST_HH
#define PROPHYSICLIST_HH

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class G4VPhysicsConstructor;

class ProPhysicsList: public G4VUserPhysicsList
{
  public:
    ProPhysicsList();
   ~ProPhysicsList();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

    void SetCuts();


  protected:
    // these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();
    void ConstructShortlived();

  protected:
//    // these methods Construct physics processes and register them
//    void ConstructGeneral();
//    void ConstructEM();
//    void AddStepMax();

    G4DecayPhysics* fDecayPhysics;
    G4RadioactiveDecayPhysics* fRadioactiveDecayPhysics;
    G4EmStandardPhysics* fEmPhysicsList;
};

#endif // PROPHYSICLIST_HH
