#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include  "HistoManager.h"
#include  "G4UnitsTable.hh"
#include  "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/** Constructs a default Histomanager.
*/
HistoManager::HistoManager()
    :rootFile(0),ntupl(0),Eabs(0),Egap(0),Labs(0),Lgap(0)
{
    // histograms
    for (G4int k=0; k<MaxHisto; k++)
    {
        histo[k] = 0;
    }

    // ntuple
    ntupl=0;
}


/** Destroys a Histomanager.
*/
HistoManager::~HistoManager()
{
    if(rootFile)
    {
        delete rootFile;
    }
}

/**

*/
void HistoManager::book()
{
    // Create or get analysis manager
    // The choice of analysis technology is done via selection of a namespace
    // in HistoManager.hh

    // Creating a tree container to handle histograms and ntuples.
    // This tree is associated to an output file "rootFile"
    //

    G4String fileName = "Test.root";

    //  To create a TFile object corresponding to a new file called
    //  "Test.root" (.root is preferred extension)

    rootFile = new TFile(fileName,"RECREATE");

    //  Other options are CREATE(same as NEW),RECREATE(i.e. replace)
    //  UPDATE and READ.

    //  To check that file was successfully opened.

    if(!rootFile) {
      G4cout << " HistoManager::book :"
             << " problem creating the ROOT TFile "
             << G4endl;
      return;
    }

    if(rootFile->IsOpen())
        G4cout << " File opened successfully \n";

    //  An histogram is created by invoking one of the class constructors. For example,
    //  TH1D(const char *name,const char *title,Int_t nbinsx,Double_t xlow,Double_t xup);

    histo[1] = new TH1D("1", "Edep in absorber", 100, 0., 800*CLHEP::MeV);
    if (!histo[1]) G4cout << "\n can't create histo 1" << G4endl;
    histo[2] = new TH1D("2", "Edep in gap", 100, 0., 100*CLHEP::MeV);
    if (!histo[2]) G4cout << "\n can't create histo 2" << G4endl;
    histo[3] = new TH1D("3", "trackL in absorber", 100, 0., 1*CLHEP::m);
    if (!histo[3]) G4cout << "\n can't create histo 3" << G4endl;
    histo[4] = new TH1D("4", "trackL in gap", 100, 0., 50*CLHEP::cm);
    if (!histo[4]) G4cout << "\n can't create histo 4" << G4endl;

    // create 1 ntuple in subdirectory "tuples"
    // Create a TTree (name,title)

    ntupl = new TTree("101", "Edep and TrackL");
    ntupl->Branch("Eabs", &Eabs, "Eabs/D");
    ntupl->Branch("Egap", &Egap, "Egap/D");
    ntupl->Branch("Labs", &Labs, "Labs/D");
    ntupl->Branch("Lgap", &Lgap, "Lgap/D");

    //  create branch with all information from the structure
    //  tree->Branch(branchname, &value,leaflist,bufsize);
    /*
       * address is the address of the first item of a structure
       * leaflist is the concatenation of all the variable names and types
         separated by a colon character :
         The variable name and the variable type are separated by a
         slash (/). The variable type must be 1 character. (Characters
         after the first are legal and will be appended to the visible
         name of the leaf, but have no effect.) If no type is given, the
         type of the variable is assumed to be the same as the previous
         variable. If the first variable does not have a type, it is
         assumed of type F by default. The list of currently supported
         types is given below:
            - C : a character string terminated by the 0 character
            - B : an 8 bit signed integer (Char_t)
            - b : an 8 bit unsigned integer (UChar_t)
            - S : a 16 bit signed integer (Short_t)
            - s : a 16 bit unsigned integer (UShort_t)
            - I : a 32 bit signed integer (Int_t)
            - i : a 32 bit unsigned integer (UInt_t)
            - F : a 32 bit floating point (Float_t)
            - D : a 64 bit floating point (Double_t)
            - L : a 64 bit signed integer (Long64_t)
            - l : a 64 bit unsigned integer (ULong64_t)
            - O : [the letter 'o', not a zero] a boolean (Bool_t)
     */

    G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{
  if (rootFile) {
    rootFile->Write(); // Writing the histograms to the file
    rootFile->Close(); // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }

  //    Filling histograms
  //    Histogram of all types are filled via the hist->Fill(..) function
  //    h1->Fill(x, w); //fill with weight

  if (histo[ih]) { histo[ih]->Fill(xbin, weight); }

//  Int_t Fill(Double_t x, Double_t w)
//  -*-*-*-*-*-*Increment bin with abscissa X with a weight w*-*-*-*-*-*-*-*


//   if x is less than the low-edge of the first bin, the Underflow bin is incremented
//   if x is greater than the upper edge of last bin, the Overflow bin is incremented

//   If the storage of the sum of squares of weights has been triggered,
//   via the function Sumw2, then the sum of the squares of weights is incremented
//   by w^2 in the bin corresponding to x.

//  -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }

  if (histo[ih]) histo[ih]->Scale(fac);

//  void Scale(Double_t c1 = 1, Option_t* option = "")

//     -*-*-*Multiply this histogram by a constant c1*-*-*-*-*-*-*-*-*


//     this = c1*this

//   Note that both contents and errors(if any) are scaled.
//   This function uses the services of TH1::Add

//   IMPORTANT NOTE: If you intend to use the errors of this histogram later
//   you should call Sumw2 before making this operation.
//   This is particularly important if you fit the histogram after TH1::Scale

//   One can scale an histogram such that the bins integral is equal to
//   the normalization parameter via TH1::Scale(Double_t norm), where norm
//   is the desired normalization divided by the integral of the histogram.

//   If option contains "width" the bin contents and errors are divided
//   by the bin width.


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(G4double energyAbs, G4double energyGap,
                              G4double trackLAbs , G4double trackLGap )
{
 Eabs = energyAbs;
 Egap = energyGap;
 Labs = trackLAbs;
 Lgap = trackLGap;

  if (ntupl) ntupl->Fill();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  if(histo[1]) {
    G4cout << "\n ----> print histograms statistic \n" << G4endl;
/*
////Double_t GetRMS(Int_t axis = 1) const

    For axis = 1,2 or 3 returns the Sigma value of the histogram along
    X, Y or Z axis
    For axis = 11, 12 or 13 returns the error of RMS estimation along
    X, Y or Z axis for Normal distribution

    Note that the mean value/sigma is computed using the bins in the currently
    defined range (see TAxis::SetRange). By default the range includes
    all bins from 1 to nbins included, excluding underflows and overflows.
    To force the underflows and overflows in the computation, one must
    call the static function TH1::StatOverflows(kTRUE) before filling
    the histogram.
    Note that this function returns the Standard Deviation (Sigma)
    of the distribution (not RMS).
    The Sigma estimate is computed as Sqrt((1/N)*(Sum(x_i-x_mean)^2))
    The name "RMS" was introduced many years ago (Hbook/PAW times).
    We kept the name for continuity.

*/
    G4cout
    << " EAbs : mean = " << G4BestUnit(histo[1]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[1]->GetRMS(), "Energy") << G4endl;
    G4cout
    << " EGap : mean = " << G4BestUnit(histo[2]->GetMean(), "Energy")
            << " rms = " << G4BestUnit(histo[2]->GetRMS(), "Energy") << G4endl;
    G4cout
    << " LAbs : mean = " << G4BestUnit(histo[3]->GetMean(), "Length")
            << " rms = " << G4BestUnit(histo[3]->GetRMS(), "Length") << G4endl;
    G4cout
    << " LGap : mean = " << G4BestUnit(histo[4]->GetMean(), "Length")
            << " rms = " << G4BestUnit(histo[4]->GetRMS(), "Length") << G4endl;



/*
Double_t GetMean(Int_t axis = 1) const

  For axis = 1,2 or 3 returns the mean value of the histogram along
  X,Y or Z axis.
  For axis = 11, 12, 13 returns the standard error of the mean value
  of the histogram along X, Y or Z axis

  Note that the mean value/RMS is computed using the bins in the currently
  defined range (see TAxis::SetRange). By default the range includes
  all bins from 1 to nbins included, excluding underflows and overflows.
  To force the underflows and overflows in the computation, one must
  call the static function TH1::StatOverflows(kTRUE) before filling
  the histogram.

*/

  }
}
