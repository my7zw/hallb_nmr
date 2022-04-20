#ifndef T8301base_hh 
#define T8301base_hh 

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TF1.h>
#include <TGraph.h>

#include "T9301read.hh"

class T8301base: public T9301read{

public :   
 
  T8301base(TString _in_fn); 
  T8301base();
  ~T8301base();

  void                GetInitialInfo();
  Bool_t              IsEventOK();
  void                InitHistos();
  void                FillVectorAll();
  void                SetSampleSize(Int_t _ss);
  Bool_t              FillVectorAve();
  void                FillHistAve();
  void                CloneHistos();
  void                PrintBaselineVector();
  void                SetFittingMinimizer();

  void                FitP2();
  void                FitConvP3();
  void                FitConvP2P3();
  void                GetP2FitSubtracted();
  void                GetResidual();

  static Double_t     fit_p2(Double_t *x , Double_t *par);
  static Double_t     fit_conv_p3(Double_t *x , Double_t *par);
  static Double_t     fit_conv_p2p3(Double_t *x , Double_t *par);
  
  void                WriteBaselineResults();
  
  std::vector<std::vector<Double_t>>  bl_sig; //vector of vectors
  TH1D               *bl_hSvsF;
  TH1D               *bl_hSvsF_cl;
  TH1D               *bl_hSvsF_subtr;
  TH1D               *bl_hSvsF_resid;
  
  TF1                *ffn_p2;
  TF1                *ffn_conv_p3;
  TF1                *ffn_conv_p2p3;
  TGraph             *ffn_p2_p2p3_dif;

  Double_t            bl_data_np;
  Double_t            bl_daq_card_n;
  Double_t            bl_daq_gain;
  Double_t            bl_nmrL_tune_v;
  Double_t            bl_nmrL_phase_v;
  Double_t            bl_nmrL_if_attn;
  Int_t               bl_nn_ave;
  Int_t               bl_cc_ave; 
  Bool_t              bl_is_ave;
  

protected:

    TFile              *blout_f;
  
private:

 void          InitT8301Variables();


};

#endif

