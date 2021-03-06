#ifndef EMCALORIMETERHIT_H
#define EMCALORIMETERHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ParticleDefinition.hh"

class G4AttDef;
class G4AttValue;

class EmCalorimeterHit : public G4VHit
{
  public:
      EmCalorimeterHit();
      EmCalorimeterHit(G4int z);
      virtual ~EmCalorimeterHit();
      EmCalorimeterHit(const EmCalorimeterHit &right);
      const EmCalorimeterHit& operator=(const EmCalorimeterHit &right);
      int operator==(const EmCalorimeterHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4int fCellID;
      G4double fKinE;
      G4double fEdep;
      G4ParticleDefinition* fPD;
      G4ThreeVector fPos;
      G4RotationMatrix fRot;
      const G4LogicalVolume* fPLogV;

  public:
      // Kinetic Energy
      //
      inline void SetKineticEnergy(G4double de) { fKinE = de; }
      inline G4double GetKineticEnergy() const  { return fKinE;}
      // Deposited Energy
      //
      inline void AddEdep(G4double de) { fEdep += de; }
      inline G4double GetEdep() const  { return fEdep; }
      // Particle name
      //
      inline void SetParticleDef (G4ParticleDefinition* pd) {fPD = pd;}
      inline G4ParticleDefinition* GetParticleDef() const {return fPD;}
      // Position Vector
      //
      inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
      inline G4ThreeVector GetPos() const   { return fPos; }
      // Rotation matrix
      //
      inline void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
      inline G4RotationMatrix GetRot() const { return fRot; }
      // Logical volume
      //
      inline void SetLogV(G4LogicalVolume* val) { fPLogV = val; }
      inline const G4LogicalVolume* GetLogV() const { return fPLogV; }
};

typedef G4THitsCollection<EmCalorimeterHit> EmCalorimeterHitsCollection;

extern G4Allocator<EmCalorimeterHit> EmCalorimeterHitAllocator;

inline void* EmCalorimeterHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)EmCalorimeterHitAllocator.MallocSingle();
  return aHit;
}

inline void EmCalorimeterHit::operator delete(void* aHit)
{
  EmCalorimeterHitAllocator.FreeSingle((EmCalorimeterHit*) aHit);
}

#endif // EMCALORIMETERHIT_H
