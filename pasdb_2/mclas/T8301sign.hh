#ifndef T8301sign_hh 
#define T8301sign_hh 

#include <fstream>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TF1.h>
#include <TH1D.h>

#include "T9301read.hh"
#include "THupgrade.hh"

class T8301sign: public T9301read{

public :   
 
  T8301sign(TString _in_fn); 
  T8301sign();
  ~T8301sign();

  void                GetBaselineInfo(TString _bl_fn);
  void                GetConfigInfo(TString _cf_fn);
  void                GetInitialInfo();
  void                InitOutFile(TString _dn);
  void                InitHistos();
  Bool_t              DoesEventMatchBaseline();
  Bool_t              IsEventOK();

  void                FillVectorAll();
  void                SetSampleSize(Int_t _ss);
  Bool_t              FillVectorAve();
  Bool_t              FillVectorAveI(Int_t _i);
  void                GetPrefix();
  void                FillHistAve();
  void                GettPTAve();
  
  void                GetBaselineSubtractedAve();
  void                GetBackgroundHistAve();
  
  void                FitP2();
  void                FitConvP3();
  void                FitConvP2P3();
  void                FitConvP4();
  void                GetP2FitSubtracted();
  void                FitP1();
  void                FitGaus();

  void                GetSignalAve();
  void                GetNoiseHis();
  void                GetTEAreaAve();
  void                GetTEAreaAve_tPTAve();
  void                GetAuxAve();
  void                GetAuxAve_tPTAve();
  void                GetSignalValues();
  void                GetNoiseValues(Int_t _ev);
  void                GetQcurveValues();
  void                IncrementAveragedEvent();
  Double_t            CalculateArea(TH1D *_h);

  static Double_t     fit_p2(Double_t *x , Double_t *par);
  static Double_t     fit_p3(Double_t *x , Double_t *par);
  static Double_t     fit_p2p3(Double_t *x , Double_t *par);
  static Double_t     fit_p4(Double_t *x , Double_t *par);
  static Double_t     fit_p1(Double_t *x , Double_t *par);
  
  void                SetFittingMinimizer();
  void                WriteAveEvent();
  void                WriteSignResults();
  
  std::vector<std::vector<Double_t>>  sg_sig; //vector of vectors
  std::vector<Double_t>               sg_t; //timestamp
  std::vector<Double_t>               sg_P; //4He pressure
  std::vector<Double_t>               sg_T; //4He temperature

  
  TString             pref;
  TH1D               *sg_hSvsF;
  TH1D               *sg_hSvsF_subtr;
  TH1D               *sg_hSvsF_bkgrd;
  TH1D               *sg_hSvsF_bkgrd_subtr;
  TH1D               *sg_hSvsF_signal;
  TH1D               *sg_hSvsF_noise;
  TH1D               *sg_hSvsF_noise_all;
  TF1                *ffn_p2;
  TF1                *ffn_p3;
  TF1                *ffn_p2p3;  
  TF1                *ffn_p1;
  TF1                *ffn_g1;
  TF1                *ffn_g2;
  
  TGraphErrorsu      *sg_gPvsT;
  TGraphErrorsu      *sg_gPvsT_T;
  TGraphErrorsu      *sg_he4PvsT;
  TGraphErrorsu      *sg_he4T_t;
  TGraphErrorsu      *sg_he4P_t;
  TGraphErrorsu      *sg_g_qcv_dcoff;//offset of qcurve minimum
  TGraphErrorsu      *sg_g_qcv_dcoff_T;//offset of qcurve minimum
  TGraphErrorsu      *sg_g_qcv_moff;//offset of qcurve minimum
  TGraphErrorsu      *sg_g_qcv_ampl;//qcurve max and min difference
  TGraphErrorsu      *sg_g_qcv_ampl_T;//qcurve max and min difference
  TGraphErrorsu      *sg_g_qcv_skew;//qcurve left and right elevation difference
  TGraphErrorsu      *sg_g_qcv_skew_T;//qcurve left and right elevation difference
  TGraphErrorsu      *sg_g_qcv_msht;//qcurve center shift from set center
  TGraphErrorsu      *sg_g_qcv_msht1;//qcurve center shift from set center
  TGraphErrorsu      *sg_g_qcv_msht1_T;//qcurve center shift from set centera
  TGraphErrorsu      *sg_g_qcv_cubc;//qcurve cubic term max and min difference
  TGraphErrorsu      *sg_g_sgl1_ampl;//pure signal peak gaussian fit par0
  TGraphErrorsu      *sg_g_sgl1_sigm;//pure signal peak gaussian fit par2
  TGraphErrorsu      *sg_g_sgl2_ampl;//pure signal peak gaussian fit par0
  TGraphErrorsu      *sg_g_sgl2_sigm;//pure signal peak gaussian fit par2
  TGraphErrorsu      *sg_g_sgl_arat;//pure signal peak gaussian fit par0
  TGraphErrorsu      *sg_g_sgl_srat;//pure signal peak gaussian fit par2
  TGraphErrorsu      *sg_g_sgl_nrms;
  TGraphErrorsu      *sg_g_noise_mean;
  TGraphErrorsu      *sg_g_noise_sigm;

  Bool_t              bl_provided;
  TString             bl_fn;
  TH1D               *bl_hSvsF;
  TF1                *bl_ffn_conv_p2p3;
  Double_t            bl_data_np;
  Double_t            bl_daq_card_n;
  Double_t            bl_daq_gain;
  Double_t            bl_nmrL_tune_v;
  Double_t            bl_nmrL_phase_v;
  Double_t            bl_nmrL_if_attn;

  Double_t            sg_data_np;
  Double_t            sg_daq_card_n;
  Double_t            sg_daq_gain;
  Double_t            sg_nmrL_tune_v;
  Double_t            sg_nmrL_phase_v;
  Double_t            sg_nmrL_if_attn;

  Double_t            sg_area;
  Double_t            sg_area_err;
  Double_t            sg_area_now;
  Double_t            sg_sgl1_ampl;
  Double_t            sg_sgl1_sigm;
  Double_t            sg_sgl2_ampl;
  Double_t            sg_sgl2_sigm;
  Int_t               sg_nn_ave;
  Int_t               sg_cc_ave;
  Bool_t              sg_is_ave;
  Int_t               sg_ave_ev_cc;
  Double_t            sg_t_av;
  Double_t            sg_t_av_er;
  Double_t            sg_P_av;
  Double_t            sg_P_av_er;
  Double_t            sg_T_av;
  Double_t            sg_T_av_er;
  
  Double_t            fitx1;
  Double_t            fitx2;
  Double_t            fitx2_1;
  Double_t            fitx3_1;
  Double_t            fitx3;
  Double_t            fitx4;
  Double_t            f0_guess;

  Double_t            bkgrd_x1;
  Double_t            bkgrd_x2;
  Double_t            bkgrd_x3;
  Double_t            bkgrd_x4;
  Double_t            signl_x1;
  Double_t            sginl_x2;


protected:

  TFile              *sgout_f;
  std::ofstream       fout2;
  
private:

 void          InitT8301Variables();


};

#endif

