#ifndef TSLCOLLIMATOR_H
#define TSLCOLLIMATOR_H

#include "globals.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "G4Material.hh"

#include "G4LogicalVolume.hh"

const G4ThreeVector fApertureMov(0.,296*mm,0.); // Aperture Moving towards + Y direction

class TSLCollimator
{
public:

    // Default constructor
    TSLCollimator();
    // Constructor with parameters
    TSLCollimator(G4double ApertureDiameter,
                  G4double CollimatorWidth,
                  G4double CollimatorHeight,
                  G4double CollimatorLength,
                  G4Material * CollimatorMat,
                  G4VPhysicalVolume * MotherPV,
                  G4ThreeVector CollimatorPos,
                  G4RotationMatrix* Rotation);

    ~TSLCollimator();

// static TSLCollimator* GetInstance(void); ////@not sure about what is this used for?

    // Get Method
    inline G4VPhysicalVolume* GetPhysicalVolume (){ return fCollimatorPhy;}
    inline G4double GetApertureDiameter () { return fApertureDiameter; }
    inline G4Material* GetCollimatorMaterial () { return fCollimatorMat; }

    inline G4double GetCollimatorWidth () const { return fCollimatorWidth;}
    inline G4double GetCollimatorHeight () const { return fCollimatorHeight;}
    inline G4double GetCollimatorLength () const { return fCollimatorLength;}

private:

    void TSLCollimatorSetup (G4VPhysicalVolume* );

    G4double fApertureDiameter; // Aperture Diameter
    G4double fCollimatorWidth;  // Length along X axes, perpendicular to direction of incident particle
    G4double fCollimatorHeight; // Length along Y axes, perpendicular to direction of incident particle
    G4double fCollimatorLength; // Length along Z axes, parallel to direction of incident particle

    G4ThreeVector fCollimatorPos;     // Collimator position in the mother volume
    G4RotationMatrix* fCollimatorRot; // Collimator rotation in the mother volume

    G4LogicalVolume * fCollimatorLog;

    G4Material * fCollimatorMat;      // Collimator Material
    G4VPhysicalVolume* fCollimatorPhy;
};



#endif // TSLCOLLIMATOR_H
