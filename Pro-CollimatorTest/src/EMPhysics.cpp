#include "EMPhysics.h"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>


EMPhysics::EMPhysics(const G4String& name)
               : G4VPhysicsConstructor(name)
{
}

EMPhysics::~EMPhysics()
{
}

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

#include "G4ProcessManager.hh"

void EMPhysics::ConstructProcess()
{
   G4ProcessManager * pManager = 0;

   //Gamma
   pManager = G4Gamma::Gamma()->GetProcessManager();
   pManager->AddDiscreteProcess(new G4GammaConversion());
   pManager->AddDiscreteProcess(new G4ComptonScattering());
   pManager->AddDiscreteProcess(new G4PhotoElectricEffect());

   //Electorn
   pManager = G4Electron::Electron()->GetProcessManager();
   G4VProcess* theeminusMultipleScattering = new G4eMultipleScattering();
   G4VProcess* theeminusIonisation = new G4eIonisation();
   G4VProcess* theeminusBremsstrahlung = new G4eBremsstrahlung();
   //
   // add process
   pManager->AddProcess(theeminusMultipleScattering);
   pManager->AddProcess(theeminusIonisation);
   pManager->AddProcess(theeminusBremsstrahlung);
   //
   // set ordering for AlongStepDoIt
   pManager->SetProcessOrdering(theeminusMultipleScattering, idxAlongStep,1);
   pManager->SetProcessOrdering(theeminusIonisation, idxAlongStep,2);
   pManager->SetProcessOrdering(theeminusBremsstrahlung, idxAlongStep,3);
   //
   // set ordering for PostStepDoIt
   pManager->SetProcessOrdering(theeminusMultipleScattering, idxPostStep,1);
   pManager->SetProcessOrdering(theeminusIonisation, idxPostStep,2);
   pManager->SetProcessOrdering(theeminusBremsstrahlung, idxPostStep,3);

   //Positron
   pManager = G4Positron::Positron()->GetProcessManager();
   G4VProcess* theeplusMultipleScattering = new G4eMultipleScattering();
   G4VProcess* theeplusIonisation = new G4eIonisation();
   G4VProcess* theeplusBremsstrahlung = new G4eBremsstrahlung();
   G4VProcess* theeplusAnnihilation = new G4eplusAnnihilation();

   pManager->AddProcess(theeplusMultipleScattering);
   pManager->AddProcess(theeplusIonisation);
   pManager->AddProcess(theeplusBremsstrahlung);
   pManager->AddProcess(theeplusAnnihilation);
   //
   // set ordering for AtRestDoIt
   pManager->SetProcessOrderingToFirst(theeplusAnnihilation, idxAtRest);
   //
   // set ordering for AlongStepDoIt
   pManager->SetProcessOrdering(theeplusMultipleScattering, idxAlongStep,1);
   pManager->SetProcessOrdering(theeplusIonisation, idxAlongStep,2);
   pManager->SetProcessOrdering(theeplusBremsstrahlung, idxAlongStep,3);
   //
   // set ordering for PostStepDoIt
   pManager->SetProcessOrdering(theeplusMultipleScattering, idxPostStep,1);
   pManager->SetProcessOrdering(theeplusIonisation, idxPostStep,2);
   pManager->SetProcessOrdering(theeplusBremsstrahlung, idxPostStep,3);
   pManager->SetProcessOrdering(theeplusAnnihilation, idxPostStep,4);

}
