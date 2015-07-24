#include <G4SDManager.hh>
#include <G4Run.hh>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4EmCalculator.hh>
#include <G4Material.hh>
#include <G4HadronicProcessStore.hh>
#include <G4ParticleTable.hh>
#include <G4NistManager.hh>
#include <G4ElementTable.hh>
#include <G4Version.hh>
#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TParameter.h>
#include <TDirectory.h>
#include <TH1.h>

#include "stdHit.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventData.hh"

#include "AnalysisMessenger.hh"
#include "AnalysisManager.hh"

#include "detectorHit.hh"

#define MAX_HIT_DISTANCE 5.0

using namespace CLHEP;

AnalysisManager::AnalysisManager(PrimaryGeneratorAction *pPrimaryGeneratorAction)
{
    m_pAnalysisMessenger = new AnalysisMessenger(this);
    m_hTreeType = "raw";
    
    runTime = new G4Timer();
    m_CollectionIDs.clear();
    
    m_SiCollectionID = -1;
    m_hDataFilename = "events.root";
    
    m_pPrimaryGeneratorAction = pPrimaryGeneratorAction;
    
    m_pEventData = new EventData();
    //plotPhysics      = kTRUE;
    // writeEmptyEvents = kTRUE;
}

AnalysisManager::~AnalysisManager()
{
    delete m_pAnalysisMessenger;
}

void
AnalysisManager::BeginOfRun(const G4Run *)
{
    // start a timer for this run....
    runTime->Start();
    // do we write empty events or not?
    // writeEmptyEvents = kTRUE;
    
    m_pTreeFile = new TFile(m_hDataFilename.c_str(), "RECREATE");
    // make tree structure
    TNamed *G4version = new TNamed("G4VERSION_TAG",G4VERSION_TAG);
    G4version->Write();
    
    _events = m_pTreeFile->mkdir("events");
    _events->cd();
    
    G4cout <<"AnalysisManager:: Init data tree ..."<<G4endl;
    m_pTree = new TTree("evt", "Event Data");
    
    gROOT->ProcessLine("#include <vector>");
    
    m_pTree->Branch("eventid", &m_pEventData->m_iEventId, "eventid/I");
    
    m_pTree->Branch("etot", &m_pEventData->m_fTotalEnergyDeposited, "etot/F");
    m_pTree->Branch("nsteps", &m_pEventData->m_iNbSteps, "nsteps/I");
    m_pTree->Branch("type_pri", "vector<string>", &m_pEventData->m_pPrimaryParticleType);
    m_pTree->Branch("xp_pri", &m_pEventData->m_fPrimaryX, "xp_pri/F");
    m_pTree->Branch("yp_pri", &m_pEventData->m_fPrimaryY, "yp_pri/F");
    m_pTree->Branch("zp_pri", &m_pEventData->m_fPrimaryZ, "zp_pri/F");
    m_pTree->Branch("e_pri",  &m_pEventData->m_fPrimaryE, "e_pri/F");
    m_pTree->Branch("w_pri",  &m_pEventData->m_fPrimaryW, "w_pri/F");
    
    if(m_hTreeType == "raw"){ // all hits
        m_pTree->Branch("trackid", "vector<int>", &m_pEventData->m_pTrackId);
        m_pTree->Branch("type", "vector<string>", &m_pEventData->m_pParticleType);
        m_pTree->Branch("parentid", "vector<int>", &m_pEventData->m_pParentId);
        m_pTree->Branch("collid", "vector<int>", &m_pEventData->m_pCollectionId);
        m_pTree->Branch("parenttype", "vector<string>", &m_pEventData->m_pParentType);
        m_pTree->Branch("creaproc", "vector<string>", &m_pEventData->m_pCreatorProcess);
        m_pTree->Branch("edproc", "vector<string>", &m_pEventData->m_pDepositingProcess);
        m_pTree->Branch("xp", "vector<float>", &m_pEventData->m_pX);
        m_pTree->Branch("yp", "vector<float>", &m_pEventData->m_pY);
        m_pTree->Branch("zp", "vector<float>", &m_pEventData->m_pZ);
        m_pTree->Branch("ed", "vector<float>", &m_pEventData->m_pEnergyDeposited);
        m_pTree->Branch("time", "vector<float>", &m_pEventData->m_pTime);
    } else if (m_hTreeType == "compact"){ // only average per detector
        m_pTree->Branch("collid", "vector<int>", &m_pEventData->m_pCollectionId);
        m_pTree->Branch("xp", "vector<float>", &m_pEventData->m_pX);
        m_pTree->Branch("yp", "vector<float>", &m_pEventData->m_pY);
        m_pTree->Branch("zp", "vector<float>", &m_pEventData->m_pZ);
        m_pTree->Branch("ed", "vector<float>", &m_pEventData->m_pEnergyDeposited);
        m_pTree->Branch("time", "vector<float>", &m_pEventData->m_pTime);
    }
    
    
    m_pNbEventsToSimulateParameter = new TParameter<int>("nbevents", m_iNbEventsToSimulate);
    m_pNbEventsToSimulateParameter->Write();

    m_pTreeFile->cd();
}

void
AnalysisManager::EndOfRun(const G4Run *)
{
    runTime->Stop();
    G4double dt = runTime->GetRealElapsed();
    // make tree structure
    TParameter<G4double> *dtPar = new TParameter<G4double>("G4RUNTIME",dt);
    dtPar->Write();
    
    m_pTreeFile->cd();
    
    m_pTreeFile->Write();
    m_pTreeFile->Close();
}

void
AnalysisManager::BeginOfEvent(const G4Event *)
{
    // only d this if teh collection has not yet been defined yet
    char idname[128];
    if(m_SiCollectionID == -1)
    {
        for(G4int idet=0; idet<4; idet++){
           sprintf(idname,"Silicon%i/HitsCollection",idet);
           G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
           m_SiCollectionID = pSDManager->GetCollectionID(idname);
           m_CollectionIDs.push_back(m_SiCollectionID);
        }
        G4cout << "AnalysisManager::BeginOfEvent  Found Silicon collection at ID = " << m_SiCollectionID <<G4endl;
    }
    
}

void
AnalysisManager::EndOfEvent(const G4Event *pEvent)
{
    m_pEventData->Clear();
    
    _events->cd();
    
    G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
    
    // get the event ID and primary particle information
    m_pEventData->m_iEventId = pEvent->GetEventID();
    m_pEventData->m_pPrimaryParticleType->push_back(m_pPrimaryGeneratorAction->GetParticleTypeOfPrimary());
    
    m_pEventData->m_fPrimaryX = m_pPrimaryGeneratorAction->GetPositionOfPrimary().x();
    m_pEventData->m_fPrimaryY = m_pPrimaryGeneratorAction->GetPositionOfPrimary().y();
    m_pEventData->m_fPrimaryZ = m_pPrimaryGeneratorAction->GetPositionOfPrimary().z();
    m_pEventData->m_fPrimaryE = m_pPrimaryGeneratorAction->GetEnergyOfPrimary() / keV;
    m_pEventData->m_fPrimaryW = pEvent->GetPrimaryVertex()->GetWeight();
    
    
    // unpack the hit collections
    G4int    iNbHits = 0;
    G4int    iNbSteps = 0;
    G4double fTotalEnergyDeposited = 0;
    
    if(pHCofThisEvent) {
        //
        // loop over all our hit collections
        //
        for(G4int icol=0; icol<(G4int)m_CollectionIDs.size(); icol++){
            // check if the ID of the collection is OK
            if(m_CollectionIDs[icol] != -1){
                stdHitsCollection* pHitsCollection = 0;
                // get the hits
                pHitsCollection = static_cast<stdHitsCollection*>(pHCofThisEvent->GetHC(m_CollectionIDs[icol]));
                // the number of hits
                iNbHits = (pHitsCollection)?(pHitsCollection->entries()):(0);
                //G4cout << icol << " Nb hits = " << iNbHits<< " ID = "<<m_CollectionIDs[icol]<<G4endl;
                
                if(iNbHits) {
                    // hits
                    if (m_hTreeType == "raw"){
                        // write all the GEANT4 steps and hits to the output tree
                        for(G4int i=0; i<iNbHits; i++) {
                            stdHit *pHit = (*pHitsCollection)[i];
                            if(pHit->GetParticleType() != "opticalphoton"){
                                m_pEventData->m_pTrackId->push_back(pHit->GetTrackId());
                                m_pEventData->m_pParentId->push_back(pHit->GetParentId());
                                m_pEventData->m_pCollectionId->push_back(icol);
                                
                                m_pEventData->m_pParticleType->push_back(pHit->GetParticleType());
                                m_pEventData->m_pParentType->push_back(pHit->GetParentType());
                                m_pEventData->m_pCreatorProcess->push_back(pHit->GetCreatorProcess());
                                m_pEventData->m_pDepositingProcess->push_back(pHit->GetDepositingProcess());
                                
                                m_pEventData->m_pX->push_back(pHit->GetPosition().x()/mm);
                                m_pEventData->m_pY->push_back(pHit->GetPosition().y()/mm);
                                m_pEventData->m_pZ->push_back(pHit->GetPosition().z()/mm);
                                
                                fTotalEnergyDeposited += pHit->GetEnergyDeposited()/keV;
                                m_pEventData->m_pEnergyDeposited->push_back(pHit->GetEnergyDeposited()/keV);
                                
                                m_pEventData->m_pKineticEnergy->push_back(pHit->GetKineticEnergy()/keV);
                                m_pEventData->m_pTime->push_back(pHit->GetTime()/second);
                                
                                iNbSteps++;
                            } // if !opticalphoton
                        }
                    } else if (m_hTreeType == "compact"){
                        //
                        // define a vector of detectorHit
                        //
                        vector<detectorHit> detHits;
                        
                        //
                        // loop over the GEANT4 energy deposits
                        //
                        for(G4int i=0; i<iNbHits; i++) {
                            stdHit *pHit = (*pHitsCollection)[i];
                            G4ThreeVector xh = pHit->GetPosition()/mm;
                            G4double      ed = pHit->GetEnergyDeposited()/keV;
                            bool added_hit = false;
                            for(G4int ih=0; ih<(G4int)detHits.size(); ih++){
                                //
                                // if the energy deposit is close to the hit add it
                                //
                                if (detHits.at(ih).getDistance(xh) < MAX_HIT_DISTANCE*mm && !added_hit) {
                                    detHits.at(ih).addEnergyDeposit(xh,ed);
                                    added_hit = true;
                                }
                            }
                            
                            //
                            // if this hit is not added to an existing detectorHit: make a detectorHit
                            //
                            if(!added_hit) detHits.push_back(detectorHit(xh,ed));
                            
                        }
                        
                        for (G4int ih=0; ih<(G4int)detHits.size(); ih++){
                            //
                            // fill the tree variables
                            //
                            
                            detectorHit detHit = detHits.at(ih);
                            
                            m_pEventData->m_pCollectionId->push_back(icol);
                            m_pEventData->m_pEnergyDeposited->push_back(detHit.getEnergy());
                            m_pEventData->m_pX->push_back(detHit.getPosition().x());
                            m_pEventData->m_pY->push_back(detHit.getPosition().y());
                            m_pEventData->m_pZ->push_back(detHit.getPosition().z());
                            m_pEventData->m_pTime->push_back(-1);

                            fTotalEnergyDeposited += detHit.getEnergy();
                        }
                    } else {
                        G4cout <<"AnalysisManager::EndOfEvent ERROR: wrong Tree type selected"<<G4endl;
                        return;
                    } // if m_hTreeType
                } // if Hits
            }
        }
    }
    
    //G4cout << " size = " << m_pEventData->m_pX->size() <<G4endl;
    // also write the header information + primary vertex of the empty events....
    m_pEventData->m_iNbSteps = iNbSteps;
    m_pEventData->m_fTotalEnergyDeposited = fTotalEnergyDeposited;
    
    // save only energy depositing events
    m_pTree->Fill(); // write all events to the tree
    
    m_pEventData->Clear();
    m_pTreeFile->cd();
    
    //  delete pHitsCollection;
}

void
AnalysisManager::Step(const G4Step *)
{
    
}
