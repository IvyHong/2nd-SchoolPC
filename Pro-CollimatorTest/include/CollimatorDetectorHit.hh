#ifndef COLLIMATORDETECTORHIT_HH
#define COLLIMATORDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/// COLLIMATORDETECTORHIT class
///
/// It defines data members to store the the position and momentum
/// of Neutron in a selected volume:

class CollimatorDetectorHit : public G4VHit
{
  public:
    CollimatorDetectorHit(G4int i);
    CollimatorDetectorHit(const CollimatorDetectorHit&);
    virtual ~CollimatorDetectorHit();

    // operators
    const CollimatorDetectorHit& operator=(const CollimatorDetectorHit&);
    G4int operator==(const CollimatorDetectorHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // methods to handle data
    void SetPosition(G4ThreeVector xyz) {pPosition=xyz;}
    void SetMomentum(G4ThreeVector XYZ) {pMomentum=XYZ;}

    // get methods
    inline G4int GetID() const {return pID;}
    G4ThreeVector GetPosition() const;
    G4ThreeVector GetMomentum() const;

  private:
    G4int pID;
    G4ThreeVector pPosition; // particle position in the sensitive volume
    G4ThreeVector pMomentum; // paticle momentum in the sensitive volume

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<CollimatorDetectorHit> CollimatorDetectorHitsCollection;

extern G4Allocator<CollimatorDetectorHit> CollimatorDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* CollimatorDetectorHit::operator new(size_t)
{
  void *hit;
  hit = (void *) CollimatorDetectorHitAllocator.MallocSingle();
  return hit;
}

inline void CollimatorDetectorHit::operator delete(void *hit)
{
  CollimatorDetectorHitAllocator.FreeSingle((CollimatorDetectorHit*) hit);
}


inline G4ThreeVector CollimatorDetectorHit::GetPosition() const {
  return pPosition;
}

inline G4ThreeVector CollimatorDetectorHit::GetMomentum() const {
  return pMomentum;
}

#endif // COLLIMATORDETECTORHIT_HH
