// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.h"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

HistoManager* HistoManager::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
{
    // ROOT style
    gROOT->Reset();

    // define histograms
    incident_map = new TH2D("Incident map","Incident Distribution",
                            50,-5.,5.,
                            50,-5.,5.);
    incident_map->GetXaxis()->SetTitle("X (cm)");
    incident_map->GetYaxis()->SetTitle("Y (cm)");
    incident_map->SetStats(0);

    incident_x_hist = new TH1D("incident x","Incident x",100,-5.,5.);
    incident_x_hist->GetXaxis()->SetTitle("X (cm)");
    incident_x_hist->SetFillColor(kRed);

    dose_map = new TH2D("Dose map","Dose Distribution",
                            500,0.,50.,
                            200,-10.,10.);
    dose_map->GetXaxis()->SetTitle("Z (cm)");
    dose_map->GetYaxis()->SetTitle("X (cm)");
    dose_map->SetStats(0);

    dose_hist = new TH1D("incident x","Incident x",500,0.,50.);
    dose_hist->GetXaxis()->SetTitle("Z (cm)");
    dose_hist->GetYaxis()->SetTitle("Dose (MeV)");
    dose_hist->SetFillColor(kBlue);
    dose_hist->SetStats(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete incident_map;
  delete incident_x_hist;
  delete dose_map;
  delete dose_hist;

  fInstance=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager* HistoManager::getInstance()
{
    if(fInstance==0)
    {
        fInstance = new HistoManager();
    }
    return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Update()
{
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Clear()
{
    incident_map->Reset();
    incident_x_hist->Reset();
    dose_map->Reset();
    dose_hist->Reset();

    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
    TFile* file = new TFile("Test_root","RECREATE","Geant4 ROOT analysis");
    incident_map->Write();
    incident_x_hist->Write();
    dose_map->Write();
    dose_hist->Write();

    file->Close();
    delete file;

    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillIncident(const G4ThreeVector &p)
{
    if(! incidentFlag){
        incident_map->Fill(p.x()/cm,p.y()/cm);
        incident_x_hist->Fill(p.x()/cm);

        incidentFlag = true;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillDose(const G4ThreeVector &p, G4double dedx)
{
    const G4double z0 = 25*cm;
    const G4double dxy = 10.*mm;

    if(std::abs(p.y())<dxy)
    {
        dose_map->Fill((p.z()+z0)/cm,p.x()/cm,dedx/MeV);

        if(std::abs(p.x()<dxy))
        {
            dose_hist->Fill((p.z()+z0)/cm,dedx/MeV);
        }
    }

}
