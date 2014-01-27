#ifndef PROGAMMAHIT_HH
#define PROGAMMAHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/// ProGammaHit class
///
/// It defines data members to store the the position and momentum
/// of Gamma in a selected volume:


class ProGammaHit : public G4VHit
{
  public:
    ProGammaHit();
    ProGammaHit(const ProGammaHit&);
    virtual ~ProGammaHit();

    // operators
    const ProGammaHit& operator=(const ProGammaHit&);
    G4int operator==(const ProGammaHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // methods to handle data
    void SetPosition(G4ThreeVector xyz) {gPosition=xyz;}
    void SetMomentum(G4ThreeVector XYZ) {gMomentum=XYZ;}


    // get methods
    G4ThreeVector GetPosition() const;
    G4ThreeVector GetMomentum() const;


  private:
    G4ThreeVector gPosition;        // particle position in the sensitive volume
    G4ThreeVector gMomentum;        // paticle momentum in the  sensitive volume

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<ProGammaHit> ProGammaHitsCollection;

extern G4Allocator<ProGammaHit> ProGammaHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ProGammaHit::operator new(size_t)
{
  void *hit;
  hit = (void *) ProGammaHitAllocator.MallocSingle();
  return hit;
}

inline void ProGammaHit::operator delete(void *hit)
{
  ProGammaHitAllocator.FreeSingle((ProGammaHit*) hit);
}


inline G4ThreeVector ProGammaHit::GetPosition() const {
  return gPosition;
}

inline G4ThreeVector ProGammaHit::GetMomentum() const {
  return gMomentum;
}


#endif // PROGAMMAHIT_HH
