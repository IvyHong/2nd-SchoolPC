#include	"ProRunAction.hh"
#include	"ProAnalysis.hh"

#include	"G4Run.hh"
#include	"G4RunManager.hh"
#include	"G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProRunAction::ProRunAction(ProAnalysis*	aly)
    :fanalysis(aly)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ProRunAction::~ProRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProRunAction::BeginOfRunAction(const G4Run* aRun)
{
    Mytime.Start();  //Recording time started

    G4cout	<<	"### Run	"<<	aRun->GetRunID()	<<	" start."	<<	G4endl;
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);

    fSumEKin_SD1 = fSum2EKin_SD1 = 0.;
    fSumEKin_SD2 = fSum2EKin_SD2 = 0.;
    fSumEKin_SD3 = fSum2EKin_SD3 = 0.;


    fanalysis->Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void ProRunAction::fillPerEvent(G4double KinESd1, G4double KinESd2,
                                G4double KinESd3)
{
  //accumulate statistic
  //
  fSumEKin_SD1 += KinESd1; fSum2EKin_SD1 += KinESd1*KinESd1;
  fSumEKin_SD2 += KinESd2; fSum2EKin_SD2 += KinESd1*KinESd2;
  fSumEKin_SD3 += KinESd3; fSum2EKin_SD3 += KinESd1*KinESd3;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ProRunAction::EndOfRunAction(const G4Run* aRun)
{
    G4int NbOfEvents =	aRun->GetNumberOfEvent();
    if	(NbOfEvents	==	0)	return;

    G4cout	<<	"****" << NbOfEvents << " in "
             << aRun->GetRunID() << " Run****** end." << G4endl;

/// *******************************************************************
    //compute statistics: mean and rms
     //
     fSumEKin_SD1 /= NbOfEvents; fSum2EKin_SD1 /= NbOfEvents;
     G4double rmsEKin_SD1 = fSum2EKin_SD1 - fSumEKin_SD1*fSumEKin_SD1;
     if (rmsEKin_SD1 >0.) rmsEKin_SD1 = std::sqrt(rmsEKin_SD1);
     else rmsEKin_SD1 = 0.;

     fSumEKin_SD2 /= NbOfEvents; fSum2EKin_SD2 /= NbOfEvents;
     G4double rmsEKin_SD2 = fSum2EKin_SD2 - fSumEKin_SD2*fSumEKin_SD2;
     if (rmsEKin_SD2 >0.) rmsEKin_SD2 = std::sqrt(rmsEKin_SD2);
     else rmsEKin_SD2 = 0.;


     fSumEKin_SD3 /= NbOfEvents; fSum2EKin_SD3 /= NbOfEvents;
     G4double rmsEKin_SD3 = fSum2EKin_SD3 - fSumEKin_SD3*fSumEKin_SD3;
     if (rmsEKin_SD3 >0.) rmsEKin_SD3 = std::sqrt(rmsEKin_SD3);
     else rmsEKin_SD3 = 0.;

    //print
    //
    G4cout
       << "\n--------------------End of Run------------------------------\n"
       << "\n mean Energy in SD1 : " << G4BestUnit(fSumEKin_SD1,"Energy")
       << " +- " << G4BestUnit(rmsEKin_SD1,"Energy")
       << "\n mean Energy in SD2 : " << G4BestUnit(fSumEKin_SD2,"Energy")
       << " +- " << G4BestUnit(rmsEKin_SD2,"Energy")
       << "\n mean Energy in SD3 : " << G4BestUnit(fSumEKin_SD3,"Energy")
       << " +- " << G4BestUnit(rmsEKin_SD3,"Energy")
       << G4endl;

/// ********************************************************************

    fanalysis->PrintStatistic();
    fanalysis->Save();


    Mytime.Stop(); // Recording time stopped
    G4cout<<"My time [s] : " << Mytime << G4endl;
    /* loopElapsedTime = Mytime.GetUserElapsed();
G4cout << "loop elapsed time [s] : "<< loopElapsedTime << G4endl; */

}

