#include "ProPhysicsList.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ProPhysicsList::ProPhysicsList() :  G4VUserPhysicsList()
{
    // Default: Add Standard Electromagnetic Physics processes
    fEmPhysicsList= new G4EmStandardPhysics(0,"Standard EM");

    //Default physics: Add Decay Ions Stopping GN();
    fDecayPhysics =new G4DecayPhysics("Decay");

    // Default physics
    fRadioactiveDecayPhysics = new G4RadioactiveDecayPhysics("RadioactiveDecay");}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ProPhysicsList::~ProPhysicsList()
{    delete fEmPhysicsList;
     delete fDecayPhysics;
     delete fRadioactiveDecayPhysics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ProPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructIons();
  ConstructShortlived();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ProPhysicsList::ConstructBosons()
{
  // Construct all Bosons
  G4BosonConstructor bConstructor;
  bConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ProPhysicsList::ConstructLeptons()
{
  // Construct all leptons
    G4LeptonConstructor lConstructor;
    lConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ProPhysicsList::ConstructMesons()
{
 //  Construct all mesons
    G4MesonConstructor mConstructor;
    mConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ProPhysicsList::ConstructBaryons()
{
//Construct all barions
    G4BaryonConstructor bbConstructor;
    bbConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void ProPhysicsList::ConstructIons()
{
 //  Construct all ions
    G4IonConstructor iConstructor;
    iConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ProPhysicsList::ConstructShortlived()
{
   // Construct all short lived
    G4ShortLivedConstructor sConstructor;
    sConstructor.ConstructParticle();
}
//********************************************************************************

#include "G4HadronElasticPhysicsHP.hh"
#include "HadronPhysicsQGSP_BIC_HP.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"

void ProPhysicsList::ConstructProcess()
{
    AddTransportation();
//  ConstructGeneral();
//  ConstructEM();

   // Simulation A only Binary cascade model
    HadronPhysicsQGSP_BIC_HP* QGSP_BIC_HP=new HadronPhysicsQGSP_BIC_HP();
    QGSP_BIC_HP->ConstructProcess();

//  //Simulation B
//  HadronPhysicsQGSP_BERT_HP* QGSP_BERT_HP=new HadronPhysicsQGSP_BERT_HP();
//  QGSP_BERT_HP->ConstructProcess();

  G4HadronElasticPhysicsHP* elasticPhysics =new G4HadronElasticPhysicsHP();
  elasticPhysics->ConstructProcess();

  fEmPhysicsList->ConstructProcess();
  fDecayPhysics->ConstructProcess();
  fRadioactiveDecayPhysics->ConstructProcess();

  G4cout<< "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n" << G4endl;
}


//**********************************************************************************
void ProPhysicsList::SetCuts()
{
  if (verboseLevel >1){
    G4cout << "PhysicsList::SetCuts:";
  }
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}
