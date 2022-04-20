#include "T9301read.hh"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>

#include "T9301make.hh"

using std::cout;
using std::endl;

T9301read::T9301read(TString _in_fn):T9301make(_in_fn){
  InitT9301Branches();
  p_data_sig        = 0;
  p_ev_time_str     = 0;
  p_nmrU_card_name  = 0;

  ReadT9301Tree();
}

////////////////////////////////////////////////////////////////////////////////////////

T9301read::T9301read(){
  t9301_fn="tpe_db_temp.root";
  InitT9301Branches();
}
T9301read::~T9301read(){
  CleanT9301();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301read::SetT9301InputParameters(){
  base_fn = t9301_fn;
  base_dn = t9301_fn;
  base_fn.Remove(0, 1+base_fn.Last('/'));
  base_fn.Remove(base_fn.Index(".root"));
  base_dn.Remove(1+base_dn.Last('/'));
  cout<<"base filename "<<endl;
  cout<<"   "<<base_fn<<endl;

  InitT9301Switches();
  
  if(base_fn.Contains("QCV") || base_fn.Contains("TEQ") || base_fn.Contains("POL")){
    is_nmrL=1;
    cout<<"read input is identified as LANL file format"<<endl;
  }
  if(base_fn.Contains("h")&&base_fn.Contains("m")&&base_fn.Contains("s")){
    cout<<"read input is identified as UVA file format"<<endl;
    is_nmrU=1;
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301read::ReadT9301Tree(){
  SetT9301InputParameters();
  t9301_f  = new TFile(t9301_fn,"READ");
  t9301    = (TTree *)t9301_f->Get("T9301");
  SetT9301BranchesAddress();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301read::SetT9301BranchesAddress(){
  t9301->SetBranchAddress("ev_time",       &ev_time);
  t9301->SetBranchAddress("ev_time_str",   &p_ev_time_str);
  t9301->SetBranchAddress("ev_id",         &ev_id);
  t9301->SetBranchAddress("ev_count",      &ev_count);
  t9301->SetBranchAddress("ev_4He_p",      &ev_4He_p);
  t9301->SetBranchAddress("ev_4He_t",      &ev_4He_t);
  
  t9301->SetBranchAddress("rf_f0",         &rf_f0);
  t9301->SetBranchAddress("rf_fstep",      &rf_fstep);
  t9301->SetBranchAddress("rf_nstep",      &rf_nstep);
  t9301->SetBranchAddress("rf_power",      &rf_power);
  t9301->SetBranchAddress("rf_mod",        &rf_mod);
  t9301->SetBranchAddress("rf_s",          &rf_s);
  t9301->SetBranchAddress("rf_e",          &rf_e);
  
  t9301->SetBranchAddress("daq_card_n",    &daq_card_n);
  t9301->SetBranchAddress("daq_gain",      &daq_gain);
  t9301->SetBranchAddress("daq_dcoff_v",   &daq_dcoff_v);
  
  t9301->SetBranchAddress("onl_calib_c",   &onl_calib_c);
  t9301->SetBranchAddress("onl_peak_area", &onl_peak_area);

  t9301->SetBranchAddress("data_np",       &data_np);
  t9301->SetBranchAddress("data_nsweep",   &data_nsweep);
  t9301->SetBranchAddress("data_sig",      &p_data_sig);
  t9301->SetBranchAddress("data_hSvsF",    &data_hSvsF);
  t9301->SetBranchAddress("data_gSvsF",    &data_gSvsF);

  if(is_nmrL){
  t9301->SetBranchAddress("nmrL_card_t",   &nmrL_card_t);
  t9301->SetBranchAddress("nmrL_card_psv", &nmrL_card_psv);
  t9301->SetBranchAddress("nmrL_tune_v",   &nmrL_tune_v);
  t9301->SetBranchAddress("nmrL_if_attn",  &nmrL_if_attn);
  t9301->SetBranchAddress("nmrL_phase_v",  &nmrL_phase_v);
  t9301->SetBranchAddress("nmrL_log_ch",   &nmrL_phase_v);
  t9301->SetBranchAddress("nmrL_pol_sign", &nmrL_pol_sign);
  t9301->SetBranchAddress("nmrL_py_hcut",  &nmrL_py_hcut);
  t9301->SetBranchAddress("nmrL_py_lcut",  &nmrL_py_lcut);
  
  t9301->SetBranchAddress("nmrL_base_amp", &nmrL_base_amp);
  t9301->SetBranchAddress("nmrL_peak_amp", &nmrL_peak_amp);
  t9301->SetBranchAddress("nmrL_peak_cntr",&nmrL_peak_cntr);
  t9301->SetBranchAddress("nmrL_beam_on",  &nmrL_beam_on);
  }
  
  if(is_nmrU){
  t9301->SetBranchAddress("nmrU_pol",      &nmrU_pol);
  t9301->SetBranchAddress("nmrU_base_time",&nmrU_base_time);
  t9301->SetBranchAddress("nmrU_daq_rate", &nmrU_daq_rate);
  t9301->SetBranchAddress("nmrU_card_name",&p_nmrU_card_name);
  }  
}

////////////////////////////////////////////////////////////////////////////////////////

Int_t T9301read::GetT9301Entries(){
  return Int_t(t9301->GetEntries());
}

////////////////////////////////////////////////////////////////////////////////////////

Int_t T9301read::GetT9301Entry(Long64_t _entry){
  return t9301->GetEntry(_entry);
}

////////////////////////////////////////////////////////////////////////////////////////

Int_t T9301read::GetT9301EntryWithIndex(Int_t _index_kn){
  return t9301->GetEntryWithIndex(_index_kn);//to read TTree with index add -lTreePlayer lib     
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301read::ShowEvent(Long64_t _entry){
  t9301->Show(_entry);
}

////////////////////////////////////////////////////////////////////////////////////////
