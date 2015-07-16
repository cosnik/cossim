#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "stdHit.hh"

using namespace CLHEP;

G4Allocator<stdHit> stdHitAllocator;

stdHit::stdHit() {}

stdHit::~stdHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pParentType) delete m_pParentType;
	if(m_pCreatorProcess) delete m_pCreatorProcess;
	if(m_pDepositingProcess) delete m_pDepositingProcess; 
}

stdHit::stdHit(const stdHit &hHit):G4VHit()
{
	m_iTrackId = hHit.m_iTrackId;
	m_iParentId = hHit.m_iParentId;
	m_pParticleType = hHit.m_pParticleType;
	m_pParentType = hHit.m_pParentType ;
	m_pCreatorProcess = hHit.m_pCreatorProcess ;
	m_pDepositingProcess = hHit.m_pDepositingProcess ;
	m_hPosition = hHit.m_hPosition;
	m_dEnergyDeposited = hHit.m_dEnergyDeposited;
	m_dKineticEnergy = hHit.m_dKineticEnergy ;
	m_dTime = hHit.m_dTime;
}

const stdHit &
stdHit::operator=(const stdHit &hHit)
{
	m_iTrackId = hHit.m_iTrackId;
	m_iParentId = hHit.m_iParentId;
	m_pParticleType = hHit.m_pParticleType;
	m_pParentType = hHit.m_pParentType ;
	m_pCreatorProcess = hHit.m_pCreatorProcess ;
	m_pDepositingProcess = hHit.m_pDepositingProcess ;
	m_hPosition = hHit.m_hPosition;
	m_dEnergyDeposited = hHit.m_dEnergyDeposited;
	m_dKineticEnergy = hHit.m_dKineticEnergy ;
	m_dTime = hHit.m_dTime;
	
	return *this;
}

G4int
stdHit::operator==(const stdHit &hHit) const
{
	return ((this == &hHit) ? (1) : (0));
}

void stdHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	
	if(pVVisManager)
	{
		G4Circle hCircle(m_hPosition);
		G4Colour hColour(1.000, 0.973, 0.184);
		G4VisAttributes hVisAttributes(hColour);
		
		hCircle.SetScreenSize(0.1);
		hCircle.SetFillStyle(G4Circle::filled);
		hCircle.SetVisAttributes(hVisAttributes);
		pVVisManager->Draw(hCircle);
	}
}

void stdHit::Print()
{
	G4cout << "-------------------- LXe hit --------------------" 
		<< "Id: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " ParentId: " << m_iParentId
		<< " ParentType: " << *m_pParentType << G4endl
		<< "CreatorProcess: " << *m_pCreatorProcess
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "Position: " << m_hPosition.x()/mm
		<< " " << m_hPosition.y()/mm
		<< " " << m_hPosition.z()/mm
		<< " mm" << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/keV << " keV"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}

