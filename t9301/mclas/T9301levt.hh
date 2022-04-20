#ifndef T9301levt_hh
#define T9301levt_hh

#include <fstream>
#include <TString.h>

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


class T9301levt {
 
public :   

  T9301levt(TString _ifn); 
  T9301levt();
  ~T9301levt();
  
  Bool_t     OpenDataFile();
  Long_t     GetDataFileSize(ifstream &_fin);

  std::istream & ReadDataEvent_nmrL();
  Bool_t     GetCsvLine();

  void       GetCsvVariable_nmrL(TString &_var);
  void       GetCsvVariable_nmrL(Int_t &_var);
  void       GetCsvVariable_nmrL(Double_t &_var);
  
  void       CloseFile();

  TString    run_cmnt;
  TString    qc_fn;
  TString    qc_cmnt;
  TString    teq_fn;
  TString    teq_cmnt;
  TString    tune_fn;
  TString    blank_7;
  TString    blank_8;
  Double_t   py_freqL;
  Double_t   py_freqH;
  Double_t   time1;
  Double_t   time2;
  Double_t   f0;
  Double_t   fstep;
  Int_t      nstep;
  Int_t      nsweep;
  Double_t   card_t;
  Double_t   card_psv;
  Double_t   tune_v;
  Double_t   offs_v;
  Double_t   phase_v;
  Double_t   peak_a;
  Double_t   qc_amp;
  Double_t   calib_c;
  Int_t      gain;
  Int_t      pol_sign;
  Int_t      log_ch;
  Double_t   peak_amp;
  Int_t      card_n;
  Double_t   peak_cntr;
  Int_t      beam_on;
  Double_t   rf_power;
  Double_t   if_attn;
  Double_t   he4_t;
  Double_t   he4_p;
  std::vector<Double_t> sig;
  
protected:
  void       InitCSVnmrLBranches();
  
private:   
  
  TString    fn;
  ifstream   fin;

  TString    line;
  Int_t      line_cc;

};

#endif

