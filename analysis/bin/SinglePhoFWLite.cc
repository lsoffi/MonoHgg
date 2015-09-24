#include <iostream>

#include <TTree.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "flashgg/DataFormats/interface/Photon.h"

using namespace std;

// per event tree
struct eventTree_struc_ {

  int nPho;
  int nMatchedPho;
};


// per photon tree
struct phoTree_struc_ {

  float pt;
  float eta;
  float phi;
  int isEBEtaGap;
  int isEBPhiGap;
  int isEERingGap;
  int isEEDeeGap;
  int isEBEEGap;
  int iPhi;
  int iEta;

  float scEta;
  float scPhi;

  float eMax;
  float e5x5;
  float energy;
  // float energyInitial;
  // float energyRegression;

  float e1x5;
  float e2x5;
  float sigmaIetaIeta;
  float r9;

  float hoe;
  float h1oe;
  float h2oe;
  float htoe;
  float ht1oe;
  float ht2oe;

  bool passEleVeto;
  bool hasPixelSeed;

  float trackIso;
  float ecalIso;
  float hcalIso;
  float chHadIso;
  float nHadIso;
  float photonIso;
  float rho;

  float trueEnergy;
  float truePt;
  float trueEta;
  float truePhi;
  float minDR;

  // da DataFormats/EgammaCandidates/interface/Photon.h, ma da capire come spacchettare
  // const EnergyCorrections & energyCorrections() const { return eCorrections_ ; }
};


eventTree_struc_ treeEv_;
phoTree_struc_ tree_;


int main(int argc, char* argv[]) {

  // ----------------------------------------------
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // parse arguments
  if ( argc < 2 ) {
    cout << "Usage : SinglePhoFWLite " << argv[0] << " [parameters.py]" << endl;
    return 0;
  }
  if( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ){
    cout << " ERROR: ParametersSet 'process' is missing in your configuration file" << endl; exit(0);
  }
                                                                                                                                           
  // get the python configuration
  const edm::ParameterSet& process = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");
  fwlite::InputSource inputHandler_(process); fwlite::OutputFiles outputHandler_(process);

  // arguments from py file
  int maxEvents_( inputHandler_.maxEvents() );


  // ----------------------------------------------
  // output file
  TFile *outputFile = new TFile(outputHandler_.file().c_str(),"RECREATE");

  // book a tree per event
  TTree *eventTree = new TTree("eventVar","eventVar");
  TString treeEvent = "nPho/I:nMatchedPho/I";
  eventTree->Branch("event",&(treeEv_.nPho),treeEvent);  

  // book a tree with photon variables
  TTree *phoTree = new TTree("singlePhotons","singlePhotons");
  TString treeKine = "pt/F:eta/F:phi/F:isEBEtaGap/I:isEBPhiGap/I:isEERingGap/I:isEEDeeGap/I:isEBEEGap/I:iPhi/I:iEta/I";
  TString treeSc   = "scEta/F:scPhi/F";
  // TString treeEne  = "eMax/F:e5x5/F:energy/F:energyInitial/F:energyRegression/F";
  TString treeEne  = "eMax/F:e5x5/F:energy/F:";
  TString treeID   = "e1x5/F:e2x5/F:sigmaIetaIeta/F:r9/F:hoe/F:h1oe/F:h2oe/F:htoe/F:ht1oe/F:ht2oe/F:passEleVeto/B:hasPixelSeed/B";
  TString treeIso  = "trackIso/F:ecalIso/F:hcalIso/F:chHadIso/F:nHadIso/F:photonIso/F:rho/F";
  TString treeTrue = "trueEnergy/F:truePt/F:trueEta/F:truePhi/F:minDR/F";
  phoTree->Branch("kinematics",&(tree_.pt),treeKine);  
  phoTree->Branch("supercluster",&(tree_.scEta),treeSc);
  phoTree->Branch("energy",&(tree_.eMax),treeEne);  
  phoTree->Branch("identification",&(tree_.e1x5),treeID);  
  phoTree->Branch("isolation",&(tree_.trackIso),treeIso);  
  phoTree->Branch("mctruth",&(tree_.trueEnergy),treeTrue);  


  // ----------------------------------------------
  // loop the events
  int ievt=0;  
  for(unsigned int iFile=0; iFile<inputHandler_.files().size(); ++iFile){
    TFile* inFile = TFile::Open(inputHandler_.files()[iFile].c_str());
    if( inFile ){

      fwlite::Event ev(inFile);
      for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt){

	if (!ievt%100) cout << ievt << endl;
	
       	// break loop if maximal number of events is reached 
	if(maxEvents_>0 ? ievt+1>maxEvents_ : false) break;

	// Handle the needed collections
	fwlite::Handle<std::vector<flashgg::Photon> > objs_pho;  
	objs_pho.getByLabel(ev,"flashggPhotons");

	fwlite::Handle<std::vector<reco::GenParticle> > objs_gens;  
	objs_gens.getByLabel(ev,"prunedGenParticles");
	
	fwlite::Handle<double> objs_rho;
        objs_rho.getByLabel(ev,"fixedGridRhoAll");
	const double rhoFixedGrd = *(objs_rho.product());


	// real analysis
	int allGamma = 0;
	int matchedGamma = 0;
	for(std::vector<flashgg::Photon>::const_iterator g1=objs_pho->begin(); g1!=objs_pho->end(); ++g1){

	  // filling the tree
	  tree_.pt=g1->pt();
	  tree_.eta=g1->eta();
	  tree_.phi=g1->phi();
	  tree_.isEBEtaGap=g1->isEBEtaGap();
	  tree_.isEBPhiGap=g1->isEBPhiGap();
	  tree_.isEERingGap=g1->isEERingGap();
	  tree_.isEEDeeGap=g1->isEEDeeGap();
	  tree_.isEBEEGap=g1->isEBEEGap();
	  tree_.iPhi= g1->iPhi();
	  tree_.iEta= g1->iEta();

	  tree_.scEta = (g1->superCluster())->eta();
	  tree_.scPhi = (g1->superCluster())->phi();
	  
	  tree_.eMax=g1->maxEnergyXtal();
	  tree_.e5x5=g1->e5x5();
	  tree_.energy=g1->energy();
	  // tree_.energyInitial=g1->getEnergyAtStep("initial");
	  // tree_.energyRegression=g1->getEnergyAtStep("regression");
	  
	  tree_.e1x5=g1->e1x5();
	  tree_.e2x5=g1->e2x5();
	  tree_.sigmaIetaIeta = g1->sigmaIetaIeta();
	  tree_.r9 = g1->r9();
	  
	  tree_.hoe   = g1->hadronicOverEm();
	  tree_.h1oe  = g1->hadronicDepth1OverEm();
	  tree_.h2oe  = g1->hadronicDepth2OverEm();
	  tree_.htoe  = g1->hadTowOverEm();
	  tree_.ht1oe = g1->hadTowDepth1OverEm();
	  tree_.ht2oe = g1->hadTowDepth2OverEm();

	  tree_.passEleVeto  = g1->passElectronVeto();
	  tree_.hasPixelSeed = g1->hasPixelSeed();

	  // DeltaR=0.4 (di sicuro per detector-based, credo anche per PF-based)
	  tree_.trackIso  = g1->trackIso();
	  tree_.ecalIso   = g1->ecalIso();
	  tree_.hcalIso   = g1->hcalIso();
	  tree_.chHadIso  = g1->chargedHadronIso();
	  tree_.nHadIso   = g1->neutralHadronIso();
	  tree_.photonIso = g1->photonIso();
	  tree_.rho       = rhoFixedGrd;

	  float matchedEne = -999.;
	  float matchedPt  = -999.;
	  float matchedEta = -999.;
	  float matchedPhi = -999.;
	  float minDR      = 999.;
	  for( std::vector<reco::GenParticle>::const_iterator gen=objs_gens->begin(); gen!=objs_gens->end(); ++gen){

	    if( gen->pdgId() == 22 && gen->status() == 1 && (gen->mother())->pdgId()==5100039 ) {

	      float dR = deltaR(gen->eta(),gen->phi(),g1->eta(),g1->phi()); 
	      if ( dR<minDR ) {
		minDR = dR;
		if ( dR<0.1 ) {
		  matchedEne = gen->energy();
		  matchedPt  = gen->energy()*sin(gen->theta());
		  matchedEta = gen->eta();
		  matchedPhi = gen->phi();
		  break;
		}
	      }
	    }
	    
	  } // loop over gen photons
	  tree_.trueEnergy = matchedEne;
	  tree_.truePt     = matchedPt;
	  tree_.trueEta    = matchedEta;
	  tree_.truePhi    = matchedPhi;
	  tree_.minDR      = minDR;

	  // to count the number of reco photons and reco matching gen ones
	  allGamma++;
	  if (matchedEne>-1) matchedGamma++;
	  
	  outputFile->cd();
	  phoTree->Fill();
	}  // loop over reco photons

	// filling the tree with per-event infos
	treeEv_.nPho        = allGamma;
	treeEv_.nMatchedPho = matchedGamma;
	outputFile->cd();
	eventTree->Fill();

      } // loop over events
      
      inFile->Close();
    }

    // break loop if maximal number of events is reached:
    // this has to be done twice to stop the file loop as well
    if(maxEvents_>0 ? ievt+1>maxEvents_ : false) break;
  }

  // saving histos
  outputFile->Write();
  outputFile->Close();

  return 0;
}
