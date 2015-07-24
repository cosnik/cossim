#ifndef __DETECOR_HIT_H__
#define __DETECOR_HIT_H__

#include <G4ThreeVector.hh>

/* -------------------------------------------------------------------------*/
//
// Class detectorHit to make a set of detector hits from a set of GEANT4 energy
// deposits
//
// A.P. Colijn - 24-07-2015
//
/* -------------------------------------------------------------------------*/

class detectorHit{
public:
    
    detectorHit();
    // add energy deposit
    void          addEnergyDeposit(G4ThreeVector e_pos, G4double e);
    // get the position of a hit - calculates the energy weighted mean
    G4ThreeVector getPosition();
    // get the total energy dposited in the hit
    G4double      getEnergy(){return e_sum;};
    // get the distance of the hit to an xyz coordinate
    G4double      getDistance(G4ThreeVector x);
    
private:
    // xyz_sum(i) = Sum xyz(i)*energy_deposit(i)
    G4ThreeVector xyz_sum;
    // e_sum      = Sum energy_deposit(i)
    G4double      e_sum;
};

#endif
