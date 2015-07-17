#ifndef __DETECTORCONSTRUCTION_H__
#define __DETECTORCONSTRUCTION_H__

#include <globals.hh>

#include <vector>
#include <map>
#include "TFile.h"
#include "TDirectory.h"

using std::vector;
using std::map;

class G4Sphere;
class G4Colour;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorMessenger;
class G4SubtractionSolid;

#include <G4UnionSolid.hh>
#include <G4UImanager.hh>

#include <G4VUserDetectorConstruction.hh>

class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(G4String fName);
    ~DetectorConstruction();
    
    G4VPhysicalVolume* Construct();
    static G4double GetGeometryParameter(const char *szParameter);
    // function needed by DetectorMessenger
//    void SetSourcePos(G4double x)    { m_hSourcePosition = x; }
//    void SetNaIPos(G4double x)    { m_hNaIPosition = x; }

    
    
private:
    void DefineMaterials();
    void DefineGeometryParameters();
    
    void ConstructLaboratory();
    void ConstructCosmicSetup();
    // define the sensitive detectors
    void ConstructSD();
    void PrintGeometryInformation();
    
    void MakeDetectorPlots();
    void StoreMaterialParameters();
    void StoreGeometryParameters();
    
    
    // == LOGICAL VOLUMES ==
    G4LogicalVolume *m_pMotherLogicalVolume;
    G4LogicalVolume *m_pLabLogicalVolume;
    
    // == Iron TUBE
    G4LogicalVolume *m_IronTube_LogicalVolume;
    vector<G4LogicalVolume*> m_Sensor_LogicalVolume;

    // == PHYSICAL VOLUMES ==
    G4VPhysicalVolume *m_pLabPhysicalVolume;
    G4VPhysicalVolume *m_IronTube_PhysicalVolume;
    vector<G4VPhysicalVolume*> m_Sensor_PhysicalVolume;
    
    
    static map<G4String, G4double> m_hGeometryParameters;
    
    DetectorMessenger *m_pDetectorMessenger;
    
    // ROOT stuff
    TFile      *_fGeom;
    G4String    detRootFile;
    TDirectory *_detector;
    
};

#endif // __DETECTORCONSTRUCTION_H__

