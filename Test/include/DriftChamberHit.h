#ifndef DRIFTCHAMBERHIT_H
#define DRIFTCHAMBERHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class DriftChamberHit : public G4VHit
{
  public:
      DriftChamberHit();
      DriftChamberHit(G4int z);
      virtual ~DriftChamberHit();
      DriftChamberHit(const DriftChamberHit &right);
      const DriftChamberHit& operator=(const DriftChamberHit &right);
      int operator==(const DriftChamberHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      inline float x();
      inline float y();

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4int fLayerID;
      G4double fTime;
      G4ThreeVector fLocalPos;
      G4ThreeVector fWorldPos;

  public:
      inline void SetLayerID(G4int z) { fLayerID = z; }
      inline G4int GetLayerID() const { return fLayerID; }
      inline void SetTime(G4double t) { fTime = t; }
      inline G4double GetTime() const { return fTime; }
      inline void SetLocalPos(G4ThreeVector xyz) { fLocalPos = xyz; }
      inline G4ThreeVector GetLocalPos() const { return fLocalPos; }
      inline void SetWorldPos(G4ThreeVector xyz) { fWorldPos = xyz; }
      inline G4ThreeVector GetWorldPos() const { return fWorldPos; }
};

typedef G4THitsCollection<DriftChamberHit> DriftChamberHitsCollection;

extern G4Allocator<DriftChamberHit> DriftChamberHitAllocator;

inline void* DriftChamberHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)DriftChamberHitAllocator.MallocSingle();
  return aHit;
}

inline void DriftChamberHit::operator delete(void* aHit)
{
  DriftChamberHitAllocator.FreeSingle((DriftChamberHit*) aHit);
}

#endif // DRIFTCHAMBERHIT_H
