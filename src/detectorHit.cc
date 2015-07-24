#include "detectorHit.hh"

using namespace CLHEP;


/*-------------------------------------------------------------------------*/
detectorHit::detectorHit(){
    
    xyz_sum.set(0.,0.,0.);
    e_sum = 0.0;
    
}

/*-------------------------------------------------------------------------*/
G4ThreeVector detectorHit::getPosition(){
    
    G4ThreeVector xyz = xyz_sum;
    if (e_sum>0){
        xyz /= e_sum;
    } else {
        xyz.set(0.,0.,0);
    }
    
    return xyz;
}

/*-------------------------------------------------------------------------*/
void detectorHit::addEnergyDeposit(G4ThreeVector e_pos, G4double e){
    
    xyz_sum += e_pos*e;
    e_sum += e;
    
}
/*-------------------------------------------------------------------------*/

G4double detectorHit::getDistance(G4ThreeVector x){
    
    G4ThreeVector xyz = getPosition();
    G4ThreeVector delta = xyz - x;
    
    return delta.mag();
    
}
/*-------------------------------------------------------------------------*/
