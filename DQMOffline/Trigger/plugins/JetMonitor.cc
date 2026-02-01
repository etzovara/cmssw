#include <string>
#include <vector>

#include "TLorentzVector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMOffline/Trigger/plugins/TriggerDQMBase.h"
#include "CommonTools/TriggerUtils/interface/GenericTriggerEventFlag.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Scouting/interface/Run3ScoutingPFJet.h"
#include "DataFormats/Scouting/interface/Run3ScoutingMuon.h"
#include "DataFormats/Scouting/interface/Run3ScoutingVertex.h"
#include "DataFormats/Scouting/interface/Run3ScoutingTrack.h"
#include "DataFormats/Math/interface/deltaR.h"
/*#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
*/
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"

#include "PhysicsTools/SelectorUtils/interface/Run3ScoutingPFJetIDSelectionFunctor.h"

class JetMonitor : public DQMEDAnalyzer, public TriggerDQMBase {
public:
  typedef dqm::reco::MonitorElement MonitorElement;
  typedef dqm::reco::DQMStore DQMStore;

  JetMonitor(const edm::ParameterSet&);
  ~JetMonitor() throw() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

protected:
  void bookHistograms(DQMStore::IBooker&, edm::Run const&, edm::EventSetup const&) override;
  void analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) override;

  bool isGoodScoutingMuon(Run3ScoutingMuon const &scoutingMuon);
  bool isCleanScoutingJet(double ScoutingJetEta, double ScoutingJetPhi, const std::vector<Run3ScoutingMuon>& scoutingMuons, double Scoutingdr2Cut); // or just 1 vector?????
  bool isBarrel(double eta);
  bool isEndCapP(double eta);
  bool isEndCapM(double eta);
  bool isForward(double eta);

  void bookMESub(DQMStore::IBooker&,
                 ObjME* a_me,
                 const int len_,
                 const std::string& h_Name,
                 const std::string& h_Title,
                 const std::string& h_subOptName,
                 const std::string& h_subOptTitle,
                 const bool doPhi = true,
                 const bool doEta = true,
                 const bool doEtaPhi = true,
                 const bool doVsLS = true);
  void FillME(ObjME* a_me,
              const double pt_,
              const double phi_,
              const double eta_,
              const int ls_,
              const std::string& denu,
              const bool doPhi = true,
              const bool doEta = true,
              const bool doEtaPhi = true,
              const bool doVsLS = true);

private:
  const std::string folderName_;

  const bool requireValidHLTPaths_;
  bool hltPathsAreValid_;

  double ptcut_;
  bool isPFJetTrig;
  bool isCaloJetTrig;
  bool isScoutingPFJetTrig;
  bool isScoutingPFJet;

  int verbose_;
  std::string JetIDQuality_;
  std::string JetIDVersion_;
  Run3ScoutingPFJetIDSelectionFunctor::Quality_t run3scoutingpfjetidquality;
  Run3ScoutingPFJetIDSelectionFunctor::Version_t run3scoutingpfjetidversion;
  Run3ScoutingPFJetIDSelectionFunctor run3scoutingpfjetIDFunctor;
  
  const bool enableFullMonitoring_;
  
///// First define inputTags, then tokens ---> should follow the same sequence also below !!!!!

  edm::InputTag scoutingMuonInputTag_;
  edm::InputTag scoutingVtxInputTag_;
  
  ///// !!!! THere's no point in using edm::View< > class , since e.g. Run3ScoutingPFJet is a simple standalone struct. It does not inherit from a base class. In general, there are no other classes that inherit from Run3ScoutingPFJet.
  /*edm::EDGetTokenT<edm::View<Run3ScoutingMuon> > scoutingMuonToken_;
  edm::EDGetTokenT<edm::View<Run3ScoutingVertex> > scoutingVtxToken_;
  edm::EDGetTokenT<edm::View<Run3ScoutingPFJet> > scoutjetSrc_;*/
  //// or the following???? (if so, change the respective lines further below, accordingly)  
  edm::EDGetTokenT<std::vector<Run3ScoutingMuon> > scoutingMuonToken_;
  edm::EDGetTokenT<std::vector<Run3ScoutingVertex> > scoutingVtxToken_;
  edm::EDGetTokenT<std::vector<Run3ScoutingPFJet> > scoutjetSrc_;
  edm::EDGetTokenT<reco::PFJetCollection> jetSrc_; 
  edm::EDGetTokenT<reco::JetCorrector> correctorToken_;
  //const edm::EDGetTokenT<reco::JetCorrector> correctorToken_;
  edm::EDGetTokenT<reco::CaloJetCollection> calojetToken_;
  //const edm::EDGetTokenT<reco::CaloJetCollection> calojetToken_;
  
  std::unique_ptr<GenericTriggerEventFlag> num_genTriggerEventFlag_;
  std::unique_ptr<GenericTriggerEventFlag> den_genTriggerEventFlag_;
  
  
  unsigned nscoutingmuons_;

  MEbinning jetpt_binning_;
  MEbinning jetptThr_binning_;
  MEbinning ls_binning_;

  ObjME a_ME[7];
  ObjME a_ME_HB[7];
  ObjME a_ME_HE[7];
  ObjME a_ME_HF[7];
  ObjME a_ME_HE_p[7];
  ObjME a_ME_HE_m[7];

  /*std::vector<double> v_jetpt;
  std::vector<double> v_jeteta;
  std::vector<double> v_jetphi;*/
  struct Jets {
    double pt;
    double eta;
    double phi;
  };
  std::vector<Jets> selected_jets;  //keep jets that pass requirements


  // (mia) not optimal, we should make use of variable binning which reflects the detector !
  MEbinning jet_phi_binning_{64, -3.2, 3.2};
  MEbinning jet_eta_binning_{50, -5, 5};
};

JetMonitor::JetMonitor(const edm::ParameterSet& iConfig)
    : folderName_(iConfig.getParameter<std::string>("FolderName")),
      requireValidHLTPaths_(iConfig.getParameter<bool>("requireValidHLTPaths")),
      hltPathsAreValid_(false),
      ptcut_(iConfig.getParameter<double>("ptcut")),
      isPFJetTrig(iConfig.getParameter<bool>("ispfjettrg")),
      isCaloJetTrig(iConfig.getParameter<bool>("iscalojettrg")),
      isScoutingPFJetTrig(iConfig.getParameter<bool>("isscoutingpfjettrg")),
      isScoutingPFJet(iConfig.getParameter<bool>("isscoutingpfjet")),
      JetIDQuality_(iConfig.getParameter<std::string>("JetIDQuality")),
      JetIDVersion_(iConfig.getParameter<std::string>("JetIDVersion")),
      enableFullMonitoring_(iConfig.getParameter<bool>("enableFullMonitoring")),
      scoutingMuonInputTag_(iConfig.getParameter<edm::InputTag>("muons")),
      scoutingVtxInputTag_(iConfig.getParameter<edm::InputTag>("vertices")),
      /*scoutingMuonToken_(mayConsume<edm::View<Run3ScoutingMuon> >(scoutingMuonInputTag_)),
      scoutingVtxToken_(mayConsume<edm::View<Run3ScoutingVertex> >(scoutingVtxInputTag_)),
      scoutjetSrc_(mayConsume<edm::View<Run3ScoutingPFJet> >(iConfig.getParameter<edm::InputTag>("jetSrc"))),*/
      scoutingMuonToken_(mayConsume<std::vector<Run3ScoutingMuon> >(scoutingMuonInputTag_)),
      scoutingVtxToken_(mayConsume<std::vector<Run3ScoutingVertex> >(scoutingVtxInputTag_)),
      scoutjetSrc_(mayConsume<std::vector<Run3ScoutingPFJet> >(iConfig.getParameter<edm::InputTag>("jetSrc"))),
      ////jetSrc_(mayConsume<edm::View<reco::Jet> >(iConfig.getParameter<edm::InputTag>("jetSrc"))),
      jetSrc_(mayConsume<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("jetSrc"))),
      correctorToken_(mayConsume<reco::JetCorrector>(iConfig.getParameter<edm::InputTag>("corrector"))),
      calojetToken_(mayConsume<reco::CaloJetCollection>(iConfig.getParameter<edm::InputTag>("jetSrc"))),
      num_genTriggerEventFlag_(new GenericTriggerEventFlag(
          iConfig.getParameter<edm::ParameterSet>("numGenericTriggerEventPSet"), consumesCollector(), *this)),
      den_genTriggerEventFlag_(new GenericTriggerEventFlag(
          iConfig.getParameter<edm::ParameterSet>("denGenericTriggerEventPSet"), consumesCollector(), *this)),
      nscoutingmuons_(iConfig.getParameter<unsigned>("nmuons")),
      jetpt_binning_(getHistoPSet(
          iConfig.getParameter<edm::ParameterSet>("histoPSet").getParameter<edm::ParameterSet>("jetPSet"))),
      jetptThr_binning_(getHistoPSet(
          iConfig.getParameter<edm::ParameterSet>("histoPSet").getParameter<edm::ParameterSet>("jetPtThrPSet"))),
      ls_binning_(getHistoPSet(
          iConfig.getParameter<edm::ParameterSet>("histoPSet").getParameter<edm::ParameterSet>("lsPSet"))) {}

JetMonitor::~JetMonitor() throw() {
  if (num_genTriggerEventFlag_) {
    num_genTriggerEventFlag_.reset();
  }
  if (den_genTriggerEventFlag_) {
    den_genTriggerEventFlag_.reset();
  }
}


void JetMonitor::bookHistograms(DQMStore::IBooker& ibooker, edm::Run const& iRun, edm::EventSetup const& iSetup) {
  // Initialize the GenericTriggerEventFlag
  if (num_genTriggerEventFlag_ && num_genTriggerEventFlag_->on()) {
    num_genTriggerEventFlag_->initRun(iRun, iSetup);
  }
  if (den_genTriggerEventFlag_ && den_genTriggerEventFlag_->on()) {
    den_genTriggerEventFlag_->initRun(iRun, iSetup);
  }

  // check if every HLT path specified in numerator and denominator has a valid match in the HLT Menu
  hltPathsAreValid_ = (num_genTriggerEventFlag_ && den_genTriggerEventFlag_ && num_genTriggerEventFlag_->on() &&
                       den_genTriggerEventFlag_->on() && num_genTriggerEventFlag_->allHLTPathsAreValid() &&
                       den_genTriggerEventFlag_->allHLTPathsAreValid());

  // if valid HLT paths are required,
  // create DQM outputs only if all paths are valid
  if (requireValidHLTPaths_ and (not hltPathsAreValid_)) {
    return;
  }

  std::string histname, histtitle;
  std::string hist_obtag = "";
  std::string histtitle_obtag = "";
  std::string currentFolder = folderName_;
  ibooker.setCurrentFolder(currentFolder);

  if (isPFJetTrig) { // || isScoutingPFJetTrig
    hist_obtag = "pfjet";
    histtitle_obtag = "PFJet";
  } else if (isCaloJetTrig) {
    hist_obtag = "calojet";
    histtitle_obtag = "CaloJet";
  } else if (isScoutingPFJetTrig) {
    hist_obtag = "scoutingpfjet";
    histtitle_obtag = "ScoutingPfJet";
  } else {
    hist_obtag = "pfjet";
    histtitle_obtag = "PFJet";
  }  //default is pfjet

  bookMESub(ibooker, a_ME, sizeof(a_ME) / sizeof(a_ME[0]), hist_obtag, histtitle_obtag, "", "");
  bookMESub(ibooker,
            a_ME_HB,
            sizeof(a_ME_HB) / sizeof(a_ME_HB[0]),
            hist_obtag,
            histtitle_obtag,
            "HB",
            "(HB)",
            true,
            true,
            true,
            false);
  bookMESub(ibooker,
            a_ME_HE,
            sizeof(a_ME_HE) / sizeof(a_ME_HE[0]),
            hist_obtag,
            histtitle_obtag,
            "HE",
            "(HE)",
            true,
            true,
            true,
            false);
  bookMESub(ibooker,
            a_ME_HF,
            sizeof(a_ME_HF) / sizeof(a_ME_HF[0]),
            hist_obtag,
            histtitle_obtag,
            "HF",
            "(HF)",
            true,
            true,
            true,
            false);

  //check the flag
  if (!enableFullMonitoring_) {
    return;
  }

  bookMESub(ibooker,
            a_ME_HE_p,
            sizeof(a_ME_HE_p) / sizeof(a_ME_HE_p[0]),
            hist_obtag,
            histtitle_obtag,
            "HE_p",
            "(HE+)",
            true,
            false,
            true,
            false);
  bookMESub(ibooker,
            a_ME_HE_m,
            sizeof(a_ME_HE_m) / sizeof(a_ME_HE_m[0]),
            hist_obtag,
            histtitle_obtag,
            "HE_m",
            "(HE-)",
            true,
            false,
            true,
            false);
}

void JetMonitor::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {
  // if valid HLT paths are required,
  // analyze event only if all paths are valid
  if (requireValidHLTPaths_ and (not hltPathsAreValid_)) {
    return;
  }

  // Filter out events if Trigger Filtering is requested
  if (den_genTriggerEventFlag_->on() && !den_genTriggerEventFlag_->accept(iEvent, iSetup))
    return;

  const int ls = iEvent.id().luminosityBlock();

  //----------------- require a tight ID Muon with Pt>30 --------------------
  //--------- we need a vrtx -----------
  /*edm::Handle<std::vector<Run3ScoutingVertex>> ScoutingVertexHandle;
  iEvent.getByToken(scoutingMuonToken_, ScoutingVertexHandle);
  if (ScoutingVertexHandle.failedToGet()) {
    edm::LogWarning("ScoutingMonitoring") << "Run3ScoutingVertex collection not found.";
    return; */
  
  
  // -------- muons ----------
  std::vector<Run3ScoutingMuon> scoutingmuons;
  edm::Handle<std::vector<Run3ScoutingMuon>> ScoutingMuonHandle;
  iEvent.getByToken(scoutingMuonToken_, ScoutingMuonHandle);
  //if (isScoutingPFJetTrig) { /////Is a filter needed to distinguish scouting and offline muons? ---> e.g. if (!isScoutingPFJetTrig) { ...}??? For PF jets probably not nedded, since muons are common for PF and Calo jets  ----> To validate this, should test if offline collections remain intact.
  if (ScoutingMuonHandle.isValid()) {
    if (ScoutingMuonHandle->size() < nscoutingmuons_) {
      //edm::LogWarning("JetMonitor") << "Run3ScoutingMuon collection not valid.";
      return;
    }
    for (auto const& iscoutmuon : *ScoutingMuonHandle) {
      /*std::cout << "scouting muon pT: " << iscoutmuon.pt() << std::endl;
      std::cout << "scouting muon eta: " << iscoutmuon.eta() << std::endl;
      std::cout << "scouting muon trk_dxy: " << iscoutmuon.trk_dxy() << std::endl;
      std::cout << "scouting muon trk_dz: " << iscoutmuon.trk_dz() << std::endl;
      std::cout << "scouting muon normalizedChi2: " << iscoutmuon.normalizedChi2() << std::endl;
      std::cout << "scouting muon nValidRecoMuonHits: " << iscoutmuon.nValidRecoMuonHits() << std::endl;
      std::cout << "scouting muon nRecoMuonMatchedStations: " << iscoutmuon.nRecoMuonMatchedStations() << std::endl;
      std::cout << "scouting muon nValidPixelHits: " << iscoutmuon.nValidPixelHits() << std::endl;
      std::cout << "scouting muon nTrackerLayersWithMeasurement: " << iscoutmuon.nTrackerLayersWithMeasurement() << std::endl;
      std::cout << "scouting muon passes: " << isGoodScoutingMuon(iscoutmuon) << std::endl;*/
                
      if (isGoodScoutingMuon(iscoutmuon)) {
        scoutingmuons.push_back(iscoutmuon);
      }
    }
    if (scoutingmuons.size() < nscoutingmuons_) {
      return;
    }
  } else {
    if (scoutingMuonInputTag_.label().empty()) {
      edm::LogWarning("JetMonitor") << "Scouting muon collection not valid \n";
    }
    else {
      edm::LogWarning("JetMonitor") << "skipping events because the collection " <<  scoutingMuonInputTag_.label().c_str() << " is not available \n";
    }
      return;
  }
  //} 
  
  
  
  //-------------------------------- Jets -----------------------------------------------------
  /*v_jetpt.clear();
  v_jeteta.clear();
  v_jetphi.clear();*/
  selected_jets.clear();

  edm::Handle<std::vector<Run3ScoutingPFJet>> ScoutingJetHandle;
  edm::Handle<reco::PFJetCollection> PFjetHandle;
  edm::Handle<reco::CaloJetCollection> calojetHandle;
  // corrector
  edm::Handle<reco::JetCorrector> Corrector;  
  iEvent.getByToken(correctorToken_, Corrector);
  
   
///////////// New format - unifying collections
  /*
  bool jetCollectionIsValid = false;
  std::string jet_type = "";
  unsigned int collSize = -1;
  
  if (isScoutingPFJetTrig) 
    iEvent.getByToken(scoutjetSrc_, ScoutingJetHandle);
    jetCollectionIsValid = ScoutingJetHandle.isValid();
    jet_type = "Scouting";
  if (isPFJetTrig) 
    iEvent.getByToken(jetSrc_, PFjetHandle);
    jetCollectionIsValid = PFjetHandle.isValid();
    jet_type = "PF";
  if (isCaloJetTrig) 
    iEvent.getByToken(calojetToken_, calojetHandle);
    jetCollectionIsValid = calojetHandle.isValid();
    jet_type = "Calo";
    
  if (!jetCollectionIsValid.isValid()) {
      edm::LogWarning("JetMonitor") << " " << jet_type << " jet handle not valid \n";
      return;
    }
  
  if (isScoutingPFJetTrig)
    collSize = ScoutingJetHandle->size();  
  if (isPFJetTrig)
    collSize = PFjetHandle->size();
  if (isCaloJetTrig)
    collSize = calojetHandle->size();
  
  ///// .... it seems to not be easily feasible (not gaining in terms of coding) with this structure
  */
  
  
///////// Updated format from View to reco
  
  if (isScoutingPFJetTrig) { //(isScoutingPFJet) { ////-------> I was using "isScoutingPFJet" test again with "isScoutingPFJetTrig" 
    iEvent.getByToken(scoutjetSrc_, ScoutingJetHandle);
    if (!ScoutingJetHandle.isValid()) {
      edm::LogWarning("JetMonitor") << "Scouting jet handle not valid \n";
      return;
    }
    
    if (JetIDVersion_ == "RUN3Scouting") {
      run3scoutingpfjetidversion = Run3ScoutingPFJetIDSelectionFunctor::RUN3Scouting;
    } else {
      if (verbose_)
        std::cout << "no valid scouting Run3ScoutinPF JetID version given" << std::endl;
    }
    if (JetIDQuality_ == "TIGHT") {
      run3scoutingpfjetidquality = Run3ScoutingPFJetIDSelectionFunctor::TIGHT;
    } else if (JetIDQuality_ == "TIGHTLEPVETO") {
      run3scoutingpfjetidquality = Run3ScoutingPFJetIDSelectionFunctor::TIGHTLEPVETO;
    } else {
      if (verbose_)
        std::cout << "no Valid scouting Run3ScoutinPF JetID quality given" << std::endl;
    }
    run3scoutingpfjetIDFunctor = Run3ScoutingPFJetIDSelectionFunctor(run3scoutingpfjetidversion, run3scoutingpfjetidquality);
    for (auto const& iscoutjet : *ScoutingJetHandle) {
      //////// having Scouting JetID hardcoded ---> not optimal
      /*double abseta = abs(iscoutjet.eta());
      double jetEnergyUncorrected = iscoutjet.chargedHadronEnergy() + iscoutjet.neutralHadronEnergy() + iscoutjet.photonEnergy() + iscoutjet.electronEnergy() + iscoutjet.muonEnergy() + iscoutjet.HFEMEnergy();
      double nhf = 0.0;
      double nef = 0.0;
      double muf = 0.0;
      //double chf = 0.0; //old
      //double cef = 0.0;
      if (jetEnergyUncorrected > 0.) {
        //chf = iscoutjet.chargedHadronEnergy() / jetEnergyUncorrected; //old
        nhf = iscoutjet.neutralHadronEnergy() / jetEnergyUncorrected;
        //cef= scoutingpfjet->electronEnergy() / jetEnergyUncorrected;  // for now: electron energy is 0, since HLT scouting jets by construction don't contain electrons
        nef = (iscoutjet.photonEnergy() + iscoutjet.HFEMEnergy()) / jetEnergyUncorrected;
        muf = iscoutjet.muonEnergy() / jetEnergyUncorrected;
      }
      int nch = iscoutjet.chargedHadronMultiplicity() + iscoutjet.electronMultiplicity();
      int nneutrals = iscoutjet.neutralHadronMultiplicity() + iscoutjet.photonMultiplicity() + iscoutjet.HFEMMultiplicity();
      int nconstituents = iscoutjet.chargedHadronMultiplicity() + iscoutjet.electronMultiplicity() + iscoutjet.neutralHadronMultiplicity() + iscoutjet.photonMultiplicity() + iscoutjet.HFEMMultiplicity();
      bool passScoutjetID = false;
      if (abseta <= 2.6) {
        passScoutjetID = (nhf<0.99 && nef<0.9 && muf<0.8 && nch>0 && nconstituents>1);
        //passScoutjetID = (chf>0.01 && nhf<0.99 && nef<0.9 && muf<0.8 && nch>0 && nconstituents>1); //old
      } else if (abseta > 2.6 && abseta <= 2.7) {
        passScoutjetID = (nef<0.99 && muf<0.8 && nneutrals>1);
        //passScoutjetID = (nef<0.9 && muf<0.8); //old
      } else if (abseta > 2.7 && abseta <= 3.0) {
        passScoutjetID = (nef<0.99 && nneutrals>1);
        //passScoutjetID = (nef<0.9); //old
      } else if (abseta > 3.0) {
        passScoutjetID = (nef<0.1);
        //passScoutjetID = (nef<0.2); //old
      }
      if (!passScoutjetID) continue;*/
      
      bool passScoutjetID = false;
      passScoutjetID = run3scoutingpfjetIDFunctor(iscoutjet);
      if (!passScoutjetID) continue;
      if (!isCleanScoutingJet(iscoutjet.eta(), iscoutjet.phi(), scoutingmuons, 0.16)) continue;
      
      reco::PFJet dummy_scoutingpfjet;
      reco::Particle::PolarLorentzVector dummy_scoutingpfjetP4(iscoutjet.pt(),
                                                               iscoutjet.eta(),
                                                               iscoutjet.phi(),
                                                               iscoutjet.m());
      dummy_scoutingpfjet.setP4(dummy_scoutingpfjetP4);
      dummy_scoutingpfjet.setJetArea(iscoutjet.jetArea());

      // apply corrections on the fly
      double jec = Corrector.isValid() ? Corrector->correction(dummy_scoutingpfjet) : 1.0;
      // std:: << "Jet Pt: "<< ijet.pt() << " JEC: " << jec << std::endl;
      
      double corjet = jec*iscoutjet.pt();  /////------> iscoutjet or dummy_...jet ?????
      if (corjet < ptcut_) {
        continue;
      }
      selected_jets.push_back({corjet, iscoutjet.eta(), iscoutjet.phi()});
    } // end for loop over jets
  } else if (isPFJetTrig) {     /////I haven't fixed the pf/calo part properly
    iEvent.getByToken(jetSrc_, PFjetHandle);
    //std::cout<<"Path is " << folderName_ <<std::endl;
    if (!PFjetHandle.isValid()) {
      edm::LogWarning("JetMonitor") << "Jet handle not valid \n";
      return;
    }
    for (auto const& ijet : *PFjetHandle) {
      double corjet = 1*ijet.pt();
      if (corjet < ptcut_) {
        continue;
      }
      selected_jets.push_back({corjet, ijet.eta(), ijet.phi()});

    } // end for loop over jets
  } else if (isCaloJetTrig) {    
    iEvent.getByToken(calojetToken_, calojetHandle);
    if (!calojetHandle.isValid()) {
      edm::LogWarning("JetMonitor") << "Jet handle not valid \n";
      return;
    }
    for (auto const& j : *calojetHandle) {
      double corjet = 1*j.pt();
      if (corjet < ptcut_) {
        continue;
      }
      selected_jets.push_back({corjet, j.eta(), j.phi()});
    } // end for loop over jets
  }   //end if Scouting, PF, or Calo Jets
  
  

///////// Original format:
  /*
  if (isScoutingPFJetTrig) //(isScoutingPFJet) {
  //edm::Handle<edm::View<Run3ScoutingPFJet>> scoutjets;
    edm::Handle<std::vector<Run3ScoutingPFJet>> scoutjets;
    iEvent.getByToken(scoutjetSrc_, scoutjets);
    if (!scoutjets.isValid()) {
      edm::LogWarning("JetMonitor") << "Scouting jet handle not valid \n";
      return;
    }
    //for (edm::View<Run3ScoutingPFJet>::const_iterator ibegin = scoutjets->begin(), iend = scoutjets->end(), ijet = ibegin;
    for (std::vector<Run3ScoutingPFJet>::const_iterator ibegin = scoutjets->begin(), iend = scoutjets->end(), ijet = ibegin;
        ijet != iend;
        ++ijet) {
      if (ijet->pt() < ptcut_) {
        continue;
      }
      v_jetpt.push_back(ijet->pt());
      v_jeteta.push_back(ijet->eta());
      v_jetphi.push_back(ijet->phi());
    } // end for loop over jets
  } else
    edm::Handle<edm::View<reco::Jet> > offjets;
    iEvent.getByToken(jetSrc_, offjets);
    if (!offjets.isValid()) {
      edm::LogWarning("JetMonitor") << "Jet handle not valid \n";
      return;
    }
    for (edm::View<reco::Jet>::const_iterator ibegin = offjets->begin(), iend = offjets->end(), ijet = ibegin;
        ijet != iend;
        ++ijet) {
      if (ijet->pt() < ptcut_) {
        continue;
      }
      v_jetpt.push_back(ijet->pt());
      v_jeteta.push_back(ijet->eta());
      v_jetphi.push_back(ijet->phi());
      //    cout << "jetpt (view ) : " << ijet->pt() << endl;
    } // end for loop over jets
  } //end if Scouting or PF/Calo Jets
  */



  /*if (v_jetpt.empty())
    return;
  double jetpt_ = v_jetpt[0];
  double jeteta_ = v_jeteta[0];
  double jetphi_ = v_jetphi[0];*/
  if (selected_jets.empty())
    return;
  std::sort(selected_jets.begin(), selected_jets.end(),[](const auto& a, const auto& b){return a.pt > b.pt;});
  double jetpt_ = selected_jets[0].pt;
  double jeteta_ = selected_jets[0].eta;
  double jetphi_ = selected_jets[0].phi;

  FillME(a_ME, jetpt_, jetphi_, jeteta_, ls, "denominator");
  if (isBarrel(jeteta_)) {
    FillME(a_ME_HB, jetpt_, jetphi_, jeteta_, ls, "denominator", true, true, true, false);
  } else if (isEndCapP(jeteta_)) {
    FillME(a_ME_HE, jetpt_, jetphi_, jeteta_, ls, "denominator", true, true, true, false);
    if (enableFullMonitoring_) {
      FillME(a_ME_HE_p, jetpt_, jetphi_, jeteta_, ls, "denominator", true, false, true, false);
    }
  } else if (isEndCapM(jeteta_)) {
    FillME(a_ME_HE, jetpt_, jetphi_, jeteta_, ls, "denominator", true, true, true, false);
    if (enableFullMonitoring_) {
      FillME(a_ME_HE_m, jetpt_, jetphi_, jeteta_, ls, "denominator", true, false, true, false);
    }
  } else if (isForward(jeteta_)) {
    FillME(a_ME_HF, jetpt_, jetphi_, jeteta_, ls, "denominator", true, true, true, false);
  }

  if (num_genTriggerEventFlag_->on() && !num_genTriggerEventFlag_->accept(iEvent, iSetup))
    return;  // Require Numerator //

  FillME(a_ME, jetpt_, jetphi_, jeteta_, ls, "numerator");
  if (isBarrel(jeteta_)) {
    FillME(a_ME_HB, jetpt_, jetphi_, jeteta_, ls, "numerator", true, true, true, false);
  } else if (isEndCapP(jeteta_)) {
    FillME(a_ME_HE, jetpt_, jetphi_, jeteta_, ls, "numerator", true, true, true, false);
    if (enableFullMonitoring_) {
      FillME(a_ME_HE_p, jetpt_, jetphi_, jeteta_, ls, "numerator", true, false, true, false);
    }
  } else if (isEndCapM(jeteta_)) {
    FillME(a_ME_HE, jetpt_, jetphi_, jeteta_, ls, "numerator", true, true, true, false);
    if (enableFullMonitoring_) {
      FillME(a_ME_HE_m, jetpt_, jetphi_, jeteta_, ls, "numerator", true, false, true, false);
    }
  } else if (isForward(jeteta_)) {
    FillME(a_ME_HF, jetpt_, jetphi_, jeteta_, ls, "numerator", true, true, true, false);
  }
}

bool JetMonitor::isGoodScoutingMuon(Run3ScoutingMuon const &scoutingMuon) {
  //if (scoutingMuon.pt() > 30
  //if (scoutingMuon.pt() > 10
  if (scoutingMuon.pt() > 1
      && abs(scoutingMuon.eta()) < 0.8
      && abs(scoutingMuon.trk_dxy()) < 0.2 //
      /////&& abs(scoutingMuon.trackIso()) < 0.15 //removed
      && abs(scoutingMuon.trk_dz()) < 0.5 //
      && scoutingMuon.normalizedChi2() < 3
      && scoutingMuon.nValidRecoMuonHits() > 0
      && scoutingMuon.nRecoMuonMatchedStations() > 1
      && scoutingMuon.nValidPixelHits() > 0
      && scoutingMuon.nTrackerLayersWithMeasurement() > 5) {
    return true;
  } else {
    return false;
  }
}

bool JetMonitor::isCleanScoutingJet(double ScoutingJetEta, double ScoutingJetPhi, const std::vector<Run3ScoutingMuon>& scoutingMuons, double Scoutingdr2Cut) {
  for (const auto& scoutingMuon : scoutingMuons) {
  	double ScoutingdR2 = deltaR2(ScoutingJetEta, ScoutingJetPhi, scoutingMuon.eta(),  scoutingMuon.phi());
  	if (ScoutingdR2 < Scoutingdr2Cut) {
    		return false;
	}
  }
  return true;
}

bool JetMonitor::isBarrel(double eta) {
  bool output = false;
  if (fabs(eta) <= 1.3)
    output = true;
  return output;
}

bool JetMonitor::isEndCapM(double eta) {
  bool output = false;
  if (fabs(eta) <= 3.0 && fabs(eta) > 1.3 && (eta < 0))
    output = true;  // (mia) this magic number should come from some file in CMSSW !!!
  return output;
}

/// For Hcal Endcap Plus Area
bool JetMonitor::isEndCapP(double eta) {
  bool output = false;
  if (fabs(eta) <= 3.0 && fabs(eta) > 1.3 && (eta > 0))
    output = true;  // (mia) this magic number should come from some file in CMSSW !!!
  return output;
}

/// For Hcal Forward Area
bool JetMonitor::isForward(double eta) {
  bool output = false;
  if (fabs(eta) > 3.0)
    output = true;
  return output;
}

void JetMonitor::FillME(ObjME* a_me,
                        const double pt_,
                        const double phi_,
                        const double eta_,
                        const int ls_,
                        const std::string& DenoOrNume,
                        const bool doPhi,
                        const bool doEta,
                        const bool doEtaPhi,
                        const bool doVsLS) {
  if (DenoOrNume == "denominator") {
    // index 0 = pt, 1 = ptThreshold , 2 = pt vs ls , 3 = phi, 4 = eta,
    // 5 = eta vs phi, 6 = eta vs pt , 7 = abs(eta) , 8 = abs(eta) vs phi
    a_me[0].denominator->Fill(pt_);  // pt
    a_me[1].denominator->Fill(pt_);  // jetpT Threshold binning for pt
    if (doVsLS)
      a_me[2].denominator->Fill(ls_, pt_);  // pt vs ls
    if (doPhi)
      a_me[3].denominator->Fill(phi_);  // phi
    if (doEta)
      a_me[4].denominator->Fill(eta_);  // eta
    if (doEtaPhi)
      a_me[5].denominator->Fill(eta_, phi_);  // eta vs phi
    if (doEtaPhi)
      a_me[6].denominator->Fill(eta_, pt_);  // eta vs pT
  } else if (DenoOrNume == "numerator") {
    a_me[0].numerator->Fill(pt_);  // pt
    a_me[1].numerator->Fill(pt_);  // jetpT Threshold binning for pt
    if (doVsLS)
      a_me[2].numerator->Fill(ls_, pt_);  // pt vs ls
    if (doPhi)
      a_me[3].numerator->Fill(phi_);  // phi
    if (doEta)
      a_me[4].numerator->Fill(eta_);  // eta
    if (doEtaPhi)
      a_me[5].numerator->Fill(eta_, phi_);  // eta vs phi
    if (doEtaPhi)
      a_me[6].numerator->Fill(eta_, pt_);  // eta vs pT
  } else {
    edm::LogWarning("JetMonitor") << "CHECK OUT denu option in FillME !!! DenoOrNume ? : " << DenoOrNume << std::endl;
  }
}

void JetMonitor::bookMESub(DQMStore::IBooker& Ibooker,
                           ObjME* a_me,
                           const int len_,
                           const std::string& h_Name,
                           const std::string& h_Title,
                           const std::string& h_subOptName,
                           const std::string& hSubT,
                           const bool doPhi,
                           const bool doEta,
                           const bool doEtaPhi,
                           const bool doVsLS) {
  std::string hName = h_Name;
  std::string hTitle = h_Title;
  const std::string hSubN = h_subOptName.empty() ? "" : "_" + h_subOptName;

  int nbin_phi = jet_phi_binning_.nbins;
  double maxbin_phi = jet_phi_binning_.xmax;
  double minbin_phi = jet_phi_binning_.xmin;

  int nbin_eta = jet_eta_binning_.nbins;
  double maxbin_eta = jet_eta_binning_.xmax;
  double minbin_eta = jet_eta_binning_.xmin;

  hName = h_Name + "pT" + hSubN;
  hTitle = h_Title + " pT " + hSubT;
  bookME(Ibooker, a_me[0], hName, hTitle, jetpt_binning_.nbins, jetpt_binning_.xmin, jetpt_binning_.xmax);
  setMETitle(a_me[0], h_Title + " pT [GeV]", "events / [GeV]");

  hName = h_Name + "pT_pTThresh" + hSubN;
  hTitle = h_Title + " pT " + hSubT;
  bookME(Ibooker, a_me[1], hName, hTitle, jetptThr_binning_.nbins, jetptThr_binning_.xmin, jetptThr_binning_.xmax);
  setMETitle(a_me[1], h_Title + "pT [GeV]", "events / [GeV]");

  if (doVsLS) {
    hName = h_Name + "pTVsLS" + hSubN;
    hTitle = h_Title + " vs LS " + hSubT;
    bookME(Ibooker,
           a_me[2],
           hName,
           hTitle,
           ls_binning_.nbins,
           ls_binning_.xmin,
           ls_binning_.xmax,
           jetpt_binning_.xmin,
           jetpt_binning_.xmax);
    setMETitle(a_me[2], "LS", h_Title + "pT [GeV]");
  }

  if (doPhi) {
    hName = h_Name + "phi" + hSubN;
    hTitle = h_Title + " phi " + hSubT;
    bookME(Ibooker, a_me[3], hName, hTitle, nbin_phi, minbin_phi, maxbin_phi);
    setMETitle(a_me[3], h_Title + " #phi", "events / 0.1 rad");
  }

  if (doEta) {
    hName = h_Name + "eta" + hSubN;
    hTitle = h_Title + " eta " + hSubT;
    bookME(Ibooker, a_me[4], hName, hTitle, nbin_eta, minbin_eta, maxbin_eta);
    setMETitle(a_me[4], h_Title + " #eta", "events");
  }

  if (doEtaPhi) {
    hName = h_Name + "EtaVsPhi" + hSubN;
    hTitle = h_Title + " eta Vs phi " + hSubT;
    bookME(Ibooker, a_me[5], hName, hTitle, nbin_eta, minbin_eta, maxbin_eta, nbin_phi, minbin_phi, maxbin_phi);
    setMETitle(a_me[5], h_Title + " #eta", "#phi");
  }

  if (doEtaPhi) {
    hName = h_Name + "EtaVspT" + hSubN;
    hTitle = h_Title + " eta Vs pT " + hSubT;
    bookME(Ibooker,
           a_me[6],
           hName,
           hTitle,
           nbin_eta,
           minbin_eta,
           maxbin_eta,
           jetpt_binning_.nbins,
           jetpt_binning_.xmin,
           jetpt_binning_.xmax);
    setMETitle(a_me[6], h_Title + " #eta", "Leading Jet pT [GeV]");
  }
}

void JetMonitor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("FolderName", "HLT/Jet");
  desc.add<bool>("requireValidHLTPaths", true);

  desc.add<edm::InputTag>("jetSrc", edm::InputTag("ak4PFJetsCHS"));
  desc.add<edm::InputTag>("muons", edm::InputTag("muons"));
  desc.add<edm::InputTag>("vertices", edm::InputTag("offlinePrimaryVertices"));
  desc.add<edm::InputTag>("corrector", edm::InputTag("ak4PFPuppiL1FastL2L3Corrector"));
  desc.add<double>("ptcut", 30);
  desc.add<bool>("ispfjettrg", true);
  desc.add<bool>("iscalojettrg", false);
  desc.add<bool>("isscoutingpfjettrg", false);
  desc.add<bool>("isscoutingpfjet", false);
  
  desc.add<std::string>("JetIDQuality", "TIGHT");
  desc.add<std::string>("JetIDVersion", "RUN3Scouting");  //maybe put a pfpuppi version, but need to define it in the relevant if above
  
  desc.add<bool>("enableFullMonitoring", true);
  desc.add<unsigned>("nmuons", 1);

  edm::ParameterSetDescription genericTriggerEventPSet;
  GenericTriggerEventFlag::fillPSetDescription(genericTriggerEventPSet);

  desc.add<edm::ParameterSetDescription>("numGenericTriggerEventPSet", genericTriggerEventPSet);
  desc.add<edm::ParameterSetDescription>("denGenericTriggerEventPSet", genericTriggerEventPSet);

  edm::ParameterSetDescription histoPSet;
  edm::ParameterSetDescription jetPSet;
  edm::ParameterSetDescription jetPtThrPSet;
  fillHistoPSetDescription(jetPSet);
  fillHistoPSetDescription(jetPtThrPSet);
  histoPSet.add<edm::ParameterSetDescription>("jetPSet", jetPSet);
  histoPSet.add<edm::ParameterSetDescription>("jetPtThrPSet", jetPtThrPSet);
  histoPSet.add<std::vector<double> >("jetptBinning",
                                      {0.,   20.,  40.,  60.,  80.,  90.,  100., 110., 120., 130., 140., 150., 160.,
                                       170., 180., 190., 200., 220., 240., 260., 280., 300., 350., 400., 450., 1000.});

  edm::ParameterSetDescription lsPSet;
  fillHistoLSPSetDescription(lsPSet);
  histoPSet.add<edm::ParameterSetDescription>("lsPSet", lsPSet);

  desc.add<edm::ParameterSetDescription>("histoPSet", histoPSet);

  descriptions.add("jetMonitoring", desc);
}

DEFINE_FWK_MODULE(JetMonitor);
