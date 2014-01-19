#ifndef NEUTRONHIT_H
#define NEUTRONHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

/// NeutronHit class
///
/// It defines data members to store the the position and momentum
/// of Neutron in a selected volume:

class NeutronHit : public G4VHit
{
  public:
    NeutronHit();
    NeutronHit(G4int i);
    NeutronHit(const NeutronHit&);
    virtual ~NeutronHit();

    // operators
    const NeutronHit& operator=(const NeutronHit&);
    G4int operator==(const NeutronHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    // methods to handle data
    inline void SetCellID(G4int z) { nID = z; }
    inline void SetPosition(G4ThreeVector xyz)  {nPosition=xyz;}
    inline void SetMomentum(G4ThreeVector uvw)  {nMomentum=uvw;}
    inline void SetEnergyDeposite(G4double edep){nEnergyDeposit=edep;}
    inline void SetRot(G4RotationMatrix rmat) { nRot = rmat; }
    inline void SetLogV(G4LogicalVolume* val) { nPLogV = val; }

    // get methods
    inline G4int  GetID() const {return nID;}
    inline G4ThreeVector GetPosition() const {return nPosition;}
    inline G4ThreeVector GetMomentum() const {return nMomentum;}
    inline G4double GetEnergyDeposite() const {return nEnergyDeposit;}
    inline G4RotationMatrix GetRot() const { return nRot; }
    inline const G4LogicalVolume* GetLogV() const { return nPLogV; }

  private:
    G4int nID;
    G4ThreeVector nPosition; // particle position in the sensitive volume
    G4ThreeVector nMomentum; // paticle momentum in the sensitive volume
    G4double      nEnergyDeposit; // energy deposition
    G4RotationMatrix nRot;
    const G4LogicalVolume* nPLogV;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<NeutronHit> NeutronHitsCollection;

extern G4Allocator<NeutronHit> NeutronHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* NeutronHit::operator new(size_t)
{
  void *hit;
  hit = (void *) NeutronHitAllocator.MallocSingle();
  return hit;
}

inline void NeutronHit::operator delete(void *hit)
{
  NeutronHitAllocator.FreeSingle((NeutronHit*) hit);
}


#endif // NEUTRONHIT_H
