#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Sphere.hh>
#include <G4Torus.hh>
#include <G4Orb.hh>
#include <G4Polyhedra.hh>
#include <G4Polycone.hh>
#include <G4Ellipsoid.hh>
#include <G4ExtrudedSolid.hh>  // SERENA
//#include <G4Trd.hh>
#include <G4UnionSolid.hh>
//#include <G4IntersectionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <globals.hh>

#include <vector>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cassert>

using std::vector;
using std::stringstream;
using std::max;

#include "DetectorMessenger.hh"

#include "SensitiveDetector.hh"
#include "DetectorConstruction.hh"

#include "G4PhysicalVolumeStore.hh"

#include "TFile.h"
#include "TParameter.h"

using namespace CLHEP;
/*----------------------------------------------------------------------------------------------*/

// define geometry parameters here - there are better ways but this will do for now

// iron tube
const G4double dTubeInnerRadius = 100*mm;
const G4double dTubeOuterRadius = 101*mm;
const G4double dTubeLength      = 500*mm;

// silicon sensors

const G4double dSensorLength    = 400*mm;
const G4double dSensorWidth     = 400*mm;
const G4double dSensorThickness = 0.3*mm;

const G4int NumberOfSensors = 4;
const G4double Xposition[NumberOfSensors] = {0.,0.,0.,0.};
const G4double Yposition[NumberOfSensors] = {0.,0.,0.,0.};
const G4double Zposition[NumberOfSensors] = {-320*mm,-250*mm,+250*mm,320*mm};

/*----------------------------------------------------------------------------------------------*/



map<G4String, G4double> DetectorConstruction::m_hGeometryParameters;

DetectorConstruction::DetectorConstruction(G4String fName)
{
    m_pDetectorMessenger = new DetectorMessenger(this);
    detRootFile = fName;
}

DetectorConstruction::~DetectorConstruction()
{
    delete m_pDetectorMessenger;
}


G4VPhysicalVolume*
DetectorConstruction::Construct()
{
    
    DefineMaterials();
    
    DefineGeometryParameters();
    
    // construction of the geometry
    ConstructLaboratory();
    // Iron tube + silicon detectors
    ConstructCosmicSetup();
    // make the appropriate components active
    ConstructSD();
    
    PrintGeometryInformation();
    
    MakeDetectorPlots();
    
    return m_pLabPhysicalVolume;
}

void
DetectorConstruction::DefineMaterials()
{
//    G4NistManager* pNistManager = G4NistManager::Instance();
    
    //================================== elements ===================================
//    pNistManager->FindOrBuildElement("U");
//    G4Element *Xe = new G4Element("Xenon",     "Xe", 54., 131.293*g/mole);
//    G4Element *H  = new G4Element("Hydrogen",  "H",  1.,  1.0079*g/mole);
//    G4Element *C  = new G4Element("Carbon",    "C",  6.,  12.011*g/mole);
//    G4Element *N  = new G4Element("Nitrogen",  "N",  7.,  14.007*g/mole);
//    G4Element *O  = new G4Element("Oxygen",    "O",  8.,  15.999*g/mole);
//    G4Element *F  = new G4Element("Fluorine",  "F",  9.,  18.998*g/mole);
    //	G4Element *Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
//    G4Element *Si = new G4Element("Silicon",   "Si", 14., 28.086*g/mole);
//    G4Element *Cr = new G4Element("Chromium",  "Cr", 24., 51.996*g/mole);
//    G4Element *Mn = new G4Element("Manganese", "Mn", 25., 54.938*g/mole);
//    G4Element *Fe = new G4Element("Iron",      "Fe", 26., 55.85*g/mole);
//    G4Element *Ni = new G4Element("Nickel",    "Ni", 28., 58.693*g/mole);
    
//    G4Element *Na = pNistManager->FindOrBuildElement("Na");
//    G4Element *I  = pNistManager->FindOrBuildElement("I");
    //================================== materials ==================================
    
    //------------------------------------- air -------------------------------------
    //G4Material *Air = pNistManager->FindOrBuildMaterial("G4_AIR");
    //G4Material *Air = G4Material::GetMaterial("G4_AIR");
    
    
    //----------------------------------- vacuum ------------------------------------
//    G4Material *Vacuum = new G4Material("Vacuum", 1.e-20*g/cm3, 2, kStateGas);
//    Vacuum->AddElement(N, 0.755);
//    Vacuum->AddElement(O, 0.245);
    
    //------------------------------------ water ------------------------
//    G4Material *Water = new G4Material("Water", 1.*g/cm3, 2, kStateLiquid);
//    Water->AddElement(H, 2);
//    Water->AddElement(O, 1);
    
    //------------------------------------ NaI   ------------------------
//    G4Material *NaI = new G4Material("NaI", 3.67*g/cm3, 2, kStateSolid);
//    NaI->AddElement(Na, 1);
//    NaI->AddElement( I, 1);
    
    //------------------------------------ plastic -----------------------------------
//    G4Material* PE = new G4Material("PE", 1.0*g/cm3, 2, kStateSolid);
//    PE->AddElement(C, 2);
//    PE->AddElement(H, 4);
    //------------------------------------ lead --------------------------------------
//    G4Material *Pb = pNistManager->FindOrBuildMaterial("G4_Pb");
    //------------------------------- stainless steel -------------------------------
//    G4Material *SS304LSteel = new G4Material("SS304LSteel", 8.00*g/cm3, 5, kStateSolid);
//    SS304LSteel->AddElement(Fe, 0.65);
//    SS304LSteel->AddElement(Cr, 0.20);
//    SS304LSteel->AddElement(Ni, 0.12);
//    SS304LSteel->AddElement(Mn, 0.02);
//    SS304LSteel->AddElement(Si, 0.01);
    //------------------------------------ teflon -----------------------------------
//    G4Material* Teflon = new G4Material("Teflon", 2.2*g/cm3, 2, kStateSolid);
//    Teflon->AddElement(C, 0.240183);
//    Teflon->AddElement(F, 0.759817);
    //------------------------------------ acrylic -----------------------------------
//    G4Material *Acrylic = new G4Material("Acrylic", 1.18*g/cm3, 3, kStateSolid, 168.15*kelvin, 1.5*atmosphere);
//    Acrylic->AddElement(C,5);
//    Acrylic->AddElement(H,8);
//    Acrylic->AddElement(O,2);
    
}

void
DetectorConstruction::DefineGeometryParameters()
{
    //================================== Laboratory =================================
    m_hGeometryParameters["LabSize"] = 1.0*m;

}

G4double
DetectorConstruction::GetGeometryParameter(const char *szParameter)
{
    return m_hGeometryParameters[szParameter];
}

void
DetectorConstruction::ConstructLaboratory()
{
    //================================== Laboratory =================================
    const G4double dLabHalfSize = 0.5*GetGeometryParameter("LabSize");
    
    G4NistManager* pNistManager = G4NistManager::Instance();

    G4Material *Air = pNistManager->FindOrBuildMaterial("G4_AIR");
    
    G4Box *pLabBox         = new G4Box("LabBox", dLabHalfSize, dLabHalfSize, dLabHalfSize);
    m_pLabLogicalVolume    = new G4LogicalVolume(pLabBox, Air, "LabVolume", 0, 0, 0);
    m_pLabPhysicalVolume   = new G4PVPlacement(0, G4ThreeVector(), m_pLabLogicalVolume, "Lab", 0, false, 0);
    m_pMotherLogicalVolume = m_pLabLogicalVolume;
    
    m_pLabLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);

}

void
DetectorConstruction::ConstructSD()
{
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    // make NaI sensitive
    char sdname[128];
    for(G4int isensor = 0; isensor<NumberOfSensors; isensor++){
      sprintf(sdname,"/Silicon%i",isensor);
      G4VSensitiveDetector* Silicon_SD = new SensitiveDetector(SDname=sdname);
      SDman->AddNewDetector(Silicon_SD);
      m_Sensor_LogicalVolume[isensor]->SetSensitiveDetector(Silicon_SD);
    }

}


void
DetectorConstruction::ConstructCosmicSetup()
{

    //================================== Materials ===================================
    G4NistManager* pNistManager = G4NistManager::Instance();
    G4Material *Si = pNistManager->FindOrBuildMaterial("G4_Si");
    G4Material *Fe = pNistManager->FindOrBuildMaterial("G4_Fe");

    
    //================================== Rotation matrices ============================
    G4RotationMatrix *pRot = new G4RotationMatrix();
    pRot->rotateY(90.*deg);
    
    //================================== Construction =================================

    //
    // Iron tube
    //
    
    //
    // G4Tubs("name",R_in,R_out,Length/2,phi_min,phi_max)
    //
//    G4Tubs *p_IronTube			= new G4Tubs("IronTube", dTubeInnerRadius, dTubeOuterRadius, dTubeLength/2., -90.*deg, 180.*deg);
    G4Tubs *p_IronTube			= new G4Tubs("IronTube", dTubeInnerRadius, dTubeOuterRadius, dTubeLength/2., 0.*deg, 360.*deg);
    //
    // Define the logical volume
    //
    m_IronTube_LogicalVolume	= new G4LogicalVolume(p_IronTube, Fe, "Fe_TUBS", 0, 0, 0);
    //
    // Place the logcal volume in the physical world
    //
    m_IronTube_PhysicalVolume 	= new G4PVPlacement(pRot, G4ThreeVector(0,0,0), m_IronTube_LogicalVolume, "IronTube", m_pMotherLogicalVolume, false, 0);
    
    //
    // Silicon telescope
    //
    
    //
    // define one silicon sensor
    //
    G4Box *p_Sensor = new G4Box("Sensor",dSensorLength/2.,dSensorWidth/2.,dSensorThickness/2.);
    
    //
    // construct the telescope
    //
    char physName[128], logName[128];
    for(G4int isensor = 0; isensor<NumberOfSensors; isensor++){
        sprintf(logName,"Si_BOX");
        m_Sensor_LogicalVolume.push_back(new G4LogicalVolume(p_Sensor, Si, logName, 0, 0, 0));
        sprintf(physName,"SiSensor%i",isensor);
        m_Sensor_PhysicalVolume.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,Zposition[isensor]),m_Sensor_LogicalVolume[isensor],physName, m_pMotherLogicalVolume, false,0));
    }
    

    //
    // visibility
    //
    G4Colour hTitaniumColor(0.600, 0.600, 0.600, 0.4);
    G4VisAttributes *pTitaniumVisAtt = new G4VisAttributes(hTitaniumColor);
    pTitaniumVisAtt->SetVisibility(true);
    m_IronTube_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);

    G4cout <<"DetectorConstruction:: end sysdef"<<G4endl;
}


void
DetectorConstruction::PrintGeometryInformation()
{
    //================================== Water =================================
    //	const G4double dWaterMass = m_pWaterLogicalVolume->GetMass(false, false)/(1000.*kg);
    //	G4cout << "Water Mass:                               " << dWaterMass << " ton" << G4endl << G4endl;
}

void DetectorConstruction::MakeDetectorPlots()
{
    _fGeom = new TFile(detRootFile,"RECREATE");
    _detector = _fGeom->mkdir("detector");
    
    // store properties of the materials that are used
    StoreMaterialParameters();
    // store geometry parameters
    StoreGeometryParameters();
    
    _fGeom->Write();
    _fGeom->Close();
    
}

void DetectorConstruction::StoreGeometryParameters()
{
    // TDirectory for storage of teh geometry parameters
    TDirectory *_geometry = _detector->mkdir("geometry");
    _geometry->cd();
    

    
    _fGeom->cd();
}


void DetectorConstruction::StoreMaterialParameters()
{
    
    // make a list of materials for graphs
    G4int nmaterial = G4Material::GetNumberOfMaterials();
    G4cout <<"MakeDetectorPlots:: Number of materials = "<<nmaterial<<G4endl;
    
    TDirectory *_materials = _detector->mkdir("materials");
    _materials->cd();
    
    //  for(G4int imat=0; imat<(G4int)matNames.size(); imat++){
    vector<TDirectory*> matdirs;
    
    for(G4int imat=0; imat<nmaterial; imat++){
        G4Material *mat = G4NistManager::Instance()->GetMaterial(imat);
        G4String matname = mat->GetName();
        G4cout <<"MakeDetectorPlots:: "<<matname<<G4endl;
        G4double T   = mat->GetTemperature();
        G4double rho = mat->GetDensity();
        G4double P   = mat->GetPressure();
        
        matdirs.push_back(_materials->mkdir(matname));
        matdirs[imat]->cd();
        TParameter<double> *TemperaturePar = new TParameter<double>("Temperature",T);
        TemperaturePar->Write();
        TParameter<double> *DensityPar     = new TParameter<double>("Density",rho / (g/cm3));
        DensityPar->Write();
        TParameter<double> *PressurePar    = new TParameter<double>("Pressure",P / bar);
        PressurePar->Write();
        // disect the material
        size_t nele = mat->GetNumberOfElements();
        G4ElementVector *elems       = (G4ElementVector*)mat->GetElementVector();
        G4double        *fractionVec = (G4double*)mat->GetFractionVector();
        
        for(size_t iele=0; iele<nele; iele++){
            G4String elname = (*elems)[iele]->GetName();
            G4double frac   = fractionVec[iele];
            //      G4cout <<iele<<" elem = "<<(*elems)[iele]->GetName()<<" f = "<<fractionVec[iele]<<G4endl;
            char  elFrac[100];
            sprintf(elFrac,"f_%s",elname.c_str());
            TParameter<double> *_fracPar = new TParameter<double>((G4String)elFrac,frac);
            _fracPar->Write();
        }
        
        
        _materials->cd();
    }
    
    _fGeom->cd();
}
