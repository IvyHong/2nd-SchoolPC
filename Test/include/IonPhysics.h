#ifndef IONPHYSICS_H
#define IONPHYSICS_H

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

#include "G4HadronElasticProcess.hh"
#include "G4LElastic.hh"

#include "G4DeuteronInelasticProcess.hh"
#include "G4LEDeuteronInelastic.hh"

#include "G4TritonInelasticProcess.hh"
#include "G4LETritonInelastic.hh"

#include "G4AlphaInelasticProcess.hh"
#include "G4LEAlphaInelastic.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4hMultipleScattering.hh"

class IonPhysics : public G4VPhysicsConstructor
{
  public:
    IonPhysics(const G4String& name="ion");
    virtual ~IonPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle(){;}

    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();
};

#endif // IONPHYSICS_H
