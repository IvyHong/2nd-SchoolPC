#ifndef CALORIMETERHIT_H
#define CALORIMETERHIT_H

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class CalorimeterHit : public G4VHit
{
public:

    // Constructor
    CalorimeterHit();
    CalorimeterHit(G4int id);
    // Destructor
    virtual ~CalorimeterHit();

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
};

#endif // CALORIMETERHIT_H
