#include "T9301levt.hh"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <TObjArray.h>
#include <TObjString.h>
 
using std::cout;
using std::endl;
using std::cerr;
using std::ios;
using namespace std;

T9301levt::T9301levt(TString _ifn){
  fn=_ifn; 
  cout<<"accepted LANL csv evnt filename: "<<endl<<fn<<endl;
  line_cc=0;

}

////////////////////////////////////////////////////////////////////////////////////////

T9301levt::T9301levt(){}
T9301levt::~T9301levt(){}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T9301levt::OpenDataFile(){
  fin.open (fn.Data(), ifstream::in);
  Bool_t stat=(fin.good() && GetDataFileSize(fin));
  return stat;
}

////////////////////////////////////////////////////////////////////////////////////////

Long_t T9301levt::GetDataFileSize(ifstream &_fin){
  _fin.seekg(0, ios::beg);
  Long_t begin = _fin.tellg();
  _fin.seekg(0, ios::end);
  Long_t  end = _fin.tellg();
  _fin.seekg(0, ios::beg);
  Long_t file_size=end-begin;
  //cout<<"size "<<file_size<<endl;
  return file_size;
}

////////////////////////////////////////////////////////////////////////////////////////

std::istream & T9301levt::ReadDataEvent_nmrL(){
  InitCSVnmrLBranches();
  
  GetCsvVariable_nmrL(run_cmnt);
  GetCsvVariable_nmrL(qc_fn);
  GetCsvVariable_nmrL(qc_cmnt);
  GetCsvVariable_nmrL(teq_fn);
  GetCsvVariable_nmrL(teq_cmnt);
  GetCsvVariable_nmrL(tune_fn);
  GetCsvVariable_nmrL(blank_7);
  GetCsvVariable_nmrL(blank_8);
  GetCsvVariable_nmrL(py_freqL);
  GetCsvVariable_nmrL(py_freqH);
  GetCsvVariable_nmrL(time1);
  GetCsvVariable_nmrL(time2);
  GetCsvVariable_nmrL(f0);
  GetCsvVariable_nmrL(fstep);
  GetCsvVariable_nmrL(nstep);
  GetCsvVariable_nmrL(nsweep);
  GetCsvVariable_nmrL(card_t);
  GetCsvVariable_nmrL(card_psv);
  GetCsvVariable_nmrL(tune_v);
  GetCsvVariable_nmrL(offs_v);
  GetCsvVariable_nmrL(phase_v);
  GetCsvVariable_nmrL(peak_a);
  GetCsvVariable_nmrL(qc_amp);
  GetCsvVariable_nmrL(calib_c);
  GetCsvVariable_nmrL(gain);
  GetCsvVariable_nmrL(pol_sign);
  GetCsvVariable_nmrL(log_ch);
  GetCsvVariable_nmrL(peak_amp);
  GetCsvVariable_nmrL(card_n);
  GetCsvVariable_nmrL(peak_cntr);
  GetCsvVariable_nmrL(beam_on);
  GetCsvVariable_nmrL(rf_power);
  GetCsvVariable_nmrL(if_attn);
  GetCsvVariable_nmrL(he4_t);
  GetCsvVariable_nmrL(he4_p);

  for(Int_t i=0;i<nstep;i++){
    Double_t v;
    GetCsvVariable_nmrL(v);
    sig.push_back(v);
  }

  return fin;
}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T9301levt::GetCsvLine(){
  Bool_t res = line.ReadLine(fin,0).good();
  line_cc++;
  if(res){
    line.Remove(line.Index("\r"));
    if(line.Length()<0 || line.Length()>100){//100 characters per line -lanl header
      cout<<"cvs size is wrong: "<<line.Length()<<" in: "<<line<<endl; exit(222);}
  }
  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301levt::GetCsvVariable_nmrL(TString &_var){
  if(GetCsvLine()){
    _var=line;
    //cout<<line_cc<<" "<<setprecision(0)<<setw(15)<<_var<<endl;
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301levt::GetCsvVariable_nmrL(Int_t &_var){
  Double_t res=0;
  if(GetCsvLine()){
    line.ReplaceAll(" ","");
    if(line.IsFloat()){
      res = line.Atof();
      if(res!=floor(res)){cout<<"cvs var is not an integer: "<<line<<endl; exit(222);}
    }
    else{cout<<"cvs var is not a number: "<<line<<endl; exit(222); }
    _var=Int_t(res);
    //cout<<line_cc<<" "<<setprecision(0)<<setw(15)<<_var<<endl;
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301levt::GetCsvVariable_nmrL(Double_t &_var){
  Double_t res=0.;
  if(GetCsvLine()){
    line.ReplaceAll(" ","");
    if(line.IsFloat()){ res = line.Atof(); }
    else{ cout<<"cvs var is not a number: "<<line<<endl; exit(222);}
    _var=res;
    //cout<<line_cc<<" "<<setprecision(7)<<setw(15)<<_var<<endl;
  }
}
 
////////////////////////////////////////////////////////////////////////////////////////

void T9301levt::CloseFile(){
  fin.close();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301levt::InitCSVnmrLBranches(){
  run_cmnt    = "";
  qc_fn       = "";
  qc_cmnt     = "";
  teq_fn      = "";
  teq_cmnt    = "";
  tune_fn     = "";
  blank_7     = "";
  blank_8     = "";
  py_freqL    = 0;
  py_freqH    = 0;
  time1       = 0;
  time2       = 0;
  f0          = 0;
  fstep       = 0;
  nstep       = 0;
  nsweep      = 0;
  card_t      = 0;
  card_psv    = 0;
  tune_v      = 0;
  offs_v      = 0;
  phase_v     = 0;
  peak_a      = 0;
  qc_amp      = 0;
  calib_c     = 0;
  gain        = 0;
  pol_sign    = 0;
  log_ch      = 0;
  peak_amp    = 0;
  card_n      = 0;
  peak_cntr   = 0;
  beam_on     = 0;
  rf_power    = 0;
  if_attn     = 0;
  he4_t       = 0;
  he4_p       = 0;
  sig.clear();
}

////////////////////////////////////////////////////////////////////////////////////////


