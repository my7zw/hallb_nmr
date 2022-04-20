#include "T9301make.hh"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>

#include "T9301uevt.hh"
#include "T9301levt.hh"

using namespace std;

using std::cout;
using std::endl;
using std::cerr;

T9301make::T9301make(TString _in_fn){
  t9301_fn=_in_fn;
  cout<<"received filename: "<<endl;
  cout<<"   "<<t9301_fn<<endl;

}

////////////////////////////////////////////////////////////////////////////////////////

T9301make::T9301make(){
  t9301_fn="tpe_db_temp.root";
}
T9301make::~T9301make(){
  CleanT9301();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::CleanT9301(){
  t9301->Delete();
  t9301_f->Close();
  t9301_f->Delete();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::ExitT9301(Int_t _err_ind){
  CleanT9301();
  Int_t err_ind=_err_ind; 
  exit(err_ind);
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::SetT9301InputParameters(){
  base_fn = t9301_fn;
  base_dn = t9301_fn;
  base_fn.Remove(0, 1+base_fn.Last('/'));
  base_fn.Remove(base_fn.Index(".csv"));
  base_dn.Remove(1+base_dn.Last('/'));
  cout<<"base filename "<<endl;
  cout<<"   "<<base_fn<<endl;
  cout<<"base dirname "<<endl;
  cout<<"   "<<base_dn<<endl;

  InitT9301Switches();

  if(base_fn.Contains("QCV") || base_fn.Contains("TEQ") || base_fn.Contains("POL")){
    is_nmrL=1;
    cout<<"input is identified as LANL file format"<<endl;
  }
  if(base_fn.Contains("h")&&base_fn.Contains("m")&&base_fn.Contains("s")){
    cout<<"input is identified as UVA file format"<<endl;
    is_nmrU=1;
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::SetT9301Tree(){
  SetT9301InputParameters();
    
  ev_count         = 0;
  TString out_fn = t9301_fn;
  out_fn = out_fn.Remove(out_fn.Index(".")) + ".root";
  cout<<"output filename "<<endl;
  cout<<"   "<<out_fn<<endl;

  t9301_f  = new TFile(out_fn,"RECREATE","T9301 with NMR data");
  t9301    = new TTree("T9301","T9301 tree");
  SetT9301Branches();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::SetT9301Branches(){
  t9301->Branch("ev_time",             &ev_time,        "ev_time/D");
  t9301->Branch("ev_time_str",         &ev_time_str);
  t9301->Branch("ev_id",               &ev_id,          "ev_id/D");
  t9301->Branch("ev_count",            &ev_count,       "ev_count/I");
  t9301->Branch("ev_4He_p",            &ev_4He_p,       "ev_4He_p/D");
  t9301->Branch("ev_4He_t",            &ev_4He_t,       "ev_4He_t/D");
  
  t9301->Branch("rf_f0",               &rf_f0,          "rf_f0/D");
  t9301->Branch("rf_fstep",            &rf_fstep,       "rf_fstep/D");
  t9301->Branch("rf_nstep",            &rf_nstep,       "rf_nstep/D");
  t9301->Branch("rf_power",            &rf_power,       "rf_power/D");
  t9301->Branch("rf_mod",              &rf_mod,         "rf_mod/D");
  t9301->Branch("rf_s",                &rf_s,           "rf_s/D");
  t9301->Branch("rf_e",                &rf_e,           "rf_e/D");
  
  t9301->Branch("daq_card_n",          &daq_card_n,     "daq_card_n/D");
  t9301->Branch("daq_gain",            &daq_gain,       "daq_gain/D");
  t9301->Branch("daq_dcoff_v",         &daq_dcoff_v,    "daq_dcoff_v/D");
  
  t9301->Branch("onl_calib_c",         &onl_calib_c,    "onl_calib_c/D");
  t9301->Branch("onl_peak_area",       &onl_peak_area,  "onl_peak_area/D");

  t9301->Branch("data_np",             &data_np,        "data_np/I");
  t9301->Branch("data_nsweep",         &data_nsweep,    "data_nsweep/I");
  t9301->Branch("data_sig",            &data_sig);
  //t9301->Branch("data_hSvsF", "TH1D",         &data_hSvsF, 128000, 0);
  //t9301->Branch("data_gSvsF", "TGraphErrorsu", &data_gSvsF, 128000, 0);
  t9301->Branch("data_hSvsF",          &data_hSvsF);
  t9301->Branch("data_gSvsF",          &data_gSvsF);

  if(is_nmrL){
  t9301->Branch("nmrL_card_t",         &nmrL_card_t,    "nmrL_card_t/D");
  t9301->Branch("nmrL_card_psv",       &nmrL_card_psv,  "nmrL_card_psv/D");
  t9301->Branch("nmrL_tune_v",         &nmrL_tune_v,    "nmrL_tune_v/D");
  t9301->Branch("nmrL_if_attn",        &nmrL_if_attn,   "nmrL_if_attn/D");
  t9301->Branch("nmrL_phase_v",        &nmrL_phase_v,   "nmrL_phase_v/D");
  t9301->Branch("nmrL_log_ch",         &nmrL_phase_v,   "nmrL_phase_v/D");
  t9301->Branch("nmrL_pol_sign",       &nmrL_pol_sign,  "nmrL_pol_sign/D");
  t9301->Branch("nmrL_py_hcut",        &nmrL_py_hcut,   "nmrL_py_hcut/D"); 
  t9301->Branch("nmrL_py_lcut",        &nmrL_py_lcut,   "nmrL_py_lcut/D");
  
  t9301->Branch("nmrL_base_amp",       &nmrL_base_amp,  "nmrL_base_amp/D");
  t9301->Branch("nmrL_peak_amp",       &nmrL_peak_amp,  "nmrL_peak_amp/D");
  t9301->Branch("nmrL_peak_cntr",      &nmrL_peak_cntr, "nmrL_peak_cntr/D"); 
  t9301->Branch("nmrL_beam_on",        &nmrL_beam_on,   "nmrL_beam_on/D");
  }
  
  if(is_nmrU){
  t9301->Branch("nmrU_pol",            &nmrU_pol,       "nmrU_pol/D");
  t9301->Branch("nmrU_base_time",      &nmrU_base_time, "nmrU_base_time/D");
  t9301->Branch("nmrU_daq_rate",       &nmrU_daq_rate,  "nmrU_daq_rate/D");
  t9301->Branch("nmrU_card_name",      &nmrU_card_name);
  }  
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::FillT9301Tree(){
  SetT9301Tree();

  if(is_nmrL){
    T9301levt *Lnmr = new T9301levt(t9301_fn);
    if(!Lnmr->OpenDataFile()){exit(333);}
    while(  Lnmr->ReadDataEvent_nmrL() ){

      InitT9301Branches();

      ev_time            = 1.e5*Lnmr->time1 + Lnmr->time2 - 2082844800;
      ev_time_str        = GetT9301TimeString(ev_time);
      ev_id              = Lnmr->nsweep;          
      ev_4He_p           = Lnmr->he4_p;      
      ev_4He_t           = Lnmr->he4_t;
  
      rf_f0              = Lnmr->f0;
      //rf_fstep           = Lnmr->fstep;
      rf_fstep           = 0.00125;      
      rf_nstep           = Lnmr->nstep;      
      rf_power           = Lnmr->rf_power;      
      //rf_mod             = Lnmr->nstep*Lnmr->fstep/2.;
      rf_mod             = rf_nstep*rf_fstep/2.;        
      rf_s               = rf_f0 - rf_nstep*rf_fstep/2.;
      rf_e               = rf_f0 + rf_nstep*rf_fstep/2.;

      daq_card_n         = Lnmr->card_n;
      daq_gain           = Lnmr->gain; 
      daq_dcoff_v        = Lnmr->offs_v;   
      
      onl_calib_c        = Lnmr->calib_c;
      onl_peak_area      = Lnmr->peak_a; 

      data_np            = Lnmr->nstep;
      data_nsweep        = 20;
      data_sig           = Lnmr->sig;
      data_hSvsF         = GetSignalHistogram();
      data_gSvsF         = GetSignalGraph();

      nmrL_card_t        = Lnmr->card_t;
      nmrL_card_psv      = Lnmr->card_psv;
      nmrL_tune_v        = Lnmr->tune_v;
      nmrL_if_attn       = Lnmr->if_attn;
      nmrL_phase_v       = Lnmr->phase_v;
      nmrL_log_ch        = Lnmr->log_ch; 
      nmrL_pol_sign      = Lnmr->pol_sign; 
      nmrL_py_hcut       = Lnmr->py_freqL; 
      nmrL_py_lcut       = Lnmr->py_freqH; 
      
      nmrL_base_amp      = Lnmr->qc_amp; 
      nmrL_peak_amp      = Lnmr->peak_amp; 
      nmrL_peak_cntr     = Lnmr->peak_cntr;
      nmrL_beam_on       = Lnmr->beam_on;

      ev_count++;
      cout<<"Filling DB tree for event---->: ";
      cout<<std::fixed<<std::setprecision(0)<<std::setw(20)
	  <<ev_id<<" "<<std::setw(15)<<ev_count<<endl;
      t9301->Fill();

      data_hSvsF->Delete();
      data_gSvsF->Delete();
    }
    Lnmr->CloseFile();
  }
  
  if(is_nmrU){
    T9301uevt *Unmr = new T9301uevt(t9301_fn);
    if(!Unmr->OpenDataFile()){exit(333);}
    while(  Unmr->ReadDataEvent_nmrU() ){

      InitT9301Branches();
      
      ev_time            = Unmr->ev_num1;
      ev_time_str        = GetT9301TimeString(ev_time);
      ev_id              = Unmr->ev_num1;          
      ev_4He_p           = Unmr->he4_p;      
      ev_4He_t           = Unmr->he4_t;
  
      rf_f0              = Unmr->f0;
      rf_fstep           = Unmr->fmod*2./1000./Unmr->nstep;
      rf_nstep           = Unmr->nstep;      
      rf_power           = Unmr->rf_power;      
      rf_mod             = Unmr->fmod/1000.;
      rf_s               = rf_f0 - rf_nstep*rf_fstep/2.;
      rf_e               = rf_f0 + rf_nstep*rf_fstep/2.;

      daq_card_n         = Unmr->qmeter_n;
      daq_gain           = Unmr->gain; 
      daq_dcoff_v        = Unmr->dc_offs;
      
      onl_calib_c        = Unmr->calib_c;
      onl_peak_area      = Unmr->area; 

      data_np            = Unmr->nstep;
      data_nsweep        = Unmr->nsweep;
      data_sig           = Unmr->sig;
      data_hSvsF         = GetSignalHistogram();
      data_gSvsF         = GetSignalGraph();

      nmrU_pol           = Unmr->pol;      
      nmrU_base_time     = Unmr->basel_fn;      
      nmrU_daq_rate      = Unmr->daq_f;      
      nmrU_card_name     = Unmr->qmeter_na;      

      ev_count++;
      cout<<"Filling DB tree for event---->: ";
      cout<<std::fixed<<std::setprecision(0)<<std::setw(20)
	  <<ev_id<<" "<<std::setw(15)<<ev_count<<endl;
      t9301->Fill();

      data_hSvsF->Delete();
      data_gSvsF->Delete();
    }
    Unmr->CloseFile();    
  }

  t9301->BuildIndex("ev_count");
  t9301_f->Write();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::FillT9301Branches(Int_t _runnum){
  //T9301pars *var = new T9301pars("");
}

////////////////////////////////////////////////////////////////////////////////////////

TH1D * T9301make::GetSignalHistogram(){
  TString hn=Form("HSvsF_%d",ev_count);
  TH1Du *res = new TH1Du(hn, hn, data_np+1, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);

  /*for(Int_t i=0;i<data_np;i++){
    res->Fill(rf_s+i*rf_fstep, data_sig.at(i));
    //cout<<i<<" "<<rf_s+i*rf_fstep<<" "<<data_sig.at(i)<<endl;
    }*/
  res->Sumw2();
  for(Int_t i=1;i<data_np+1;i++){
    res->SetBinContent(i, data_sig.at(i-1));
    //cout<<i<<" "<<rf_s+i*rf_fstep<<" "<<data_sig.at(i-1)<<endl;
  }

  //cout<<"his head "<<data_np<<" "<<rf_s<<" "<<rf_e<<" "<<res->GetNbinsX()<<endl;
  //cout<<"his head "<<data_np<<" "<<rf_s-rf_fstep/2.<<" "
  //<<rf_e+rf_fstep/2.<<" "<<rf_e-rf_s<<" "<<endl;
  //cout<<"his head "<<data_np<<" "<<rf_s<<" "<<rf_e<<" "<<rf_nstep<<" "<<data_np<<endl;
  
  /*for(Int_t i=1;i<res->GetNbinsX();i++){
    cout<<fixed
	<<setprecision(0)<<setw(4)<<i<<" "
      	<<setprecision(7)<<setw(13)<<res->GetBinWidth(i)<<" "
	<<setprecision(7)<<setw(13)<<res->GetBinCenter(i)-res->GetBinWidth(i)/2<<" "
      	<<setprecision(7)<<setw(13)<<rf_s+(i-1)*rf_fstep<<" "
      	<<setprecision(7)<<setw(13)<<res->GetBinCenter(i)+res->GetBinWidth(i)/2<<" "
	<<setprecision(7)<<setw(13)<<res->GetBinContent(i)<<" "
	<<setprecision(7)<<setw(13)<<data_sig.at(i-1)
	<<endl;
	}*/


  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

TGraphErrors * T9301make::GetSignalGraph(){
  Double_t rf_s = rf_f0 - data_np*rf_fstep/2.;
  Double_t rf_e = rf_f0 + data_np*rf_fstep/2.;
  TString gn=Form("GSvsF_%d",ev_count);
  TGraphErrorsu *res = new TGraphErrorsu();
  res->SetName(gn);   res->SetTitle(gn);
  
  for(Int_t i=0;i<data_np;i++){
    res->SetPoint(i, rf_s+i*rf_fstep, data_sig.at(i));
    //cout<<i<<" "<<rf_s+i*rf_fstep<<" "<<data_sig.at(i)<<endl;
  }
  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

TString T9301make::GetT9301TimeString(Double_t _unixtime){
  //check mclas/readme.txt
  time_t time_stamp = _unixtime;
  char buf[100];
  strftime (buf, 100, "%F_%T", localtime(&time_stamp));
  TString time_str = buf;
  return time_str;
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::InitT9301Switches(){
  is_nmrL          = 0;
  is_nmrU          = 0;
  is_bl            = 0;
  is_te            = 0;
  is_ep            = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301make::InitT9301Branches(){
  ev_time          = 0;
  ev_id            = 0;

  ev_4He_p         = 0;
  ev_4He_t         = 0;
  
  rf_f0            = 0;
  rf_fstep         = 0;
  rf_nstep         = 0;
  rf_power         = 0;
  rf_mod           = 0;
  rf_s             = 0;
  rf_e             = 0;
  
  daq_card_n       = 0;
  daq_gain         = 0;
  daq_dcoff_v      = 0;
  
  onl_calib_c      = 0;
  onl_peak_area    = 0;

  data_np          = 0;
  data_nsweep      = 0;
  data_sig.clear();
  data_hSvsF       = 0;
  data_gSvsF       = 0;

  nmrL_card_t      = 0;
  nmrL_card_psv    = 0;
  nmrL_tune_v      = 0;
  nmrL_if_attn     = 0;
  nmrL_phase_v     = 0;
  nmrL_log_ch      = 0;
  nmrL_pol_sign    = 0;
  nmrL_py_hcut     = 0;
  nmrL_py_lcut     = 0;
  
  nmrL_base_amp    = 0;
  nmrL_peak_amp    = 0;
  nmrL_peak_cntr   = 0;
  nmrL_beam_on     = 0;
  
  nmrU_pol         = 0;
  nmrU_base_time   = 0;
  nmrU_daq_rate    = 0;
  nmrU_card_name   = "";
}

////////////////////////////////////////////////////////////////////////////////////////
