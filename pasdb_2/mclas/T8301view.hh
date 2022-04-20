#ifndef T8301view_hh 
#define T8301view_hh 

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "T9301read.hh"

class T8301view: public T9301read{

public :   
 
  T8301view(TString _in_fn); 
  T8301view();
  ~T8301view();

  void                GetInitialInfo();
  Bool_t              IsEventOK();
  void                PrepareBaselineVector();
  void                PrintBaselineVector();
  void                FillAveragedBaselineHis();
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
  TH1D               *bl_hSvsF_subtr;
  TH1D               *bl_hSvsF_resid;

  TF1                *ffn_rpol2;
  TF1                *ffn_p2;
  TF1                *ffn_conv_p3;
  TF1                *ffn_conv_p2p3;

  Double_t            bl_data_np;
  Double_t            bl_daq_card_n;
  Double_t            bl_daq_gain;
  Double_t            bl_nmrL_tune_v;
  Double_t            bl_nmrL_phase_v;
  Double_t            bl_nmrL_if_attn;
protected:

    TFile              *blout_f;
  
private:

 void          InitT8301Variables();


};

#endif

