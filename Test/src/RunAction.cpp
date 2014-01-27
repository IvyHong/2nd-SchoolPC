#include "RunAction.h"
#include "G4Timer.hh"
#include "HistoManager.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
    :myTimer(0)
{
    myTimer = new G4Timer;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction()
{
    delete myTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{

    HistoManager* analysis = HistoManager::getInstance();
    analysis->Update();


    G4cout <<"### Run "<< aRun->GetRunID() <<" start.." << G4endl;
    myTimer->Start();

    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    G4int NbOfEvents = aRun->GetNumberOfEvent();

    if (NbOfEvents == 0) return;

    myTimer->Stop();
    G4cout << "The number of event = " << NbOfEvents
           << " " << *myTimer << G4endl;
    // Write histograms to file
    G4cout << "Close and Save Histograms" << G4endl;
    G4cout << "### Run " << aRun->GetRunID() << " ended." << G4endl;


    HistoManager* analysis = HistoManager::getInstance();
    analysis->Save();


}
