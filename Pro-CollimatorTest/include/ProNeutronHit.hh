#ifndef PRONEUTRONHIT_HH
#define PRONEUTRONHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// ProNeutronhit class
///
/// It defines data members to store the the position and momentum
/// of Neutron in a selected volume:

class ProNeutronHit : public G4VHit
{
  public:
    ProNeutronHit();        // Default Constructor
    ProNeutronHit(G4int i);
    ProNeutronHit(const ProNeutronHit&);  // Copy Constructor
    virtual ~ProNeutronHit();

    // operators
    const ProNeutronHit& operator=(const ProNeutronHit&);
    G4int operator==(const ProNeutronHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    inline void SetID(G4int z) { fID = z; }
    inline G4int GetID() const { return fID; }
    inline void SetTime(G4double t) { fTime = t; }
    inline G4double GetTime() const { return fTime; }
    inline void SetLocalPos(G4ThreeVector xyz) { fLocalPos = xyz; }
    inline G4ThreeVector GetLocalPos() const { return fLocalPos; }
    inline void SetWorldPos(G4ThreeVector XYZ) { fWorldPos = XYZ; }
    inline G4ThreeVector GetWorldPos() const { return fWorldPos; }
    inline void SetKinE(G4double Engy) {fKinE = Engy;}
    inline G4double GetKinE() const {return fKinE;}
    inline void SetMomentum(G4ThreeVector momentum) {fMomentum=momentum;}
    inline G4ThreeVector GetMomentum() const {return fMomentum;}
    inline void SetDirection(G4ThreeVector dir) {fDirection=dir;}
    inline G4ThreeVector GetDirection() const {return fDirection;}

  private:
    G4int fID;
    G4double fTime;
    G4ThreeVector fLocalPos;
    G4ThreeVector fWorldPos;
    G4double fKinE;
    G4ThreeVector fMomentum;
    G4ThreeVector fDirection;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<ProNeutronHit> ProNeutronHitsCollection;

extern G4Allocator<ProNeutronHit> ProNeutronHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ProNeutronHit::operator new(size_t)
{
  void *hit;
  hit = (void *) ProNeutronHitAllocator.MallocSingle();
  return hit;
}

inline void ProNeutronHit::operator delete(void *hit)
{
  ProNeutronHitAllocator.FreeSingle((ProNeutronHit*) hit);
}


#endif // PRONEUTRONHIT_HH
