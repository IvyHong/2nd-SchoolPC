#ifndef HADCALORIMETERHIT_H
#define HADCALORIMETERHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class HadCalorimeterHit : public G4VHit
{
  public:
      HadCalorimeterHit();
      HadCalorimeterHit(G4int iCol,G4int iRow);
      virtual ~HadCalorimeterHit();
      HadCalorimeterHit(const HadCalorimeterHit &right);
      const HadCalorimeterHit& operator=(const HadCalorimeterHit &right);
      int operator==(const HadCalorimeterHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4int fColumnID;
      G4int fRowID;
      G4double fEdep;
      G4ThreeVector fPos;
      G4RotationMatrix fRot;

  public:
      inline void SetColumnID(G4int z) { fColumnID = z; }
      inline G4int GetColumnID() const { return fColumnID; }
      inline void SetRowID(G4int z) { fRowID = z; }
      inline G4int GetRowID() const { return fRowID; }
      inline void SetEdep(G4double de) { fEdep = de; }
      inline void AddEdep(G4double de) { fEdep += de; }
      inline G4double GetEdep() const { return fEdep; }
      inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
      inline G4ThreeVector GetPos() const { return fPos; }
      inline void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
      inline G4RotationMatrix GetRot() const { return fRot; }

};

typedef G4THitsCollection<HadCalorimeterHit> HadCalorimeterHitsCollection;

extern G4Allocator<HadCalorimeterHit> HadCalorimeterHitAllocator;

inline void* HadCalorimeterHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)HadCalorimeterHitAllocator.MallocSingle();
  return aHit;
}

inline void HadCalorimeterHit::operator delete(void* aHit)
{
  HadCalorimeterHitAllocator.FreeSingle((HadCalorimeterHit*) aHit);
}

#endif // HADCALORIMETERHIT_H
