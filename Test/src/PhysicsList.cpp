#include "PhysicsList.h"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <iomanip>

#include "GeneralPhysics.h"
#include "EMPhysics.h"
#include "MuonPhysics.h"
#include "HadronPhysics.h"
#include "IonPhysics.h"

//#include "HadronPhysicsQGSP_BERT.hh"
//#include "HadronPhysicsQGSP_BIC.hh"

PhysicsList::PhysicsList(): G4VModularPhysicsList()
{

  G4cout << "You are using the PhysicsList" << G4endl;
  G4cout << "Full set of particles (barions bosons and mesons) will be created and" << G4endl;
  G4cout << "Standard EM Physics and Low & High Energy parameterized models will be applied." << G4endl;
  G4cout << "PhysicsList is optimized for robustness" << G4endl;
  G4cout << "and not for any particular usage." << G4endl;
  G4cout << "For the hadronic physics, educated guesses of physics list are prepared for various use cases." << G4endl;
  G4cout << "When you will start REAL calculations for your own interest," << G4endl;
  G4cout << "please consider the usage of hadronic_lists instead of PhysicsLists." << G4endl;
  G4cout << "More information can also be found from the Geant4 HyperNews." << G4endl;
  G4cout << "http://geant4-hn.slac.stanford.edu:5090/Geant4-HyperNews/index" << G4endl;
  G4cout << "" << G4endl;

  // default cut value (1.0mm)
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);

  // General Physics ( Create ALL Particle and apply Decay )
  RegisterPhysics( new GeneralPhysics("general") );

  // EM Physics ( Apply related Processes to gamma and e-/+)
  RegisterPhysics( new EMPhysics("standard EM"));

  // Muon Physics ( Apply related processes to mu and tau
  RegisterPhysics( new MuonPhysics("muon"));

   // Hadron Physics ( Apply related processes to hadrons )
  RegisterPhysics( new HadronPhysics("hadron"));
// We do not use hadronic lists since v7.
  //RegisterPhysics( new HadronPhysicsQGSP_BERT("hadron"));
  //RegisterPhysics( new HadronPhysicsQGSP_BIC("hadron"));

  // Ion Physics ( Apply related processes to ions )
  RegisterPhysics( new IonPhysics("ion"));

}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::SetCuts()
{
  // " G4VUserPhysicsList::SetCutsWithDefault" method sets
  // the default cut value for all particle types
  SetCutsWithDefault();
}
