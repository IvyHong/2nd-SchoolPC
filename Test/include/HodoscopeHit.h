#ifndef HODOSCOPEHIT_H
#define HODOSCOPEHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class HodoscopeHit : public G4VHit
{
  public:
      HodoscopeHit(G4int i,G4double t);
      virtual ~HodoscopeHit();
      HodoscopeHit(const HodoscopeHit &right);
      const HodoscopeHit& operator=(const HodoscopeHit &right);
      int operator==(const HodoscopeHit &right) const;


      inline void *operator new(size_t);
      inline void operator delete(void*aHit);

      void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      void Print();

  private:
      G4int fId;
      G4double fTime;
      G4ThreeVector fPos;
      G4RotationMatrix fRot;
      const G4LogicalVolume* fPLogV;

  public:
      inline G4int GetID() const { return fId; }
      inline G4double GetTime() const { return fTime; }
      inline void SetTime(G4double val) { fTime = val; }
      inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
      inline G4ThreeVector GetPos() const { return fPos; }
      inline void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
      inline G4RotationMatrix GetRot() const { return fRot; }
      inline void SetLogV(G4LogicalVolume* val) { fPLogV = val; }
      inline const G4LogicalVolume* GetLogV() const { return fPLogV; }
};

typedef G4THitsCollection<HodoscopeHit> HodoscopeHitsCollection;

extern G4Allocator<HodoscopeHit> HodoscopeHitAllocator;

inline void* HodoscopeHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)HodoscopeHitAllocator.MallocSingle();
  return aHit;
}

inline void HodoscopeHit::operator delete(void*aHit)
{
  HodoscopeHitAllocator.FreeSingle((HodoscopeHit*) aHit);
}



#endif // HODOSCOPEHIT_H
