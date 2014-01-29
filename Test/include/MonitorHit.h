#ifndef MONITORHIT_H
#define MONITORHIT_H

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

class MonitorHit : public G4VHit
{
  public:
      MonitorHit();
      MonitorHit(G4int z);
      virtual ~MonitorHit();
      MonitorHit(const MonitorHit &right);
      const MonitorHit& operator=(const MonitorHit &right);
      int operator==(const MonitorHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      inline float x();
      inline float y();

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4int fNoID;
      G4double fTime;
      G4ThreeVector fWorldPos;
      G4ParticleDefinition* fPD;
      G4double fKinEnergy;
      G4ThreeVector fMomentumD;


  public:
      inline void SetLayerID(G4int z) { fNoID = z; }
      inline G4int GetLayerID() const { return fNoID; }

      inline void SetTime(G4double t) { fTime = t; }
      inline G4double GetTime() const { return fTime; }

      inline void SetWorldPos(G4ThreeVector xyz) { fWorldPos = xyz; }
      inline G4ThreeVector GetWorldPos() const { return fWorldPos; }

      inline void SetIncidenceDefinition (G4ParticleDefinition* pd) {fPD = pd;}
      inline G4ParticleDefinition* GetIncidenceDefinition() const {return fPD;}

      inline void SetIncidenceKinEnergy(G4double energy) { fKinEnergy = energy; }
      inline G4double GetIncidenceKinEnergy() const { return fKinEnergy; }

      inline void SetIncidenceMomentumDirection(G4ThreeVector mom) { fMomentumD = mom; }
      inline G4ThreeVector GetIncidenceMomentumDirection() const { return fMomentumD; }
};

typedef G4THitsCollection<MonitorHit> MonitorHitsCollection;

extern G4Allocator<MonitorHit> MonitorHitAllocator;

inline void* MonitorHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)MonitorHitAllocator.MallocSingle();
  return aHit;
}

inline void MonitorHit::operator delete(void* aHit)
{
  MonitorHitAllocator.FreeSingle((MonitorHit*) aHit);
}

#endif // MonitorHit_H
