#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4StackManager.hh"
#include "G4RunManagerKernel.hh"

#include "PrimaryGeneratorAction.hh"

using namespace CLHEP;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PrimaryGeneratorAction::PrimaryGeneratorAction(G4String fName)
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0)
{
    //fParticleGun = new G4GeneralParticleSource ();
    fParticleGun = new G4ParticleGun ();
    
    // APC added
    m_hParticleTypeOfPrimary = "";
    m_dEnergyOfPrimary = 0.;
    m_hPositionOfPrimary = G4ThreeVector(0., 0., 0.);
    m_hDirectionOfPrimary = G4ThreeVector(0., 0., 0.);
    
    generatorRootFile = fName;
    // histograms.....
    _f_prim = new TFile(generatorRootFile,"RECREATE");
    
    _generator = _f_prim->mkdir("generator");
    
    _generator->cd();
    _log_primary_w = new TH1F("_log_primary_w","_log_primary_w",100,-20,0);
    _cost_all      = new TH1F("_cost_all","_cost_all",100,-1,1);
    _energy_all    = new TH1F("_energy_all","_energy_all",1000,0,5);
    
    _phi_accept    = new TH1F("_phi_accept","_phi_accept",100,-0.5,2*pi+0.5);
    _cost_accept   = new TH1F("_cost_accept","_cost_accept",100,-1.1,1.1);
    _energy_accept = new TH1F("_energy_accept","_energy_accept",1000,0,5);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    _f_prim->cd();
    _generator->cd();
    
    _f_prim->cd();
    
    _f_prim->Write();
    _f_prim->Close();
    
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //
    // generate a mu-
    //
    G4String particleName = "mu-";
    //
    //
    //
    G4double R = CLHEP::RandFlat::shoot();
    // azimuth
    G4double phi  = 2*pi*R;
    // polar angle
    R = CLHEP::RandFlat::shoot();
    G4double cost = pow(1-0.5*R,1./3);
//    G4double cost = (1.-2.*R);
    G4double sint = sqrt(1-cost*cost);
    // energy
    R = CLHEP::RandFlat::shoot();
    G4double energy = pow(1-R,-1./1.7)*GeV;

    // direction of our cosmic...
    G4double tx = sint*cos(phi);
    G4double ty = sint*sin(phi);
    G4double tz = -cost;
    //G4double tx = 0;
    //G4double ty = 0;
    //G4double tz = -1.0;

    // position of generation: z = 350mm, [x,y] = random tussen +-210mm
    R = CLHEP::RandFlat::shoot();
    G4double x = (2*R - 1)*225*mm;
    R = CLHEP::RandFlat::shoot();
    G4double y = (2*R - 1)*225*mm;

    G4double z = 325*mm;

    //
    // setup the event generation with the standard G4ParticleGun
    //
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(tx,ty,tz));

    // generate an event
    fParticleGun->GeneratePrimaryVertex(anEvent);
    // particle name of primary
    m_hParticleTypeOfPrimary = fParticleGun->GetParticleDefinition()->GetParticleName();
    // kinetic energy of primary
    m_dEnergyOfPrimary       = fParticleGun->GetParticleEnergy();
    // position of primary
    m_hPositionOfPrimary     = fParticleGun->GetParticlePosition();
    // direction of primary
    m_hDirectionOfPrimary    = fParticleGun->GetParticleMomentumDirection();
    
    // fill histograms
    _energy_accept->Fill(m_dEnergyOfPrimary);
    _phi_accept->Fill(phi);
    _cost_accept->Fill(cost);
}

