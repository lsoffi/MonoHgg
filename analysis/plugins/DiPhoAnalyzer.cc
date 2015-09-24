#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"

//#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
//#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"

#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"


#include "TMath.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#define MAX_PU_REWEIGHT 60

using namespace std;
using namespace edm;
using namespace flashgg;

using pat::PackedGenParticle;   

// diphoton tree
struct diphoTree_struc_ {

  int hltPhoton26Photon16Mass60;
  int hltPhoton36Photon22Mass15;
  int hltPhoton42Photon25Mass15;
  int hltDiphoton30Mass95;
  int hltDiphoton30Mass70;
  int hltDiphoton30Mass55;
  int hltDiphoton30Mass55PV;
  int hltDiphoton30Mass55EB;
  int run;
  int event;
  int lumi;
  int nvtx;
  float rho;
  int sampleID;
  float totXsec;
  float pu_weight;
  float pu_n;
  float sumDataset;
  float perEveW;
  float calomet;
  float calometPhi;
  float calometSumEt;
  float pfmet;
  float pfmetPhi;
  float pfmetSumEt;
  float t1pfmet;
  float t1pfmetPhi;
  float t1pfmetSumEt;
  float ptgg;
  float mgg;
  int eventClass;
  float pt1; 
  float ptOverM1; 
  float eta1; 
  float phi1;
  float sceta1;
  float r91; 
  float sieie1; 
  float hoe1; 
  float scRawEne1;
  float chiso1; 
  float phoiso1; 
  float neuiso1;
  int eleveto1;
  float pt2; 
  float ptOverM2; 
  float eta2; 
  float phi2;
  float sceta2;
  float r92; 
  float sieie2; 
  float hoe2; 
  float scRawEne2;
  float chiso2; 
  float phoiso2; 
  float neuiso2;
  int eleveto2;
  int presel1;
  int presel2;
  int sel1;
  int sel2;
  int vtxIndex;
  float vtxX; 
  float vtxY; 
  float vtxZ;
  int genmatch1;   
  int genmatch2;
  float genmgg;
  float geniso1;   
  float geniso2;
  float genVtxX; 
  float genVtxY; 
  float genVtxZ;
  int passCHiso1;
  int passCHiso2;
  int passNHiso1; 
  int passNHiso2;
  int passPHiso1;
  int passPHiso2;
  int passSieie1;
  int passSieie2;
  int passHoe1;
  int passHoe2;
};

class DiPhoAnalyzer : public edm::EDAnalyzer {
  
public:
  
  explicit DiPhoAnalyzer(const edm::ParameterSet&);
  ~DiPhoAnalyzer();
  
private:
  
  edm::Service<TFileService> fs_;
  
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  void initTreeStructure();

  void SetPuWeights(std::string puWeightFile);
  float GetPUWeight(float pun);
  bool isGammaPresel( float sceta, float pt, float r9, float chiso);
  bool isGammaSelected( float rho, float pt, float sceta, float r9, float chiso, float nhiso, float phoiso, float hoe, float sieie, bool passElectronVeto);
  int effectiveAreaRegion(float sceta);
  bool testPhotonIsolation(int passSieie, int passCHiso, int passNHiso, int passPHiso, int passHoe);
  //bool testPhotonIsolation(float rho,float pt, float sceta, float r9, float chiso, float nhiso, float phoiso , float hoe, float sieie, bool passElectronVeto);
  double getGammaEAForPhotonIso(float sceta);
  double getChargedHadronEAForPhotonIso(float sceta);
  double getNeutralHadronEAForPhotonIso(float sceta);
  int passSieieCuts(float sceta, float sieie);
  int passCHisoCuts(float sceta, float chiso, float pt);
  int passNHisoCuts(float sceta, float nhiso, float pt);
  int passPHisoCuts(float sceta, float phiso, float pt);
  int passHoeCuts(float sceta, float hoe);

  // collections
  //edm::EDGetTokenT<EcalRecHitCollection> ecalHitEBToken_;
  //edm::EDGetTokenT<EcalRecHitCollection> ecalHitEEToken_;
  EDGetTokenT<View<reco::Vertex> > vertexToken_;
  EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diPhotonToken_; 
  EDGetTokenT<edm::View<PileupSummaryInfo> > PileUpToken_; 
  edm::InputTag rhoFixedGrid_;
  EDGetTokenT<vector<flashgg::GenPhotonExtra> > genPhotonExtraToken_;
  edm::InputTag genInfo_;
  EDGetTokenT<View<reco::GenParticle> > genPartToken_;

  EDGetTokenT<View<pat::MET> > METToken_;

  EDGetTokenT<edm::TriggerResults> triggerBitsToken_;
  // sample-dependent parameters needed for the analysis
  int dopureweight_;
  int sampleIndex_;
  string puWFileName_;
  float xsec_;    // pb
  float kfac_;
  float sumDataset_;

  // to compute weights for pileup
  std::vector<Double_t> puweights_;

  // output tree with several diphoton infos
  TTree *DiPhotonTree;
  diphoTree_struc_ treeDipho_;

  // to keep track of the number of events
  TH1F *h_entries;

  // to keep track of the sum of weights
  TH1F *h_sumW;
  bool isFilled;

  // events breakdown
  TH1F *h_selection;
};
   

DiPhoAnalyzer::DiPhoAnalyzer(const edm::ParameterSet& iConfig):

  // collections
  //reducedBarrelRecHitCollection_ = iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection");
  //reducedBarrelRecHitCollectionToken_ = mayConsume<EcalRecHitCollection>(reducedBarrelRecHitCollection_);
  //ecalHitEBToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection"))),
  //ecalHitEEToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEndcapRecHitCollection"))),
  vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
  diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
  PileUpToken_(consumes<View<PileupSummaryInfo> >(iConfig.getUntrackedParameter<InputTag> ("PileUpTag", InputTag("addPileupInfo")))),
  genPhotonExtraToken_(mayConsume<vector<flashgg::GenPhotonExtra> >(iConfig.getParameter<InputTag>("genPhotonExtraTag"))),
  genPartToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticlesTag", InputTag("flashggPrunedGenParticles")))),
  METToken_( consumes<View<pat::MET> >( iConfig.getUntrackedParameter<InputTag> ( "METTag", InputTag( "slimmedMETs" ) ) ) ),
  triggerBitsToken_( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "bits" ) ) )
{ 
  dopureweight_ = iConfig.getUntrackedParameter<int>("dopureweight", 0);
  sampleIndex_  = iConfig.getUntrackedParameter<int>("sampleIndex",0);
  puWFileName_  = iConfig.getParameter<std::string>("puWFileName");   
  xsec_         = iConfig.getUntrackedParameter<double>("xsec",1.); 
  kfac_         = iConfig.getUntrackedParameter<double>("kfac",1.); 
  sumDataset_   = iConfig.getUntrackedParameter<double>("sumDataset",-999.);
  genInfo_      = iConfig.getParameter<edm::InputTag>("generatorInfo"); 
};

DiPhoAnalyzer::~DiPhoAnalyzer() { };

void DiPhoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  // Sample index
  int sampleID = sampleIndex_;

  // access edm objects                                                                                    
  Handle<View<reco::Vertex> > primaryVertices;
  iEvent.getByToken(vertexToken_,primaryVertices);

  Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
  iEvent.getByToken(diPhotonToken_,diPhotons);

  Handle<View< PileupSummaryInfo> > PileupInfos;
  iEvent.getByToken(PileUpToken_,PileupInfos);
  
  Handle<double> objs_rho;
  iEvent.getByLabel("fixedGridRhoAll",objs_rho);

  Handle<vector<flashgg::GenPhotonExtra> > genPhotonsHandle;
  edm::Handle<GenEventInfoProduct> genInfo;
  edm::Handle<View<reco::GenParticle> > genParticles;
 
  if (sampleID>0 && sampleID<10000) {     // MC
    iEvent.getByToken(genPhotonExtraToken_,genPhotonsHandle);
    iEvent.getByLabel(genInfo_,genInfo);   
    iEvent.getByToken( genPartToken_, genParticles );
  }


  // to recompute not-zero-suppressed cluster shapes 
  //noZS::EcalClusterLazyTools *lazyToolnoZS = new noZS::EcalClusterLazyTools(iEvent, iSetup, ecalHitEBToken_, ecalHitEEToken_);

  //EcalClusterLazyTools lazyTools(iEvent, iSetup, reducedBarrelRecHitCollectionToken_, reducedEndcapRecHitCollectionToken_);
  // To keep track of the total number of events
  h_entries->Fill(5);

  Handle<View<pat::MET> > METs;
  iEvent.getByToken( METToken_, METs );

  Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken( triggerBitsToken_, triggerBits );

  // --------------------------------------------------
  //std::cout<<"------------------------------"<<std::endl;

  //Trigger info
 
  //HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v2 
  //HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v2
  //HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon25_AND_HE10_R9Id65_Eta2_Mass15_v1
  //HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v1
  //HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v1 
  //HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1
  //HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v1
  //HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1

  int hltPhoton26Photon16Mass60=-500;
  int hltPhoton36Photon22Mass15=-500;
  int hltPhoton42Photon25Mass15=-500;
  int hltDiphoton30Mass95=-500;
  int hltDiphoton30Mass70=-500;
  int hltDiphoton30Mass55=-500;
  int hltDiphoton30Mass55EB=-500;  
  int hltDiphoton30Mass55PV=-500;

  const edm::TriggerNames &triggerNames = iEvent.triggerNames( *triggerBits );
  vector<std::string> const &names = triggerNames.triggerNames();  
  for( unsigned index = 0; index < triggerNames.size(); ++index ) {
    // print out triggers that match "HLT_Photon or HLT_Diphoton" and have "Mass" as well
    //if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Photon") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass")  ) cout << index << " " << triggerNames.triggerName( index ) << " " << triggerBits->accept( index ) << endl;
    //if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Diphoton") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass")  ) cout << index << " " << triggerNames.triggerName( index ) << " " << triggerBits->accept( index ) << endl;
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Photon26") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Photon16")&& (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass60")  )hltPhoton26Photon16Mass60 = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Photon36") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Photon22")&& (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass15")  )hltPhoton36Photon22Mass15 = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Photon42") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Photon25")&& (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass15")  )hltPhoton42Photon25Mass15 = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Diphoton30") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass95")  )hltDiphoton30Mass95 = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Diphoton30") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("Mass70")  )hltDiphoton30Mass70 = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Diphoton30PV") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("DoublePixelVeto_Mass55")  )hltDiphoton30Mass55PV = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Diphoton30") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("R9Id_Mass55")  )hltDiphoton30Mass55 = triggerBits->accept( index );
    if( (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("HLT_Diphoton30EB") && (TString::Format((triggerNames.triggerName( index )).c_str())).Contains("DoublePixelVeto_Mass55")  )hltDiphoton30Mass55EB = triggerBits->accept( index );

}
  

  // Event info
  int run   = iEvent.eventAuxiliary().run();
  int event = iEvent.eventAuxiliary().event();
  int lumi  = iEvent.eventAuxiliary().luminosityBlock(); 

  // # Vertices
  int nvtx = primaryVertices->size(); 

  // Energy density
  float rho = *(objs_rho.product());

  // PU weight (for MC only and if requested)
  float pu_weight = 1.;
  float pu_n      = -1.;
  if (sampleID>0 && sampleID<10000) {     // MC
    //  pu_n = 0.;
    //for( unsigned int PVI = 0; PVI < PileupInfos->size(); ++PVI ) {
    //Int_t pu_bunchcrossing = PileupInfos->ptrAt( PVI )->getBunchCrossing();
    //if( pu_bunchcrossing == 0 ) {
    //	pu_n = PileupInfos->ptrAt( PVI )->getPU_NumInteractions();
    //}
    //}
    if (dopureweight_) 
      pu_weight = GetPUWeight(nvtx);         
  }
  
 
  // x-sec * kFact for MC only 
  float totXsec = 1.;
  if (sampleID>0 && sampleID<10000) totXsec = xsec_ * kfac_;

  // other weights for the dataset
  float sumDataset = 1.;  
  float perEveW    = 1.;
  if (sampleID>0 && sampleID<10000) { 
    sumDataset = sumDataset_;
    const auto & eveWeights = genInfo->weights();
    if(!eveWeights.empty()) perEveW = eveWeights[0];
  }

  // To keep track of the sum of weights
  if (!isFilled) {
    h_sumW->Fill(5,sumDataset);
    isFilled = true;
  }

  // Events breakdown
  if(hltDiphoton30Mass95)h_selection->Fill(0.,perEveW);


  // Get MET
  if( METs->size() != 1 )
    { std::cout << "WARNING number of MET is not equal to 1" << std::endl; }
  Ptr<pat::MET> theMET = METs->ptrAt( 0 );


  // Loop over diphoton candidates
  if (diPhotons->size()>0) {

    // Diphoton candidates: preselection
    vector<int> preselDipho;
    for( size_t diphotonlooper = 0; diphotonlooper < diPhotons->size(); diphotonlooper++ ) {

      Ptr<flashgg::DiPhotonCandidate> diphoPtr = diPhotons->ptrAt( diphotonlooper );      
      
      float leadScEta  = (diphoPtr->leadingPhoton()->superCluster())->eta();         
      float leadPt     = diphoPtr->leadingPhoton()->et();
      float leadChIso  = diphoPtr->leadingPhoton()->egChargedHadronIso()- rho * getChargedHadronEAForPhotonIso((diphoPtr->leadingPhoton()->superCluster())->eta());//Livia correction: add pu correction here
      float leadR9noZS = diphoPtr->leadingPhoton()->full5x5_r9(); 
      bool leadPresel  = isGammaPresel( leadScEta, leadPt, leadR9noZS, leadChIso); 

      float subleadScEta  = (diphoPtr->subLeadingPhoton()->superCluster())->eta();               
      float subleadPt     = diphoPtr->subLeadingPhoton()->et();
      float subleadChIso  = diphoPtr->subLeadingPhoton()->egChargedHadronIso()- rho * getChargedHadronEAForPhotonIso((diphoPtr->subLeadingPhoton()->superCluster())->eta());
      float subleadR9noZS = diphoPtr->subLeadingPhoton()->full5x5_r9();
      bool subleadPresel  = isGammaPresel( subleadScEta, subleadPt, subleadR9noZS, subleadChIso); 
      if (!leadPresel || !subleadPresel) continue;   

      preselDipho.push_back(diphotonlooper);
    }



    if (preselDipho.size()>0) {
      h_selection->Fill(1.,perEveW);
      
      // Diphoton candidates: Id/isolation selection
      vector<int> selectedDipho;
      for( size_t diphotonlooper = 0; diphotonlooper < preselDipho.size(); diphotonlooper++ ) {

	int theDiphoton = preselDipho[diphotonlooper];
	Ptr<flashgg::DiPhotonCandidate> diphoPtr = diPhotons->ptrAt( theDiphoton );
	
	// chiara: init comment x efficiencies
	/*std::vector<float> leadCovnoZS    = lazyToolnoZS->localCovariances(*(diphoPtr->leadingPhoton()->superCluster()->seed())) ;
	std::vector<float> subleadCovnoZS = lazyToolnoZS->localCovariances(*(diphoPtr->subLeadingPhoton()->superCluster()->seed())) ;
	
	float leadSieienoZS;
	if (!isnan(leadCovnoZS[0]))
	  leadSieienoZS = sqrt (leadCovnoZS[0]); 
	else 
	  continue;
	
	float subleadSieienoZS;
	if (!isnan(subleadCovnoZS[0]))
	  subleadSieienoZS = sqrt (subleadCovnoZS[0]); 
	else 
	  continue;*/


	float leadPt     = diphoPtr->leadingPhoton()->et();
	float leadScEta  = (diphoPtr->leadingPhoton()->superCluster())->eta();   
	//float leadR9noZS = diphoPtr->leadingPhoton()->full5x5_r9();
        float leadSieienoZS = diphoPtr->leadingPhoton()->full5x5_sigmaIetaIeta();
	float leadHoE    = diphoPtr->leadingPhoton()->hadTowOverEm();
	
	float leadChIso  = TMath::Max(diphoPtr->leadingPhoton()->egChargedHadronIso()- rho * getChargedHadronEAForPhotonIso((diphoPtr->leadingPhoton()->superCluster())->eta()),0.);
	float leadNeuIso = TMath::Max(diphoPtr->leadingPhoton()->egNeutralHadronIso()- rho * getNeutralHadronEAForPhotonIso((diphoPtr->leadingPhoton()->superCluster())->eta()),0.);
	float leadPhoIso = TMath::Max(diphoPtr->leadingPhoton()->egPhotonIso()- rho * getGammaEAForPhotonIso((diphoPtr->leadingPhoton()->superCluster())->eta()),0.);
	//bool  leadOkEleV = diphoPtr->leadingPhoton()->passElectronVeto();
	//bool  leadSelel  = isGammaSelected( rho, leadPt, leadScEta, leadR9noZS, leadChIso, leadNeuIso, leadPhoIso, leadHoE, leadSieienoZS, leadOkEleV); 
	//bool  leadSelel  = testPhotonIsolation( rho, leadPt, leadScEta, leadR9noZS, leadChIso, leadNeuIso, leadPhoIso, leadHoE, leadSieienoZS, leadOkEleV); 
	int passLeadSieie = passSieieCuts( leadScEta, leadSieienoZS );
        int passLeadCHiso = passCHisoCuts( leadScEta, leadChIso, leadPt );
        int passLeadNHiso = passNHisoCuts( leadScEta, leadNeuIso, leadPt );
        int passLeadPHiso = passPHisoCuts( leadScEta, leadPhoIso, leadPt );
	int passLeadHoe   = passHoeCuts( leadScEta, leadHoE );
        bool leadSelel    = testPhotonIsolation( passLeadSieie, passLeadCHiso, passLeadNHiso, passLeadPHiso, passLeadHoe ); 

	float subleadPt     = diphoPtr->subLeadingPhoton()->et();
	float subleadScEta  = (diphoPtr->subLeadingPhoton()->superCluster())->eta();   
	//float subleadR9noZS = diphoPtr->subLeadingPhoton()->full5x5_r9();
        float subleadSieienoZS = diphoPtr->subLeadingPhoton()->full5x5_sigmaIetaIeta();
	float subleadHoE    = diphoPtr->subLeadingPhoton()->hadTowOverEm();
	float subleadChIso  = TMath::Max(diphoPtr->subLeadingPhoton()->egChargedHadronIso()- rho * getChargedHadronEAForPhotonIso((diphoPtr->subLeadingPhoton()->superCluster())->eta()),0.);
	float subleadNeuIso = TMath::Max(diphoPtr->subLeadingPhoton()->egNeutralHadronIso()- rho * getNeutralHadronEAForPhotonIso((diphoPtr->subLeadingPhoton()->superCluster())->eta()),0.);
	float subleadPhoIso = TMath::Max(diphoPtr->subLeadingPhoton()->egPhotonIso()- rho * getGammaEAForPhotonIso((diphoPtr->subLeadingPhoton()->superCluster())->eta()),0.);
	//bool  subleadOkEleV = diphoPtr->subLeadingPhoton()->passElectronVeto();
	//bool  subleadSelel  = isGammaSelected( rho, subleadPt, subleadScEta, subleadR9noZS, subleadChIso, subleadNeuIso, subleadPhoIso, subleadHoE, subleadSieienoZS, subleadOkEleV);  
	//bool  subleadSelel  = testPhotonIsolation( rho, subleadPt, subleadScEta, subleadR9noZS, subleadChIso, subleadNeuIso, subleadPhoIso, subleadHoE, subleadSieienoZS, subleadOkEleV);  
	int passSubLeadSieie = passSieieCuts( subleadScEta, subleadSieienoZS );
        int passSubLeadCHiso = passCHisoCuts( subleadScEta, subleadChIso, subleadPt );
        int passSubLeadNHiso = passNHisoCuts( subleadScEta, subleadNeuIso, subleadPt );
        int passSubLeadPHiso = passPHisoCuts( subleadScEta, subleadPhoIso, subleadPt );
	int passSubLeadHoe   = passHoeCuts( subleadScEta, subleadHoE );
        bool subleadSelel    = testPhotonIsolation( passSubLeadSieie, passSubLeadCHiso, passSubLeadNHiso, passSubLeadPHiso, passSubLeadHoe ); 

        int numpassing = 0;
	if (leadSelel || subleadSelel) numpassing++;
	
	if (!leadSelel || !subleadSelel ) continue; //Livia Correction: applies pho ID selection 
	// chiara: end comment x efficiencies
	

	selectedDipho.push_back(theDiphoton);    
      }
     
      if (selectedDipho.size()>0) {
	h_selection->Fill(2.,perEveW);

	// Diphoton candidates: pT cuts
	vector<int> kineDipho;
        for( size_t diphotonlooper = 0; diphotonlooper < selectedDipho.size(); diphotonlooper++ ) {

	  int theDiphoton = selectedDipho[diphotonlooper];
	  Ptr<flashgg::DiPhotonCandidate> diphoPtr = diPhotons->ptrAt( theDiphoton );
	  
	  float leadPt    = diphoPtr->leadingPhoton()->et();
	  float subleadPt = diphoPtr->subLeadingPhoton()->et();

	  if (leadPt<30 || subleadPt<20) continue;             // chiara: x ana: 80; x phys14: 200; x sinc 100

	  kineDipho.push_back(theDiphoton);
	}


	if (kineDipho.size()>0) {
	  h_selection->Fill(3.,perEveW);

	  // Diphoton candidates: mgg cut
	  vector<int> massDipho;
	  for( size_t diphotonlooper = 0; diphotonlooper < kineDipho.size(); diphotonlooper++ ) {

	    int theDiphoton = kineDipho[diphotonlooper];
	    Ptr<flashgg::DiPhotonCandidate> diphoPtr = diPhotons->ptrAt( theDiphoton );
	    
	    float thisSystemMgg = diphoPtr->mass();

	    if (thisSystemMgg<50 ) continue;    // chiara: x ana: 300; x phys14: 500; x sinc 200 

	    float leadPt    = diphoPtr->leadingPhoton()->et();
	    float subleadPt = diphoPtr->subLeadingPhoton()->et();

	    if (leadPt< thisSystemMgg/3 || subleadPt<thisSystemMgg/4) continue;             //Livia correction: add scaling pt cuts

	    massDipho.push_back(theDiphoton);
	  }
  
	  if (massDipho.size()>0) {
	    h_selection->Fill(4.,perEveW);

	    // chiara: studiare il numero di candidati

	    // Diphoton candidates choice: highest scalar sum pT
	    float maxSumPt = -999.;
	    int candIndex = 9999; // This int will store the index of the best diphoton candidate
	    for( size_t diphotonlooper = 0; diphotonlooper < massDipho.size(); diphotonlooper++ ) {  

	      int theDiphoton = massDipho[diphotonlooper];
	      Ptr<flashgg::DiPhotonCandidate> diphoPtr = diPhotons->ptrAt( theDiphoton );

	      float thisSumPt = diphoPtr->leadingPhoton()->et() + diphoPtr->subLeadingPhoton()->et();
	      if (thisSumPt>maxSumPt) {
		maxSumPt = thisSumPt;
		candIndex = theDiphoton;
	      }
	    }
	    
	    if (candIndex<999) {

	      Ptr<flashgg::DiPhotonCandidate> candDiphoPtr = diPhotons->ptrAt( candIndex );
	    
	      // analysis cuts: good vertex 
	      // Since diphoton candidates have already an associated vtx, I check it only and discard the event if bad 
	      // this is why I put this selection AFTER the diphoton choice
	      bool goodVtx = true;
	      int theVertex = candDiphoPtr->vertexIndex();
	      float vtxX = (primaryVertices->ptrAt(theVertex))->position().x();
	      float vtxY = (primaryVertices->ptrAt(theVertex))->position().y();
	      float d0vtx = sqrt( vtxX*vtxX + vtxY*vtxY );
	      if ( (primaryVertices->ptrAt(theVertex))->ndof()<=4 )  goodVtx = false;
	      if (fabs(d0vtx)>2) goodVtx = false;
	      if (fabs((primaryVertices->ptrAt(theVertex))->position().z())>=24) goodVtx = false;
	      bool isVtxFake = ((primaryVertices->ptrAt(theVertex))->ndof()==0) && ((primaryVertices->ptrAt(theVertex))->chi2()==0);   // chiara: also && tracks.empty, but can not be used here
	      if (isVtxFake) goodVtx = false;

	      // chiara: studiare quanti sono e quante volte e' il primo
	      
	      if (goodVtx) {
		h_selection->Fill(5.,perEveW);

		// to be kept in the tree
		float ptgg, mgg;
		int eventClass;
		float pt1, ptOverM1, eta1, phi1;
		float sceta1;
		float r91, sieie1, hoe1, scRawEne1;
		float chiso1, phoiso1, neuiso1;
		float pt2, ptOverM2, eta2, phi2;
		float sceta2;
		float r92, sieie2, hoe2, scRawEne2;
		float chiso2, phoiso2, neuiso2;
		int presel1, presel2, sel1, sel2;
		int vtxIndex;
		float vtxX, vtxY, vtxZ;
		int genmatch1, genmatch2;
		float genmgg;
		float geniso1, geniso2;
		float genVtxX, genVtxY, genVtxZ;   
		int eleveto1, eleveto2;
		float pfmet,pfmetPhi, pfmetSumEt,t1pfmet,t1pfmetPhi, t1pfmetSumEt,calomet,calometPhi, calometSumEt;
                int passCHiso1, passCHiso2, passNHiso1, passNHiso2, passPHiso1, passPHiso2, passSieie1, passSieie2, passHoe1, passHoe2;

		// fully selected event: tree re-initialization                                                                          
		initTreeStructure();        
		
		//met
		t1pfmet = theMET->pt();
		t1pfmetPhi = theMET->phi();
		t1pfmetSumEt = theMET->sumEt();
		pfmet = theMET->uncorrectedPt();
		pfmetPhi = theMET->uncorrectedPhi();
		pfmetSumEt = theMET->uncorrectedSumEt();
		calomet = theMET->caloMETPt();
		calometPhi = theMET->caloMETPhi();
		calometSumEt = theMET->caloMETSumEt();

		
		//-------> diphoton system properties 
		ptgg = candDiphoPtr->pt();
		mgg  = candDiphoPtr->mass();
				
		//-------> individual photon properties
		//std::vector<float> leadCovnoZS = lazyToolnoZS->localCovariances(*(candDiphoPtr->leadingPhoton()->superCluster()->seed())) ;
		//std::vector<float> subleadCovnoZS = lazyToolnoZS->localCovariances(*(candDiphoPtr->subLeadingPhoton()->superCluster()->seed())) ;
		
		pt1       = candDiphoPtr->leadingPhoton()->et();
		ptOverM1  = pt1/mgg;
		eta1      = candDiphoPtr->leadingPhoton()->eta();
		phi1      = candDiphoPtr->leadingPhoton()->phi();
		sceta1    = (candDiphoPtr->leadingPhoton()->superCluster())->eta();
		r91	  = candDiphoPtr->leadingPhoton()->full5x5_r9();
		sieie1	  = candDiphoPtr->leadingPhoton()->full5x5_sigmaIetaIeta();
		//r91       = lazyToolnoZS->e3x3(*(candDiphoPtr->leadingPhoton()->superCluster()->seed())) / candDiphoPtr->leadingPhoton()->superCluster()->rawEnergy();
		//sieie1    = sqrt(leadCovnoZS[0]);
		hoe1      = candDiphoPtr->leadingPhoton()->hadTowOverEm();
		scRawEne1 = candDiphoPtr->leadingPhoton()->superCluster()->rawEnergy();
		/*	chiso1    = candDiphoPtr->leadingPhoton()->egChargedHadronIso();
		phoiso1   = candDiphoPtr->leadingPhoton()->egPhotonIso();
		neuiso1   = candDiphoPtr->leadingPhoton()->egNeutralHadronIso();*/
		chiso1    = TMath::Max(candDiphoPtr->leadingPhoton()->egChargedHadronIso()- rho * getChargedHadronEAForPhotonIso((candDiphoPtr->leadingPhoton()->superCluster())->eta()),0.);
		neuiso1   = TMath::Max(candDiphoPtr->leadingPhoton()->egNeutralHadronIso()- rho * getNeutralHadronEAForPhotonIso((candDiphoPtr->leadingPhoton()->superCluster())->eta()),0.);
		phoiso1   = TMath::Max(candDiphoPtr->leadingPhoton()->egPhotonIso()- rho * getGammaEAForPhotonIso((candDiphoPtr->leadingPhoton()->superCluster())->eta()),0.);

		eleveto1  = 0;
		if (candDiphoPtr->leadingPhoton()->passElectronVeto()) eleveto1 = 1;
		//bool eleveto1b = candDiphoPtr->leadingPhoton()->passElectronVeto();

		pt2       = candDiphoPtr->subLeadingPhoton()->et();
		ptOverM2  = pt2/mgg;
		eta2      = candDiphoPtr->subLeadingPhoton()->eta();
		phi2      = candDiphoPtr->subLeadingPhoton()->phi();
		sceta2    = (candDiphoPtr->subLeadingPhoton()->superCluster())->eta();
		r92	  = candDiphoPtr->subLeadingPhoton()->full5x5_r9();
		//r92       = lazyToolnoZS->e3x3(*(candDiphoPtr->subLeadingPhoton()->superCluster()->seed())) / candDiphoPtr->subLeadingPhoton()->superCluster()->rawEnergy();;
		sieie2	  = candDiphoPtr->subLeadingPhoton()->full5x5_sigmaIetaIeta();
		//sieie2    = sqrt(subleadCovnoZS[0]);
		hoe2      = candDiphoPtr->subLeadingPhoton()->hadTowOverEm();
		scRawEne2 = candDiphoPtr->subLeadingPhoton()->superCluster()->rawEnergy();
		/*chiso2    = candDiphoPtr->subLeadingPhoton()->egChargedHadronIso();
		phoiso2   = candDiphoPtr->subLeadingPhoton()->egPhotonIso();
		neuiso2   = candDiphoPtr->subLeadingPhoton()->egNeutralHadronIso();*/
		chiso2    = TMath::Max(candDiphoPtr->subLeadingPhoton()->egChargedHadronIso()- rho * getChargedHadronEAForPhotonIso((candDiphoPtr->subLeadingPhoton()->superCluster())->eta()),0.);
		neuiso2   = TMath::Max(candDiphoPtr->subLeadingPhoton()->egNeutralHadronIso()- rho * getNeutralHadronEAForPhotonIso((candDiphoPtr->subLeadingPhoton()->superCluster())->eta()),0.);      	       
		phoiso2   = TMath::Max(candDiphoPtr->subLeadingPhoton()->egPhotonIso()- rho * getGammaEAForPhotonIso((candDiphoPtr->subLeadingPhoton()->superCluster())->eta()),0.);
	
		eleveto2  = 0;
		if (candDiphoPtr->subLeadingPhoton()->passElectronVeto()) eleveto2 = 1;
		//bool eleveto2b = candDiphoPtr->subLeadingPhoton()->passElectronVeto();

		//-------> photon selection (should be on, may be useful for extra studies
		presel1 = isGammaPresel( sceta1, pt1, r91, chiso1 ); 
		presel2 = isGammaPresel( sceta2, pt2, r92, chiso2 ); 
		//	sel1 = isGammaSelected( rho, pt1, sceta1, r91, chiso1, neuiso1, phoiso1, hoe1, sieie1, eleveto1b ); 
		//sel1 = testPhotonIsolation( rho, pt1, sceta1, r91, chiso1, neuiso1, phoiso1, hoe1, sieie1, eleveto1b ); 
		//sel2 = testPhotonIsolation( rho, pt2, sceta2, r92, chiso2, neuiso2, phoiso2, hoe2, sieie2, eleveto2b ); 


                //-------> pass each photon ID cut separately
		passSieie1 = passSieieCuts( sceta1, sieie1);
		passSieie2 = passSieieCuts( sceta2, sieie2);
                passCHiso1 = passCHisoCuts( sceta1, chiso1, pt1);
                passCHiso2 = passCHisoCuts( sceta2, chiso2, pt2);
		passNHiso1 = passNHisoCuts( sceta1, neuiso1, pt1);
		passNHiso2 = passNHisoCuts( sceta2, neuiso2, pt2);
		passPHiso1 = passPHisoCuts( sceta1, phoiso1, pt1);
		passPHiso2 = passPHisoCuts( sceta2, phoiso2, pt2);
		passHoe1   = passHoeCuts( sceta1, hoe1);
		passHoe2   = passHoeCuts( sceta2, hoe2);
		sel1 = testPhotonIsolation( passSieie1, passCHiso1, passNHiso1, passPHiso1, passHoe1 );
		sel2 = testPhotonIsolation( passSieie2, passCHiso2, passNHiso2, passPHiso2, passHoe2 );

		//-------> event class
		float maxEta = sceta1;
		if (fabs(sceta2)>fabs(sceta1)) maxEta = sceta2;
		
		float minR9 = r92;
		if ( r91<r92 ) minR9 = r91;
		
		eventClass = -1;
		if (fabs(maxEta)<1.5 && minR9>0.94) eventClass = 0;
		else if (fabs(maxEta)<1.5 && minR9<0.94) eventClass = 1;
		else if (fabs(maxEta)>1.5 && minR9>0.94) eventClass = 2;
		else if (fabs(maxEta)>1.5 && minR9<0.94) eventClass = 3;

		//-------> vtx info
		vtxIndex = candDiphoPtr->vertexIndex();
		vtxX= candDiphoPtr->vtx()->x();
		vtxY= candDiphoPtr->vtx()->y();
		vtxZ= candDiphoPtr->vtx()->z();
		
		//-------> generated vtx info
		genVtxX = -999.;
		genVtxY = -999.;
		genVtxZ = -999.;
		if (sampleID>0 && sampleID<10000) {     // MC
		  for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
		    
		    if( genParticles->ptrAt( genLoop )->pdgId() != 2212 || genParticles->ptrAt( genLoop )->vertex().z() != 0. ) {
		      genVtxX = genParticles->ptrAt( genLoop )->vertex().x();
		      genVtxY = genParticles->ptrAt( genLoop )->vertex().y();
		      genVtxZ = genParticles->ptrAt( genLoop )->vertex().z();
		      break;
		    }
		  }
		}
	      	
		//-------> photons, MC truth match
		genmatch1 = -999;
		genmatch2 = -999;
		geniso1   = -999.;
		geniso2   = -999.;
		if (sampleID>0 && sampleID<10000) {   

		  const auto & genPhotons = *genPhotonsHandle;
		  
		  if (candDiphoPtr->leadingPhoton()->hasMatchedGenPhoton()) {
		    genmatch1 = (candDiphoPtr->leadingPhoton()->genMatchType() == Photon::kPrompt); 
		    for (unsigned int j = 0 ; j < genPhotons.size() ; j++) {   
		      auto igen = genPhotons[j].ptr();
		      if ( igen->status() != 1 || igen->pdgId() != 22 ) continue; 
		      if ( fabs(igen->eta()-candDiphoPtr->leadingPhoton()->matchedGenPhoton()->eta())<0.001 && fabs(igen->phi()-candDiphoPtr->leadingPhoton()->matchedGenPhoton()->phi())<0.001 ) {
			auto & extra = genPhotons[j];
			geniso1 = extra.genIso();
			break;
		      }
		    }
		  }
		  
		  if (candDiphoPtr->subLeadingPhoton()->hasMatchedGenPhoton()) {
		    genmatch2 = (candDiphoPtr->subLeadingPhoton()->genMatchType() == Photon::kPrompt); 
		    for (unsigned int j = 0 ; j < genPhotons.size() ; j++) {   
		      auto igen = genPhotons[j].ptr();
		      if ( igen->status() != 1 || igen->pdgId() != 22 ) continue; 
		      if ( fabs(igen->eta()-candDiphoPtr->subLeadingPhoton()->matchedGenPhoton()->eta())<0.001 && fabs(igen->phi()-candDiphoPtr->subLeadingPhoton()->matchedGenPhoton()->phi())<0.001 ) {
			auto & extra = genPhotons[j];
			geniso2 = extra.genIso();
			break;
		      }
		    }
		  }
		}
		
		/* old version
		if (sampleID>0) {   
		  
		  const auto & genPhotons = *genPhotonsHandle;
		  for (unsigned int j = 0 ; j < genPhotons.size() ; j++) {
		  auto igen = genPhotons[j].ptr();
		  
		  if( igen->status() != 1 || igen->pdgId() != 22 ) continue; 
		  float deta = candDiphoPtr->leadingPhoton()->eta() - igen->eta();
		  float dphi = deltaPhi(candDiphoPtr->leadingPhoton()->phi(),igen->phi());
		  float dr = sqrt(deta*deta + dphi*dphi);
		  
		  if (dr<0.3) {
		  genmatch1 = j;  
		  break;
		  }
		  }
		  
		  if (genmatch1>=0) {
		  auto & extra = genPhotons[genmatch1];
		  geniso1 = extra.genIso();
		  }
		  }
		*/

		//--------> gen level mgg for signal samples
		genmgg = -999.;
		if (sampleID>99 && sampleID<10000) {  // signal only 

		  for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
		    
		    genmgg = -1999.;

		    if ( genParticles->ptrAt( genLoop )->pdgId()==5100039) {  // graviton

		      if (genParticles->ptrAt( genLoop )->numberOfDaughters()!=2) {
			genmgg = -2999.;
			break;
		      }

		      int statusd1 = genParticles->ptrAt( genLoop )->daughter(0)->status();
		      int statusd2 = genParticles->ptrAt( genLoop )->daughter(1)->status();
		      int pdgidd1  = genParticles->ptrAt( genLoop )->daughter(0)->pdgId();
		      int pdgidd2  = genParticles->ptrAt( genLoop )->daughter(1)->pdgId();
		      if (statusd1!=1 || statusd2!=1 || pdgidd1!=22 || pdgidd2!=22) { 
			genmgg = -3999.;
			break;
		      }

		      float ptd1  = genParticles->ptrAt( genLoop )->daughter(0)->pt();
		      float ptd2  = genParticles->ptrAt( genLoop )->daughter(1)->pt();
		      float etad1 = genParticles->ptrAt( genLoop )->daughter(0)->eta();
		      float etad2 = genParticles->ptrAt( genLoop )->daughter(1)->eta();
		      float phid1 = genParticles->ptrAt( genLoop )->daughter(0)->phi();
		      float phid2 = genParticles->ptrAt( genLoop )->daughter(1)->phi();
		      
		      TLorentzVector *myGenD1 = new TLorentzVector(0,0,0,0);
		      TLorentzVector *myGenD2 = new TLorentzVector(0,0,0,0);
		      myGenD1->SetPtEtaPhiM(ptd1, etad1, phid1, 0.);
		      myGenD2->SetPtEtaPhiM(ptd2, etad2, phid2, 0.);
		      genmgg = (*myGenD1+*myGenD2).M();

		      break;
		    }
		  }
		}

		// Variables for the tree
		treeDipho_.hltPhoton26Photon16Mass60=hltPhoton26Photon16Mass60;
		treeDipho_.hltPhoton36Photon22Mass15=hltPhoton36Photon22Mass15;
		treeDipho_.hltPhoton42Photon25Mass15=hltPhoton42Photon25Mass15;
		treeDipho_.hltDiphoton30Mass95=hltDiphoton30Mass95;
  		treeDipho_.hltDiphoton30Mass70=hltDiphoton30Mass70;
  		treeDipho_.hltDiphoton30Mass55=hltDiphoton30Mass55;
  		treeDipho_.hltDiphoton30Mass55PV=hltDiphoton30Mass55PV;
  		treeDipho_.hltDiphoton30Mass55EB=hltDiphoton30Mass55EB;
		treeDipho_.run = run;
		treeDipho_.event = event;
		treeDipho_.lumi = lumi;
		treeDipho_.nvtx = nvtx;
		treeDipho_.rho = rho;
		treeDipho_.sampleID = sampleID;  
		treeDipho_.totXsec = totXsec;  
		treeDipho_.pu_weight = pu_weight;
		treeDipho_.pu_n = pu_n;
		treeDipho_.sumDataset = sumDataset;
		treeDipho_.perEveW = perEveW;
		treeDipho_.pfmet = pfmet;
		treeDipho_.pfmet = pfmetPhi;
		treeDipho_.pfmet = pfmetSumEt;
		treeDipho_.t1pfmet = t1pfmet;
		treeDipho_.t1pfmetPhi = t1pfmetPhi;
		treeDipho_.t1pfmetSumEt = t1pfmetSumEt;
		treeDipho_.calomet = calomet;
		treeDipho_.calometPhi = calometPhi;
		treeDipho_.calometSumEt = calometSumEt;
		treeDipho_.ptgg = ptgg;
		treeDipho_.mgg = mgg;
		treeDipho_.eventClass = eventClass;
		treeDipho_.pt1 = pt1;
		treeDipho_.ptOverM1 = ptOverM1;
		treeDipho_.eta1 = eta1;
		treeDipho_.phi1 = phi1;
		treeDipho_.sceta1 = sceta1;
		treeDipho_.r91 = r91;
		treeDipho_.sieie1 = sieie1;
		treeDipho_.hoe1 = hoe1; 
		treeDipho_.scRawEne1 = scRawEne1;
		treeDipho_.chiso1 = chiso1; 
		treeDipho_.phoiso1 = phoiso1; 
		treeDipho_.neuiso1 = neuiso1;
		treeDipho_.eleveto1 = eleveto1;
		treeDipho_.pt2 = pt2;
		treeDipho_.ptOverM2 = ptOverM2;
		treeDipho_.eta2 = eta2;
		treeDipho_.phi2 = phi2;
		treeDipho_.sceta2 = sceta2;
		treeDipho_.r92 = r92;
		treeDipho_.sieie2 = sieie2;
		treeDipho_.hoe2 = hoe2; 
		treeDipho_.scRawEne2 = scRawEne2;
		treeDipho_.chiso2 = chiso2; 
		treeDipho_.phoiso2 = phoiso2; 
		treeDipho_.neuiso2 = neuiso2;
		treeDipho_.eleveto2 = eleveto2;
		treeDipho_.presel1 = presel1;
		treeDipho_.presel2 = presel2;
		treeDipho_.sel1 = sel1;
		treeDipho_.sel2 = sel2;
		treeDipho_.vtxIndex = vtxIndex;
		treeDipho_.vtxX = vtxX;
		treeDipho_.vtxY = vtxY;
		treeDipho_.vtxZ = vtxZ;
		treeDipho_.genmatch1 = genmatch1; 
		treeDipho_.genmatch2 = genmatch2; 
		treeDipho_.genmgg  = genmgg;        // -999: not enough gen level gamma; -1999: strange association with reco
		treeDipho_.geniso1 = geniso1; 
		treeDipho_.geniso2 = geniso2; 
		treeDipho_.genVtxX = genVtxX;
		treeDipho_.genVtxY = genVtxY;
		treeDipho_.genVtxZ = genVtxZ;
		treeDipho_.passCHiso1 = passCHiso1;
		treeDipho_.passCHiso2 = passCHiso2;
		treeDipho_.passNHiso1 = passNHiso1;
		treeDipho_.passNHiso2 = passNHiso2;
		treeDipho_.passPHiso1 = passPHiso1;
		treeDipho_.passPHiso2 = passPHiso2;
		treeDipho_.passSieie1 = passSieie1;
		treeDipho_.passSieie2 = passSieie2;
		treeDipho_.passHoe1 = passHoe1;
		treeDipho_.passHoe2 = passHoe2;	
	
		// Filling the trees
		DiPhotonTree->Fill();
		
	      } // good vertex found
	    }   // final diphoton candidate found
	  }     // diphoton candidate passing mass cuts
	}       // diphoton candidate passing pT cuts
      }       // diphoton candidate passing ID+iso
    }         // diphoton candidate passing preselection
  }           // at least 1 reco diphoton candidate  

  // delete
  //delete lazyToolnoZS;
}

void DiPhoAnalyzer::beginJob() {

  // loading weights for pileup if needed
  if (dopureweight_) 
    SetPuWeights(puWFileName_);
  
  // to keep track of the original number of events
  h_entries = fs_->make<TH1F>("h_entries", "h_entries", 10,  0., 10.);
  h_entries->Sumw2();

  // to keep track of the sum of weights
  h_sumW = fs_->make<TH1F>("h_sumW", "h_sumW", 10,  0., 10.);
  h_sumW->Sumw2();
  isFilled = false;
  
  // for the event breakdown
  h_selection = fs_->make<TH1F>("h_selection", "h_selection", 6, -0.5, 5.5);
  h_selection->Sumw2();

  // Trees
  DiPhotonTree = fs_->make<TTree>("DiPhotonTree","di-photon tree");

  // with all infos
  DiPhotonTree->Branch("hltPhoton26Photon16Mass60",&(treeDipho_.hltPhoton26Photon16Mass60),"hltPhoton26Photon16Mass60/I");
  DiPhotonTree->Branch("hltPhoton36Photon22Mass15",&(treeDipho_.hltPhoton36Photon22Mass15),"hltPhoton36Photon22Mass15/I");
  DiPhotonTree->Branch("hltPhoton42Photon25Mass15",&(treeDipho_.hltPhoton42Photon25Mass15),"hltPhoton42Photon25Mass15/I");
  DiPhotonTree->Branch("hltDiphoton30Mass95",&(treeDipho_.hltDiphoton30Mass95),"hltDiphoton30Mass95/I");
  DiPhotonTree->Branch("hltDiphoton30Mass70",&(treeDipho_.hltDiphoton30Mass70),"hltDiphoton30Mass70/I");
  DiPhotonTree->Branch("hltDiphoton30Mass55",&(treeDipho_.hltDiphoton30Mass55),"hltDiphoton30Mass55/I");
  DiPhotonTree->Branch("hltDiphoton30Mass55PV",&(treeDipho_.hltDiphoton30Mass55PV),"hltDiphoton30Mass55PV/I");
  DiPhotonTree->Branch("hltDiphoton30Mass55EB",&(treeDipho_.hltDiphoton30Mass55EB),"hltDiphoton30Mass55EB/I");

  DiPhotonTree->Branch("run",&(treeDipho_.run),"run/I");
  DiPhotonTree->Branch("event",&(treeDipho_.event),"event/I");
  DiPhotonTree->Branch("lumi",&(treeDipho_.lumi),"lumi/I");
  DiPhotonTree->Branch("nvtx",&(treeDipho_.nvtx),"nvtx/I");
  DiPhotonTree->Branch("rho",&(treeDipho_.rho),"rho/F");
  DiPhotonTree->Branch("sampleID",&(treeDipho_.sampleID),"sampleID/I");
  DiPhotonTree->Branch("totXsec",&(treeDipho_.totXsec),"totXsec/F");
  DiPhotonTree->Branch("pu_weight",&(treeDipho_.pu_weight),"pu_weight/F");
  DiPhotonTree->Branch("pu_n",&(treeDipho_.pu_n),"pu_n/F");
  DiPhotonTree->Branch("sumDataset",&(treeDipho_.sumDataset),"sumDataset/F");
  DiPhotonTree->Branch("perEveW",&(treeDipho_.perEveW),"perEveW/F");
  DiPhotonTree->Branch("pfmet",&(treeDipho_.pfmet),"pfmet/F");
  DiPhotonTree->Branch("pfmetPhi",&(treeDipho_.pfmetPhi),"pfmetPhi/F");
  DiPhotonTree->Branch("pfmetSumEt",&(treeDipho_.pfmetSumEt),"pfmetSumEt/F");
  DiPhotonTree->Branch("t1pfmet",&(treeDipho_.t1pfmet),"t1pfmet/F");
  DiPhotonTree->Branch("t1pfmetPhi",&(treeDipho_.t1pfmetPhi),"t1pfmetPhi/F");
  DiPhotonTree->Branch("t1pfmetSumEt",&(treeDipho_.t1pfmetSumEt),"t1pfmetSumEt/F");
  DiPhotonTree->Branch("calomet",&(treeDipho_.calomet),"calomet/F");
  DiPhotonTree->Branch("calometPhi",&(treeDipho_.calometPhi),"calometPhi/F");
  DiPhotonTree->Branch("calometSumEt",&(treeDipho_.calometSumEt),"calometSumEt/F");
  DiPhotonTree->Branch("ptgg",&(treeDipho_.ptgg),"ptgg/F");
  DiPhotonTree->Branch("mgg",&(treeDipho_.mgg),"mgg/F");
  DiPhotonTree->Branch("eventClass",&(treeDipho_.eventClass),"eventClass/I");
  DiPhotonTree->Branch("pt1",&(treeDipho_.pt1),"pt1/F");
  DiPhotonTree->Branch("ptOverM1",&(treeDipho_.ptOverM1),"ptOverM1/F");
  DiPhotonTree->Branch("eta1",&(treeDipho_.eta1),"eta1/F");
  DiPhotonTree->Branch("phi1",&(treeDipho_.phi1),"phi1/F");
  DiPhotonTree->Branch("sceta1",&(treeDipho_.sceta1),"sceta1/F");
  DiPhotonTree->Branch("r91",&(treeDipho_.r91),"r91/F");
  DiPhotonTree->Branch("sieie1",&(treeDipho_.sieie1),"sieie1/F");
  DiPhotonTree->Branch("hoe1",&(treeDipho_.hoe1),"hoe1/F");
  DiPhotonTree->Branch("scRawEne1",&(treeDipho_.scRawEne1),"scRawEne1/F");
  DiPhotonTree->Branch("chiso1",&(treeDipho_.chiso1),"chiso1/F");
  DiPhotonTree->Branch("phoiso1",&(treeDipho_.phoiso1),"phoiso1/F");
  DiPhotonTree->Branch("neuiso1",&(treeDipho_.neuiso1),"neuiso1/F");
  DiPhotonTree->Branch("eleveto1",&(treeDipho_.eleveto1),"eleveto1/I");
  DiPhotonTree->Branch("pt2",&(treeDipho_.pt2),"pt2/F");
  DiPhotonTree->Branch("ptOverM2",&(treeDipho_.ptOverM2),"ptOverM2/F");
  DiPhotonTree->Branch("eta2",&(treeDipho_.eta2),"eta2/F");
  DiPhotonTree->Branch("phi2",&(treeDipho_.phi2),"phi2/F");
  DiPhotonTree->Branch("sceta2",&(treeDipho_.sceta2),"sceta2/F");
  DiPhotonTree->Branch("r92",&(treeDipho_.r92),"r92/F");
  DiPhotonTree->Branch("sieie2",&(treeDipho_.sieie2),"sieie2/F");
  DiPhotonTree->Branch("hoe2",&(treeDipho_.hoe2),"hoe2/F");
  DiPhotonTree->Branch("scRawEne2",&(treeDipho_.scRawEne2),"scRawEne2/F");
  DiPhotonTree->Branch("chiso2",&(treeDipho_.chiso2),"chiso2/F");
  DiPhotonTree->Branch("phoiso2",&(treeDipho_.phoiso2),"phoiso2/F");
  DiPhotonTree->Branch("neuiso2",&(treeDipho_.neuiso2),"neuiso2/F");
  DiPhotonTree->Branch("eleveto2",&(treeDipho_.eleveto2),"eleveto2/I");
  DiPhotonTree->Branch("presel1",&(treeDipho_.presel1),"presel1/I");
  DiPhotonTree->Branch("presel2",&(treeDipho_.presel2),"presel2/I");
  DiPhotonTree->Branch("sel1",&(treeDipho_.sel1),"sel1/I");
  DiPhotonTree->Branch("sel2",&(treeDipho_.sel2),"sel2/I");
  DiPhotonTree->Branch("genmatch1",&(treeDipho_.genmatch1),"genmatch1/I");
  DiPhotonTree->Branch("genmatch2",&(treeDipho_.genmatch2),"genmatch12/I");
  DiPhotonTree->Branch("genmgg",&(treeDipho_.genmgg),"genmgg/F");
  DiPhotonTree->Branch("geniso1",&(treeDipho_.geniso1),"geniso1/F");
  DiPhotonTree->Branch("geniso2",&(treeDipho_.geniso2),"geniso2/F");
  DiPhotonTree->Branch("vtxIndex",&(treeDipho_.vtxIndex),"vtxIndex/I");
  DiPhotonTree->Branch("vtxX",&(treeDipho_.vtxX),"vtxX/F");
  DiPhotonTree->Branch("vtxY",&(treeDipho_.vtxY),"vtxY/F");
  DiPhotonTree->Branch("vtxZ",&(treeDipho_.vtxZ),"vtxZ/F");
  DiPhotonTree->Branch("genVtxX",&(treeDipho_.genVtxX),"genVtxX/F");
  DiPhotonTree->Branch("genVtxY",&(treeDipho_.genVtxY),"genVtxY/F");
  DiPhotonTree->Branch("genVtxZ",&(treeDipho_.genVtxZ),"genVtxZ/F");
  DiPhotonTree->Branch("passCHiso1",&(treeDipho_.passCHiso1),"passCHiso1/I");
  DiPhotonTree->Branch("passCHiso2",&(treeDipho_.passCHiso2),"passCHiso2/I");
  DiPhotonTree->Branch("passNHiso1",&(treeDipho_.passNHiso1),"passNHiso1/I");
  DiPhotonTree->Branch("passNHiso2",&(treeDipho_.passNHiso2),"passNHiso2/I");
  DiPhotonTree->Branch("passPHiso1",&(treeDipho_.passPHiso1),"passPHiso1/I");
  DiPhotonTree->Branch("passPHiso2",&(treeDipho_.passPHiso2),"passPHiso2/I");
  DiPhotonTree->Branch("passSieie1",&(treeDipho_.passSieie1),"passSieie1/I");
  DiPhotonTree->Branch("passSieie2",&(treeDipho_.passSieie2),"passSieie2/I");
  DiPhotonTree->Branch("passHoe1",&(treeDipho_.passHoe1),"passHoe1/I");
  DiPhotonTree->Branch("passHoe2",&(treeDipho_.passHoe2),"passHoe2/I");
}

void DiPhoAnalyzer::endJob() { }

void DiPhoAnalyzer::initTreeStructure() {
  treeDipho_.hltPhoton26Photon16Mass60=-500;
  treeDipho_.hltPhoton36Photon22Mass15=-500;
  treeDipho_.hltPhoton42Photon25Mass15=-500;
  treeDipho_.hltDiphoton30Mass95=-500;   
  treeDipho_.hltDiphoton30Mass70=-500;   
  treeDipho_.hltDiphoton30Mass55=-500; 
  treeDipho_.hltDiphoton30Mass55PV=-500; 
  treeDipho_.hltDiphoton30Mass55EB=-500; 
 
  treeDipho_.run   = -500;
  treeDipho_.event = -500;
  treeDipho_.lumi  = -500;
  treeDipho_.nvtx  = -500;
  treeDipho_.rho   = -500.;
  treeDipho_.sampleID  = -500;
  treeDipho_.totXsec   = -500.;
  treeDipho_.pu_weight = -500.; 
  treeDipho_.pu_n = -500.;
  treeDipho_.sumDataset = -500.;
  treeDipho_.perEveW = -500.;
  treeDipho_.pfmet = -500.;
  treeDipho_.pfmetPhi = -500.;
  treeDipho_.pfmetSumEt = -500.;
  treeDipho_.t1pfmet = -500.;
  treeDipho_.t1pfmetPhi = -500.;
  treeDipho_.t1pfmetSumEt = -500.;
  treeDipho_.calomet = -500.;
  treeDipho_.calometPhi = -500.;
  treeDipho_.calometSumEt = -500.;
  treeDipho_.ptgg = -500.;
  treeDipho_.mgg  = -500.;
  treeDipho_.eventClass  = -500;
  treeDipho_.pt1  = -500.;
  treeDipho_.ptOverM1 = -500.;
  treeDipho_.eta1 = -500.;
  treeDipho_.phi1 = -500.;
  treeDipho_.sceta1 = -500.;
  treeDipho_.r91  = -500.;
  treeDipho_.sieie1 = -500.;
  treeDipho_.hoe1   = -500.;
  treeDipho_.scRawEne1 = -500.;
  treeDipho_.chiso1  = -500.;
  treeDipho_.phoiso1 = -500.;
  treeDipho_.neuiso1 = -500.;
  treeDipho_.eleveto1 = -500;
  treeDipho_.pt2  = -500.;
  treeDipho_.ptOverM2 = -500.;
  treeDipho_.eta2 = -500.;
  treeDipho_.phi2 = -500.;
  treeDipho_.sceta2 = -500.;
  treeDipho_.r92  = -500.;
  treeDipho_.sieie2 = -500.;
  treeDipho_.hoe2   = -500.;
  treeDipho_.scRawEne2 = -500.;
  treeDipho_.chiso2  = -500.;
  treeDipho_.phoiso2 = -500.;
  treeDipho_.neuiso2 = -500.;
  treeDipho_.eleveto2 = -500;
  treeDipho_.presel1 = -500;
  treeDipho_.presel2 = -500;
  treeDipho_.sel1 = -500;
  treeDipho_.sel2 = -500;
  treeDipho_.vtxIndex = -500;
  treeDipho_.vtxX = -500.;
  treeDipho_.vtxY = -500.;
  treeDipho_.vtxZ = -500.;
  treeDipho_.genmatch1 = -500;
  treeDipho_.genmatch2 = -500;
  treeDipho_.genmgg  = -500.;
  treeDipho_.geniso1 = -500.;
  treeDipho_.geniso2 = -500.;
  treeDipho_.genVtxX = -500.;
  treeDipho_.genVtxY = -500.;
  treeDipho_.genVtxZ = -500.;
  treeDipho_.passCHiso1 = -500;
  treeDipho_.passCHiso2 = -500;
  treeDipho_.passNHiso1 = -500;
  treeDipho_.passNHiso2 = -500;
  treeDipho_.passPHiso1 = -500;
  treeDipho_.passPHiso2 = -500;
  treeDipho_.passSieie1 = -500;
  treeDipho_.passSieie2 = -500;
  treeDipho_.passHoe1 = -500;
  treeDipho_.passHoe2 = -500;
}

void DiPhoAnalyzer::SetPuWeights(std::string puWeightFile) {

  if (puWeightFile == "") {
    std::cout << "you need a weights file to use this function" << std::endl;
    return;
  }
  std::cout << "PU REWEIGHTING:: Using file " << puWeightFile << std::endl;

  TFile *f_pu  = new TFile(puWeightFile.c_str(),"READ");
  f_pu->cd();

  TH1D *puweights = 0;
  // TH1D *gen_pu = 0;
  //gen_pu    = (TH1D*) f_pu->Get("generated_pu");
  //puweights = (TH1D*) f_pu->Get("weights");
  puweights = (TH1D*) f_pu->Get("puhist");

  if (!puweights /*|| !gen_pu*/) {
    std::cout << "weights histograms  not found in file " << puWeightFile << std::endl;
    return;
  }
  //TH1D* weightedPU= (TH1D*)gen_pu->Clone("weightedPU");
  //  weightedPU->Multiply(puweights);

  // Rescaling weights in order to preserve same integral of events                               
  //TH1D* weights = (TH1D*)puweights->Clone("rescaledWeights");
  //weights->Scale( gen_pu->Integral(1,MAX_PU_REWEIGHT) / weightedPU->Integral(1,MAX_PU_REWEIGHT) );

  float sumPuWeights=0.;
  for (int i = 0; i<MAX_PU_REWEIGHT; i++) {
    float weight=1.;
    weight=puweights->GetBinContent(i+1);
    sumPuWeights+=weight;
    puweights_.push_back(weight);
  }
}

float DiPhoAnalyzer::GetPUWeight(float pun) {
  
  float weight=1;
  if (sampleIndex_!=0 && pun<MAX_PU_REWEIGHT && puweights_.size()>0 && dopureweight_) 
    weight = puweights_[pun];
  return weight;
}

// miniAOD preselection + ECAL acceptance
bool DiPhoAnalyzer::isGammaPresel( float sceta, float pt, float r9, float chiso) {

  bool isPresel = false;

  // ECAL good acceptance
  if (fabs(sceta)>2.5) return false;
  if (fabs(sceta)>1.4442 && fabs(sceta)<1.566) return false;
  
  // miniAOD preselection
  if (r9>0.8)         return true;
  if (chiso<20)       return true;
  if ((chiso/pt)<0.3) return true;
  
  return isPresel;
}

double DiPhoAnalyzer::getChargedHadronEAForPhotonIso(float eta) {
if (fabs(eta) < 1.0) return 0.0158;
else if (fabs(eta) >= 1.0 && fabs(eta) < 1.479) return 0.0143;
else if (fabs(eta) >= 1.479 && fabs(eta) < 2.0 ) return 0.0115;
else if (fabs(eta) >= 2.0 && fabs(eta) < 2.2 ) return 0.0094;
else if (fabs(eta) >= 2.2 && fabs(eta) < 2.3 ) return 0.0095;
else if (fabs(eta) >= 2.3 && fabs(eta) < 2.4 ) return 0.0068;
else if (fabs(eta) >= 2.4) return 0.0053;
else return 0.;
}
double DiPhoAnalyzer::getNeutralHadronEAForPhotonIso(float eta) {
if (fabs(eta) < 1.0) return 0.0143;
else if (fabs(eta) >= 1.0 && fabs(eta) < 1.479) return 0.0210;
else if (fabs(eta) >= 1.479 && fabs(eta) < 2.0 ) return 0.0147;
else if (fabs(eta) >= 2.0 && fabs(eta) < 2.2 ) return 0.0082;
else if (fabs(eta) >= 2.2 && fabs(eta) < 2.3 ) return 0.0124;
else if (fabs(eta) >= 2.3 && fabs(eta) < 2.4 ) return 0.0186;
else if (fabs(eta) >= 2.4) return 0.0320;
else return 0.;
}

double DiPhoAnalyzer::getGammaEAForPhotonIso(float eta) {
if (fabs(eta) < 1.0) return 0.0725;
else if (fabs(eta) >= 1.0 && fabs(eta) < 1.479) return 0.0604;
else if (fabs(eta) >= 1.479 && fabs(eta) < 2.0 ) return 0.0320;
else if (fabs(eta) >= 2.0 && fabs(eta) < 2.2 ) return 0.0512;
else if (fabs(eta) >= 2.2 && fabs(eta) < 2.3 ) return 0.0766;
else if (fabs(eta) >= 2.3 && fabs(eta) < 2.4 ) return 0.0949;
else if (fabs(eta) >= 2.4) return 0.1160;
else return 0.;
}
//Livia CorrectionL write beeter the photon ID cuts
int DiPhoAnalyzer::passSieieCuts(float sceta, float sieie){
  int passes = 1;
  if (fabs(sceta)<1.4442 && sieie>0.0100) passes = 0;
  if ((fabs(sceta)>1.566 && fabs(sceta)<2.5) && sieie>0.0267) passes = 0;
  return passes;
}
int DiPhoAnalyzer::passCHisoCuts(float sceta, float chiso, float pt){
  int passes = 1;
  if (fabs(sceta)<1.4442 && chiso>1.31) passes = 0;
  if ((fabs(sceta)>1.566 && fabs(sceta)<2.5) && chiso>1.25) passes = 0;
  return passes;
}
int DiPhoAnalyzer::passNHisoCuts(float sceta, float nhiso, float pt){
  int passes = 1;
  if (fabs(sceta)<1.4442 && nhiso > 0.60+exp(0.0044*pt+0.5809)) passes = 0;
  if ((fabs(sceta)>1.566 && fabs(sceta)<2.5) &&  nhiso > 1.65 + exp(0.0040*pt+0.9402)) passes = 0;
  return passes;
}
int DiPhoAnalyzer::passPHisoCuts(float sceta, float phiso, float pt){
  int passes = 1;
  if (fabs(sceta)<1.4442 && phiso > 1.33+0.0043*pt) passes = 0;
  if ((fabs(sceta)>1.566 && fabs(sceta)<2.5) &&  phiso > 1.02+0.0041*pt) passes = 0;
  return passes;
}
int DiPhoAnalyzer::passHoeCuts(float sceta, float hoe){
  int passes = 1;
  if (fabs(sceta)<1.4442 && hoe>0.05) passes = 0;
  if ((fabs(sceta)>1.566 && fabs(sceta)<2.5) &&hoe>0.05) passes = 0;
  return passes;
}

bool DiPhoAnalyzer::testPhotonIsolation(int passSieie, int passCHiso, int passNHiso, int passPHiso, int passHoe){
  if (passSieie == 1 && passHoe == 1 && passCHiso == 1 && passNHiso == 1 && passPHiso == 1) return true; //passes all selection
  else return false;
}

/*
  bool DiPhoAnalyzer::testPhotonIsolation(float rho,float pt, float sceta, float r9, float chiso, float nhiso, float phoiso , float hoe, float sieie, bool passElectronVeto) {
  double corrCHIso = chiso;
  double corrNHIso = nhiso;
  double corrPHIso = phoiso;

 if (fabs(sceta)<1.479) {
   if (corrCHIso > 1.79) return false;
   if (corrNHIso > 0.16+exp(0.0028*pt+0.5408)) return false;
   if (corrPHIso > 1.9+0.0014*pt) return false;
   if (sieie>0.010) return false;
   if (hoe>0.012) return false;
 }
 else {
   if (corrCHIso > 1.09) return false;
   if (corrNHIso > 4.31+ 0.0172*pt) return false;
   if (corrPHIso > 1.90+0.0091*pt) return false;
   if (sieie>0.0267) return false;
   if (hoe>0.023) return false;
 }
 
 // electron veto 
//  if (!passElectronVeto) return false;//livia
 return true;
} */




bool DiPhoAnalyzer::isGammaSelected( float rho, float pt, float sceta, float r9, float chiso, float nhiso, float phoiso, float hoe, float sieie, bool passElectronVeto) {
  std::cout<<rho<<" "<<pt<<" "<<sceta<<" "<<r9<<" "<<chiso<<" "<<nhiso<<" "<<phoiso<<" "<<hoe<<" "<<sieie<<" "<<passElectronVeto<<std::endl;
  // classes: 0 = EB highR9, 1 = EB low R9, 2 = EE high R9, 3 = EE lowR9
  int etaclass = fabs(sceta)>1.5;
  int r9class  = r9<0.94;                   
  int theclass = 2.*etaclass + r9class;                  

  // cuts - hardcoded
  float chiso_cut[4]  = { 5.95, 7.08, 6.10, 5.07 };     
  float phoiso_cut[4] = { 2.87, 5.47, 5.98, 3.44 };  
  // float nhiso_cut[4]  = { 27.4, 30.0, 30.0, 15.0 };  
  float sieie_cut[4]  = { 1.05e-02, 1.05e-02, 2.82e-02, 2.8e-02 };
  float hoe_cut[4]    = { 4.53e-01, 2.12e-01, 6.3e-02, 7.8e-02 };
  
  // effective areas - hardcoded 
  float chIsoAE[5] = { 0.00,0.000,0.00,0.00,0.00 };
  float phIsoAE[5] = { 0.21,0.200,0.14,0.22,0.31 };
  // float nhIsoAE[5] = { 0.04,0.059,0.05,0.05,0.15 };

  // EA corrections 
  int theEAregion = effectiveAreaRegion(sceta);
  float corrChIso = chiso - rho*chIsoAE[theEAregion];
  float corrPhIso = phoiso - rho*phIsoAE[theEAregion];
  //float corrChIso = std::max(chiso - rho*chIsoAE[theEAregion],0.);
  //float corrPhIso = std::max(phoiso - rho*phIsoAE[theEAregion],0.);
  // float corrNhIso = nhiso - rho*nhIsoAE[theEAregion];   

  if (corrChIso > chiso_cut[theclass])  return false;
  if (corrPhIso > phoiso_cut[theclass]) return false;
  // if (corrNhIso > nhiso_cut[theclass])  return false;
  if (sieie > sieie_cut[theclass])      return false;
  if (hoe> hoe_cut[theclass])           return false;

  // electron veto 
//  if (!passElectronVeto) return false;//livia

  return true;
} 

int DiPhoAnalyzer::effectiveAreaRegion(float sceta) {

  int theEAregion = 999;
  if (fabs(sceta)<=0.9) theEAregion = 0;
  if (fabs(sceta)<=1.5 && fabs(sceta)>0.9)  theEAregion = 1;
  if (fabs(sceta)<=2.0 && fabs(sceta)>1.5)  theEAregion = 2;
  if (fabs(sceta)<=2.2 && fabs(sceta)>2.0)  theEAregion = 3;
  if (fabs(sceta)<=2.5 && fabs(sceta)>2.2)  theEAregion = 4;
  return theEAregion;
}


DEFINE_FWK_MODULE(DiPhoAnalyzer);
