#include <G4ios.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4Track.hh>
#include <G4Event.hh>
#include <G4VProcess.hh>
#include <G4StackManager.hh>

#include "AnalysisManager.hh"

#include "StackingAction.hh"

StackingAction::StackingAction(AnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track *pTrack)
{

    //
    // This function classifies tracks: if there is a long lived daughter in a decay chain, we should take care that
    // we make a separate event from it. By default GEANT4 simulates the full decays chain in one go.
    //
    // The tracks classification is then to be checked within the PrimaryGeneratorAction class when an event is generated
    //
    
    G4ClassificationOfNewTrack hTrackClassification = fUrgent;

    //G4cout << G4endl;
    //G4cout << "NAME     = " << pTrack->GetParticleDefinition()->GetParticleName()<<G4endl;
    //G4cout << "TYPE     = " << pTrack->GetDefinition()->GetParticleType()<<G4endl;
    //G4cout << "LIFETIME = " << pTrack->GetParticleDefinition()->GetPDGLifeTime()/CLHEP::second<<G4endl;
    //G4cout << "STABLE   = " << pTrack->GetDefinition()->GetPDGStable()<<G4endl;
    if(pTrack->GetDefinition()->GetParticleType() == "nucleus" && !pTrack->GetDefinition()->GetPDGStable())
	{
        if(pTrack->GetParentID() > 0 && pTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay"){
//            G4cout << "NAME     = " << pTrack->GetParticleDefinition()->GetParticleName()<<G4endl;
//            G4cout << "LIFETIME = " << pTrack->GetParticleDefinition()->GetPDGLifeTime()/CLHEP::second<<G4endl;
     
			hTrackClassification = fPostpone;
        }
    }

	return hTrackClassification;
}

void
StackingAction::NewStage()
{
}

void
StackingAction::PrepareNewEvent()
{ 
}








