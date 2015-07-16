#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4StackManager.hh"
#include "G4RunManagerKernel.hh"

#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PrimaryGeneratorAction::PrimaryGeneratorAction(G4String fName)
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0)
{
    fParticleGun = new G4GeneralParticleSource ();
    
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
    
    _cost_accept   = new TH1F("_cost_accept","_cost_accept",100,-1,1);
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
    
    G4StackManager *pStackManager = (G4RunManagerKernel::GetRunManagerKernel())->GetStackManager();
    
    
    //G4cout << "PrimaryGeneratorAction: track status: "
    //<< pStackManager->GetNUrgentTrack() << " urgent, "
    //<< pStackManager->GetNWaitingTrack() << " waiting, "
    //<< pStackManager->GetNPostponedTrack() << " postponed"
    //<< G4endl;

    //
    // If there are no postponed tracks we generate a new event.
    // In this way we take care of generating decays in a chain as separate events
    //
    if(!pStackManager->GetNPostponedTrack()){
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
    } else {
        
        // compose the postponed event.....
        
        pStackManager->TransferStackedTracks(fPostpone, fUrgent);
        G4VTrajectory* pTrajectory;
        G4Track *pTrack = pStackManager->PopNextTrack(&pTrajectory);
        
        G4double dPX = pTrack->GetMomentum().x();
        G4double dPY = pTrack->GetMomentum().y();
        G4double dPZ = pTrack->GetMomentum().z();
        
        G4PrimaryVertex *pVertex = new G4PrimaryVertex(pTrack->GetPosition(), 0.);
        
        G4PrimaryParticle *pPrimary = new G4PrimaryParticle(pTrack->GetDefinition(), dPX, dPY, dPZ);
        pPrimary->SetMass(pTrack->GetDefinition()->GetPDGMass());
        pPrimary->SetCharge(pTrack->GetDefinition()->GetPDGCharge());
        
        pVertex->SetPrimary(pPrimary);
        anEvent->AddPrimaryVertex(pVertex);
        
        // particle name of primary
        m_hParticleTypeOfPrimary = pTrack->GetParticleDefinition()->GetParticleName();
        // kinetic energy of primary
        m_dEnergyOfPrimary       = pTrack->GetKineticEnergy();
        // position of primary
        m_hPositionOfPrimary     = pTrack->GetPosition();
        // direction of primary
        m_hDirectionOfPrimary    = pTrack->GetMomentumDirection();

        delete pTrack;
    }

    
    // fill histograms
    _energy_accept->Fill(m_dEnergyOfPrimary);
    _cost_accept->Fill(m_hDirectionOfPrimary.z());
}

