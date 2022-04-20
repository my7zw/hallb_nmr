#ifndef T9301make_hh 
#define T9301make_hh 

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "THupgrade.hh"

class T9301make{

public :   
 
  T9301make(TString _in_fn); 
  T9301make();
  ~T9301make();

  void       FillT9301Tree();
  void       CleanT9301();
  void       ExitT9301(Int_t _err_ind);

  
  Bool_t                is_nmrL;
  Bool_t                is_nmrU;
  Bool_t                is_bl;
  Bool_t                is_te;
  Bool_t                is_ep;

  //--------------------------------------------------------------
  
  Double_t              ev_time;
  TString               ev_time_str;
  Double_t              ev_id;         //count
  Int_t                 ev_count;      //count
  Double_t              ev_4He_p;      //Torr
  Double_t              ev_4He_t;      //K
  
  Double_t              rf_f0;         //MHz
  Double_t              rf_fstep;      //MHz
  Double_t              rf_nstep;      //count ->data_np
  Double_t              rf_power;      //
  Double_t              rf_mod;        //kHz
  Double_t              rf_s;          //MHz
  Double_t              rf_e;          //MHz

  Double_t              daq_card_n;    //count
  Double_t              daq_gain;      //0:low, 1:medium, 2:high
  Double_t              daq_dcoff_v;   //V
  
  Double_t              onl_calib_c;   //
  Double_t              onl_peak_area; //?
  Double_t              onl_bl_fn;
  
  Int_t                 data_np;       //count
  Int_t                 data_nsweep;   //count
  std::vector<Double_t> data_sig;      //V
  TH1D                 *data_hSvsF;
  TGraphErrors         *data_gSvsF;

  //--------------------------------------------------------------
  
  Double_t              nmrL_card_t;   //C
  Double_t              nmrL_card_psv; //V
  Double_t              nmrL_tune_v;   //V 
  Double_t              nmrL_if_attn;  //
  Double_t              nmrL_phase_v;  //V
  Double_t              nmrL_log_ch;   // 1 if we are using log amp
  Double_t              nmrL_pol_sign; // 0-no invert, 1-inverted
  Double_t              nmrL_py_hcut;  //MHz
  Double_t              nmrL_py_lcut;  //MHz
  
  Double_t              nmrL_base_amp; //?
  Double_t              nmrL_peak_amp; //
  Double_t              nmrL_peak_cntr;//
  Double_t              nmrL_beam_on;  //

  //--------------------------------------------------------------
  
  Double_t              nmrU_pol;      //%
  Double_t              nmrU_base_time;//
  Double_t              nmrU_daq_rate; //Hz
  TString               nmrU_card_name;//

protected:

  TString               t9301_fn; 
  TString               base_fn;
  TString               base_dn;
  TFile                *t9301_f;  
  TTree                *t9301;

  
  void            InitT9301Branches();
  void            InitT9301Switches();
  
private:

  void            SetT9301InputParameters();
  void            SetT9301Tree();
  void            SetT9301Branches();
  void            FillT9301Branches(Int_t _runnum);
  TH1D           *GetSignalHistogram();
  TGraphErrors   *GetSignalGraph();
  TString         GetT9301TimeString(Double_t _unixtime);


};

#endif

