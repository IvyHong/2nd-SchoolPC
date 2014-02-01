#include <QtCore/QCoreApplication>

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "detectorconstruction.h"
#include "physicslist.h"
#include "primarygeneratoraction.h"

int main(int argc, char** argv)
{
    // Construct the default run manager
    //
    G4RunManager* runManager = new G4RunManager;

    // Set Mandatory initialization classes
    //
    G4VUserDetectorConstruction* detector = new DetectorConstruction;
    runManager->SetUserInitialization(detector);

    //
    G4VUserPhysicsList* physics = new PhysicsList;
    runManager->SetUserInitialization(physics);

    // Set Mandatory user action classes
    //
    G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction;
    runManager->SetUserAction(gen_action);

    // Initialize G4 kernel
    //
    runManager->Initialize();

    // Get the pointer to the UI manager and set verbosities
    //
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    uiManager->ApplyCommand("/run/verbose 1");
    uiManager->ApplyCommand("/event/verbose 1");
    uiManager->ApplyCommand("/tracking/verbose 1");

    // Start a run
    //
    G4int numberOfEvent = 3;
    runManager->BeamOn(numberOfEvent);

    // Job termination
    //
    // Free the store: user actions, physics_list and detector_description are
    //                 owned and deleted by the run manager, so they should not
    //                 be deleted in the main() program !
    delete runManager;

    return 0;
}
