#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.h"
#include "PhysicsList.h"
#include "PrimaryGeneratorAction.h"

#include "RunAction.h"
#include "EventAction.h"
#include "HistoManager.h"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "time.h"
#include "stdio.h"

int main(int argc,char** argv)
{
/// ****************** Random Number Generator Initialization **********
  // First to choose a engine, otherwise system will choose a
  // default engine HepJamesRandom
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  // Set random seed
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);

  // Eventhough,you run the same events, the simulation results will
  // be different

/// ********************************************************************

  // RunManager construction
  //
  G4RunManager* runManager = new G4RunManager;

#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // mandatory user initialization classes
  DetectorConstruction* detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);

  PhysicsList* physics = new PhysicsList();
  runManager->SetUserInitialization(physics);

  // create an analysis manager object
  HistoManager * analysis = new HistoManager();

  // mandatory user action class
  G4VUserPrimaryGeneratorAction* generator = new PrimaryGeneratorAction(detector);
  runManager->SetUserAction(generator);

  // optional user action classes
  RunAction* RunAct = new RunAction(analysis);
  runManager->SetUserAction(RunAct);

  EventAction* EvAct = new EventAction(RunAct,analysis);
  runManager->SetUserAction(EvAct);

  // initialize Geant4 kernel
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(argc!=1)
  // execute an argument macro file if exist
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else
  // start interactive session
  {

#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif

    ui->SessionStart();    //appear Idle>
    delete ui;
#endif

#ifdef G4VIS_USE
    delete visManager;
#endif
  }


//**************************************************************************

      G4cout<<" Collimator Test : Protons having 180MeV "
            <<" pass through collimator, and two collimator detectors are "
            <<" used to record neutrons information" << G4endl;

//**************************************************************************

      //Job termination

  delete analysis;
  delete runManager;

  return 0;
}
