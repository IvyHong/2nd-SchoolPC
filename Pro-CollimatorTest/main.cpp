#include <iostream>
#include "G4UImanager.hh"
#include "G4RunManager.hh"

#include "ProDetectorConstruction.hh"
#include "PhysicsList.h"
#include "ProPrimaryGeneratorAction.hh"
#include "ProRunAction.hh"
#include "ProEventAction.hh"
#include "ProAnalysis.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


#include "time.h"
#include "stdio.h"

struct tm *ptr;

int main(int argc,char** argv)
{

    time_t lt;
    lt = time(NULL);
    ptr = gmtime(&lt);
    printf(asctime(ptr)); // Greenwich Mean time (GMT)
    printf(ctime(&lt)); //local time

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    G4long seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);

    G4RunManager* runManager = new G4RunManager;

    ProDetectorConstruction* detectorCon = new ProDetectorConstruction();
    runManager->SetUserInitialization(detectorCon);

    PhysicsList* physics = new PhysicsList();
    runManager->SetUserInitialization(physics);

    ProAnalysis * anly = new ProAnalysis();

    // UserAction classes - optional
    G4VUserPrimaryGeneratorAction* generator = new ProPrimaryGeneratorAction(detectorCon);
    runManager->SetUserAction(generator);

    ProRunAction* RunAct = new ProRunAction(anly);
    runManager->SetUserAction(RunAct);
    ProEventAction* EvAct = new ProEventAction(RunAct,anly);
    runManager->SetUserAction(EvAct);

    runManager->Initialize();

#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    G4UImanager* uiManager = G4UImanager::GetUIpointer();

    if(argc != 1){
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command+fileName);
    }
    else {
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
        uiManager->ApplyCommand("/control/execute vis.mac");
#endif
        ui->SessionStart();
        delete ui;
#endif
    }

//*******************************************************************

    G4cout<<" This is a Collimator Test which let neutrons having 180MeV "
          <<" pass through collimator, and two collimator detectors are "
          <<" used to record neutrons information" <<G4endl;
//*******************************************************************

#ifdef G4VIS_USE
    delete visManager;
#endif

    delete anly;
    delete runManager;

    return 0;
}
