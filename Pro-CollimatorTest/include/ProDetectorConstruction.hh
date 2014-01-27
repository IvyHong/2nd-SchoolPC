#ifndef PRODETECTORCONSTRUCTION_HH
#define PRODETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class TSLCollimator;

class ProDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  // Constructor
   ProDetectorConstruction();
  // Destructor
  virtual ~ProDetectorConstruction();

  // Method
  virtual G4VPhysicalVolume* Construct();

  void DestroyMaterials();
  void DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth=0);

  inline G4double GetWorldFullLength () const {return fWorldLength;}
  inline G4double GetTargetLength() const {return targetLength;}

private:

   void               DefineMaterials();
   G4VPhysicalVolume* SetupGeometry();
   TSLCollimator*     CollimatorGeometry;

   G4double              fWorldLength;

   G4double       collimatorW;    ///< Horizontal distance perpendicular to the beam axis.
   G4double       collimatorL;    ///< Horizontal distance parallel to the beam axis.
   G4double       collimatorH;    ///< Vertical distance perpendicular to the beam axis.
   G4double       apertureDia;
   G4double          Distance;    ///< Horizontal distance from target to collimator front edge.
   G4double      targetLength;

   G4double          Detthick;
   G4int         NbOfDetector;
   G4double    collimatorYMov;

  // Logical volumes;
  //
   G4LogicalVolume*          WorldLog;
   G4LogicalVolume*         TargetLog;

   G4LogicalVolume*      ChamberSDLog;


   G4VPhysicalVolume*        WorldPhy;
   G4bool                    fCheckOverlap;

   G4Material*               Air;
   G4Material*               TargetMat;
   G4Material*               DefaultMat;
   G4Material*               CollimatorMat;

};

#endif // PRODETECTORCONSTRUCTION_HH
