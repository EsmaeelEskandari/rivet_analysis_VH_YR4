// -*- C++ -*-
#include "MC_VHbb.h"

namespace Rivet {

  using namespace Cuts;

  class MC_ZnunuHbb_undecayed : public Analysis {

  private:

    
  public:

    /// Constructor
    MC_ZnunuHbb_undecayed()
    : Analysis("MC_ZnunuHbb_undecayed")
    {    
    }

    //////////////////////////////////////////////////
    /// Book histograms and initialise projections before the run
    //////////////////////////////////////////////////
    void init() {

      // initialise projections
      FinalState fs;
      addProjection(fs, "fs");
      
      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// START OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////
      
      // find Znuenue
      ZFinder znuenuefinder(fs, cut, PID::NU_E, 0*GeV, 1e10*GeV, 0.2);
      addProjection(znuenuefinder, "znuenuefinder");
      // find Znumunumu
      ZFinder znumunumufinder(fs, cut, PID::NU_MU, 0*GeV, 1e10*GeV, 0.2);
      addProjection(znumunumufinder, "znumunumufinder");
      // find Znutaunutau
      ZFinder znutaunutaufinder(fs, cut, PID::NU_TAU, 0*GeV, 1e10*GeV, 0.2);
      addProjection(znutaunutaufinder, "znutaunutaufinder");
      // fish out Zee, Zmm, Znunu
      VetoedFinalState jetproinput;
      jetproinput.addVetoOnThisFinalState(znuenuefinder);
      jetproinput.addVetoOnThisFinalState(znumunumufinder);
      jetproinput.addVetoOnThisFinalState(znutaunutaufinder);
      
      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// END OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////
      
      ////////////////////////////////////////////////////////
      // Unstable Higgs
      ////////////////////////////////////////////////////////
      UnstableFinalState undec_higgs(PID::HIGGS);
      addProjection(undec_higgs, "UFS");
      IdentifiedFinalState higgses(PID::HIGGS);
      jetproinput.addVetoOnThisFinalState(higgses);
      addProjection(higgses, "Higgses");
      VetoedFinalState rest(fs);
      rest.addVetoOnThisFinalState(higgses);
      addProjection(rest, "Rest");

      // make jets
      FastJets jetpro(jetproinput, FastJets::ANTIKT, 0.5);
      addProjection(jetpro, "jetpro");
      // jets for differential jet rates
      FastJets jetpro_kt(jetproinput, FastJets::KT, 0.7);
      addProjection(jetpro_kt, "jetpro_kt");

      ////////////////////////////////////////////////////////
      // MET
      ////////////////////////////////////////////////////////
      MissingMomentum met(fs);
      addProjection(met, "MET");

      initialize_Histos();
    }
    
    ////////////////////////////////////////////////////////
    // ---- HISTOS ----
    ////////////////////////////////////////////////////////
    void initialize_Histos(){

      std::vector<Histo1DPtr> dummy1;
      
      // CONTROL PLOTS ONLY IN INCLUSIVE CASE
      // all jets used for dij
      for (size_t i=0; i < njets; ++i) {
        _h_VptInc_AjetInc__log10_d.push_back(bookHisto1D("xcheck_VptInc_AjetInc__log10_d_" +to_str(i) + to_str(i+1), 100, 0.2, 4));
        _h_VptInc_AjetInc__ajets_pT.push_back(bookHisto1D("xcheck_VptInc_AjetInc__aJet_pT" +to_str(i),hist_nbins,0,500));
        _h_VptInc_AjetInc__ajets_eta.push_back(bookHisto1D("xcheck_VptInc_AjetInc__aJet_eta" +to_str(i),hist_nbins,-5,5));
      }
      // selected jets
      for (size_t i=0; i < 1; ++i) {
        _h_VptInc_AjetInc__Sel_ajets_pT.push_back(bookHisto1D("xcheck_VptInc_AjetInc__Sel_aJet_pT" +to_str(i),hist_nbins,0,500));
        _h_VptInc_AjetInc__Sel_ajets_eta.push_back(bookHisto1D("xcheck_VptInc_AjetInc__Sel_aJet_eta" +to_str(i),hist_nbins,-5,5));
      }
      
      // CONTROL PLOTS ONLY IN INCLUSIVE CASE (didn't change the histo array structure in case some histos need to be "promoted")
      for (size_t ivpt=0; ivpt < 1; ivpt++){
        for (size_t iajet=0; iajet < 1; iajet++){
        
        _h__weights.push_back(dummy1);
        _h__weights[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__ev_weights",3,-1.5,1.5));
        // Z properties
        _h__V_mass.push_back(dummy1);
        _h__V_mass[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__V_mass",hist_nbins,60,120));
        _h__V_rap.push_back(dummy1);
        _h__V_rap[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__V_rap",hist_nbins,hist_H_rap_min,hist_H_rap_max));
        _h__V_pT.push_back(dummy1);
        // ------> V_pT is NOT a crosscheck plot!!!
        _h__V_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__V_pT",hist_nbins,hist_V_pt_min,hist_V_pt_max));
        // higgs_candidate properties
        _h__higgs_candidate_mass.push_back(dummy1);
        _h__higgs_candidate_mass[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__higgs_candidate_mass",hist_nbins,90,160));
        // H+Z properties
        _h__HV_rap.push_back(dummy1);
        _h__HV_rap[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__HV_rap",hist_nbins,hist_H_rap_min,hist_H_rap_max));
        _h__HV_pT.push_back(dummy1);
        _h__HV_pT[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__HV_pT",hist_nbins,hist_V_pt_min,hist_V_pt_max));
        // higgs jets
        std::vector<std::vector<Histo1DPtr> > dummy2;

        _h__dR_HV.push_back(dummy1);
        _h__dR_HV[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__dR_HV",hist_nbins,0,5));
        _h__dphi_HV.push_back(dummy1);
        _h__dphi_HV[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__dphi_HV",hist_nbins,0,3.2));
        _h__deta_HV.push_back(dummy1);
        _h__deta_HV[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__deta_HV", hist_nbins,0,5));
        
        // nJets
        _h__Sel_najets.push_back(dummy1);
        _h__Sel_najets[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__Sel_najets",11,-0.5,10.5));
        _h__Sel_nbjets.push_back(dummy1);
        _h__Sel_nbjets[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__Sel_nbjets",11,-0.5,10.5));
      }
    }
      
      // 0 < pT_H < 500 (a)
      // -5 < y_H < 5 (b)
      // 0 < pT_lepton < 500 (c)
      // 0 < pT_nu/nubar < 500 (d)
      // -5 < y_lepton < 5 (e)

      for (size_t ivpt=0; ivpt < Vpt_bins; ivpt++){
        for (size_t iajet=0; iajet < NJet_bins; iajet++){
          
          // higgs_candidate properties
          _h__higgs_candidate_rap.push_back(dummy1);
          _h__higgs_candidate_rap[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__higgs_candidate_rap",hist_nbins,hist_H_rap_min,hist_H_rap_max));
          _h__higgs_candidate_pT.push_back(dummy1);
          _h__higgs_candidate_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__higgs_candidate_pT",hist_nbins,hist_H_pt_min,hist_H_pt_max));
          // V lepton properties
          _h__leadlep_pT.push_back(dummy1);
          _h__leadlep_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__leadlep_pT",hist_nbins,hist_lep_pt_min,hist_lep_pt_max));
          _h__leadlep_eta.push_back(dummy1);
          _h__leadlep_eta[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__leadlep_eta",hist_nbins,hist_lep_eta_min,hist_lep_eta_max));
          _h__subleadlep_pT.push_back(dummy1);
          _h__subleadlep_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__subleadlep_pT",hist_nbins,hist_lep_pt_min,hist_lep_pt_max));
          _h__subleadlep_eta.push_back(dummy1);
          _h__subleadlep_eta[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__subleadlep_eta",hist_nbins,hist_lep_eta_min,hist_lep_eta_max));

        }
      }

    }

    //////////////////////////////////////////////////
    /// Perform the per-event analysis
    //////////////////////////////////////////////////
    void analyze(const Event& event) {
      const double weight = event.weight();

      ////////////////////////////////////////////////////////
      // MET
      ////////////////////////////////////////////////////////
      // const MissingMomentum& MET_tmp = applyProjection<MissingMomentum>(event, "MET");
      // MET_4v = -MET_tmp.visibleMomentum();
      // MET = MET_4v.pT();

      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// START OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////

      // apply the projections
      const ZFinder& znuenuefinder = applyProjection<ZFinder>(event, "znuenuefinder");
      const ZFinder& znumunumufinder = applyProjection<ZFinder>(event, "znumunumufinder");
      const ZFinder& znutaunutaufinder = applyProjection<ZFinder>(event, "znutaunutaufinder");
      const Particles znunu = znuenuefinder.bosons() + znumunumufinder.bosons()+ znutaunutaufinder.bosons();
      ParticleVector higgses =
      applyProjection<IdentifiedFinalState>(event, "Higgses").particles();
      // unstable higgs
      const UnstableFinalState& undec_higgs = applyProjection<UnstableFinalState>(event, "UFS");
      // jets
      const FastJets& jetpro = applyProjection<FastJets>(event,"jetpro");
      ParticleVector rest =
      applyProjection<VetoedFinalState>(event, "Rest").particles();
      
      if(higgses.size()==0)
        for (size_t i(0);i<rest.size();++i) {
        if(rest[i].pid()==25){
          // cout << " i= " << i << " PdgId= " << rest[i].pid() << endl;
          higgses.push_back(rest[i]);
          FourMomentum dummy_mom(0,0,0,0);
          // cout << "before rest["<<i<<"].momentum()= " << rest[i].momentum() << " PdgId= " << rest[i].pid() << endl;
          rest[i].setMomentum(dummy_mom);
          // cout << "after rest["<<i<<"].momentum()= " << rest[i].momentum() << " PdgId= " << rest[i].pid() << endl;
        }
      }

      // only continue if a Z has been found
      if ( znunu.empty() ) vetoEvent;

      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// END OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////

      Jets ajets, sel_ajets;
      ajets.clear();
      sel_ajets.clear();
      Jet h0, h1; 
      FourMomentum  higgs_candidate,Z, HV;
      std::vector<size_t> b_indices;
      int na = 0.0;
      int nb = 0.0;
      const Jets alljets = jetpro.jetsByPt();
      // find all jets with bs
      for (size_t i = 0; i < alljets.size(); ++i) {
        if (alljets[i].containsBottom()) {
          b_indices.push_back(i);
          if (alljets[i].pT() > cut_bjets_ptmin && abs(alljets[i].eta()) < cut_bjets_etamax) nb++;
        }
      }

      // std::cout << "higgses.size()= "<< higgses.size() << " alljets.size()= "<< alljets.size() << " b_indices.size()= "<< b_indices.size() << " undec_higgs.size()= "<< undec_higgs.size() << std::endl;
      // require at least 2 b jets or undecayed higgs
      if (b_indices.size() < 2 && undec_higgs.size() == 0 && higgses.size() == 0) vetoEvent;
      if (higgses.size()>1) vetoEvent;

      
      size_t idx0 = -1;
      size_t idx1 = -1;
      higgs_candidate = higgses[0].momentum();
      
      // cout << "before veto: higgs_candidate.pT()= " << higgs_candidate.pT() << endl;
      if( ! higgs_candidate.pT()>0 ) vetoEvent;

      Z = znunu[0].momentum();
      
      Particles leptons;
      if(znuenuefinder.bosons().size()>0) leptons = znuenuefinder.constituents(cmpMomByPt);
      else if(znumunumufinder.bosons().size()>0) leptons = znumunumufinder.constituents(cmpMomByPt);
      else if(znutaunutaufinder.bosons().size()>0) leptons = znutaunutaufinder.constituents(cmpMomByPt);
      // const FourMomentum lminus = leptons[0].charge() < 0 ? leptons[0].momentum() : leptons[1].momentum();
      // const FourMomentum lplus = leptons[0].charge() < 0 ? leptons[1].momentum() : leptons[0].momentum();      
      const FourMomentum leadlep = leptons[0].momentum();
      const FourMomentum subleadlep = leptons[1].momentum();

      // HV system
      HV = higgs_candidate + Z;
      
      // add everything else to the ajets 
      for (size_t i = 0; i < alljets.size(); ++i) {
        if (i != idx0 && i != idx1){
          ajets.push_back(alljets[i]);
          if (alljets[i].pT() > cut_addjets_ptmin && abs(alljets[i].eta()) < cut_addjets_etamax){
            na++;
            sel_ajets.push_back(alljets[i]);
          }
        }
      }
      
      //////////////////////////////////////////////////
      // fill histos
      //////////////////////////////////////////////////
      
      //////////////////////////////////////////////////
      /// Only Inclusive pT, inclusive jet
      //////////////////////////////////////////////////
      
      // Jet resolutions and integrated jet rates
      double d_ij;
      const FastJets& jetpro_kt = applyProjection<FastJets>(event,"jetpro_kt");
      const fastjet::ClusterSequence* seq = jetpro_kt.clusterSeq();
      if (seq != NULL) {
        for (size_t i = 0; i < njets; ++i) {
          // Jet resolution i -> j
          d_ij = log10(sqrt(seq->exclusive_dmerge_max(i)));

          // Fill differential jet resolution
          _h_VptInc_AjetInc__log10_d[i]->fill(d_ij, weight);

          //fill aJet pT histos
          _h_VptInc_AjetInc__ajets_pT[i]->fill(ajets[i].pT(),weight);
          _h_VptInc_AjetInc__ajets_eta[i]->fill(ajets[i].eta(),weight);
        }
      }
      for (size_t iajet=0; iajet < 1; iajet++){
        if(sel_ajets.size()>0){
          _h_VptInc_AjetInc__Sel_ajets_pT[iajet]->fill(sel_ajets[0].pT(),weight);
          _h_VptInc_AjetInc__Sel_ajets_eta[iajet]->fill(sel_ajets[0].eta(),weight);
        }
      }
      for (size_t ivpt=0; ivpt < 1; ivpt++){
        for (size_t iajet=0; iajet < 1; iajet++){
          
          _h__weights[ivpt][iajet]->fill(weight/(std::abs(weight)>0?std::abs(weight):1));
          
          _h__V_mass[ivpt][iajet]->fill(Z.mass(),weight);
          _h__V_rap[ivpt][iajet]->fill(Z.rapidity(),weight);
          _h__V_pT[ivpt][iajet]->fill(Z.pT(),weight);
          
          _h__higgs_candidate_mass[ivpt][iajet]->fill(higgs_candidate.mass(),weight);
          
          _h__HV_rap[ivpt][iajet]->fill(HV.rapidity(),weight);
          _h__HV_pT[ivpt][iajet]->fill(HV.pT(),weight);

          _h__Sel_najets[ivpt][iajet]->fill(na,weight);
          _h__Sel_nbjets[ivpt][iajet]->fill(nb,weight);

          _h__dR_HV[ivpt][iajet]->fill(deltaR(higgs_candidate,Z),weight);
          _h__deta_HV[ivpt][iajet]->fill(deltaEta(higgs_candidate,Z),weight);
          _h__dphi_HV[ivpt][iajet]->fill(deltaPhi(higgs_candidate,Z),weight);
        }
      }
      //////////////////////////////////////////////////
      /// Loop over pT bins, jet bins
      //////////////////////////////////////////////////
      for (size_t ivpt=0; ivpt < Vpt_bins; ivpt++){
        
        // std::cout << "Z.pT()= "<< Z.pT() << std::endl;
     
        if(Z.pT() < cut_Znunu_pt_low_edge_bin[ivpt] || Z.pT() > cut_Znunu_pt_high_edge_bin[ivpt]) continue;
          
        for (size_t iajet=0; iajet < NJet_bins; iajet++){

          // std::cout << "sel_ajets.size()= "<< sel_ajets.size() << std::endl;
          if(sel_ajets.size() < cut_najets_low_edge_bin[iajet] || sel_ajets.size() >= cut_najets_high_edge_bin[iajet]) continue;
          
          // std::cout 
          // << "leadlep.pT()= "<< leadlep.pT() << " leadlep.eta()= "<< leadlep.eta() 
          // << " subleadlep.pT()= "<< subleadlep.pT() << " subleadlep.eta()= "<< subleadlep.eta() 
          // << " higgs_candidate.pT()= "<< higgs_candidate.pT() << " higgs_candidate.rapidity()= "<< higgs_candidate.rapidity() 
          // << std::endl;
          
          _h__leadlep_pT[ivpt][iajet]->fill(leadlep.pT(),weight);
          _h__leadlep_eta[ivpt][iajet]->fill(leadlep.eta(),weight);
          _h__subleadlep_pT[ivpt][iajet]->fill(subleadlep.pT(),weight);
          _h__subleadlep_eta[ivpt][iajet]->fill(subleadlep.eta(),weight);

          _h__higgs_candidate_rap[ivpt][iajet]->fill(higgs_candidate.rapidity(),weight);
          _h__higgs_candidate_pT[ivpt][iajet]->fill(higgs_candidate.pT(),weight);

        }
      }
    }

    /// Normalise histograms etc., after the run
    void finalize() {

      for (size_t i = 0; i < njets; ++i) {
        scale(_h_VptInc_AjetInc__log10_d[i], /*crossSection()*/ 1./sumOfWeights());
        scale(_h_VptInc_AjetInc__ajets_pT[i], /*crossSection()*/ 1./sumOfWeights());
        scale(_h_VptInc_AjetInc__ajets_eta[i], /*crossSection()*/ 1./sumOfWeights());
      }
      
      scale(_h_VptInc_AjetInc__Sel_ajets_pT[0], /*crossSection()*/ 1./sumOfWeights());
      scale(_h_VptInc_AjetInc__Sel_ajets_eta[0], /*crossSection()*/ 1./sumOfWeights());

      for (size_t ivpt=0; ivpt < 1; ivpt++){
        for (size_t iajet=0; iajet < 1; iajet++){
                    
          scale(_h__V_mass[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__V_rap[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__V_pT[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__higgs_candidate_mass[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          
          scale(_h__HV_pT[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__HV_rap[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());

          scale(_h__dR_HV[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__deta_HV[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__dphi_HV[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());

          scale(_h__Sel_najets[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__Sel_nbjets[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
        }
      }
      
      for (size_t ivpt=0; ivpt < Vpt_bins; ivpt++){
        for (size_t iajet=0; iajet < NJet_bins; iajet++){

          scale(_h__higgs_candidate_rap[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__higgs_candidate_pT[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__leadlep_pT[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__leadlep_eta[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__subleadlep_pT[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
          scale(_h__subleadlep_eta[ivpt][iajet], /*crossSection()*/ 1./sumOfWeights());
        }
      }
    }

  };

  DECLARE_RIVET_PLUGIN(MC_ZnunuHbb_undecayed);

}
