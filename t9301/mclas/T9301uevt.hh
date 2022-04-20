#ifndef T9301uevt_hh
#define T9301uevt_hh

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


class T9301uevt {
 
public :   

  T9301uevt(TString _ifn); 
  T9301uevt();
  ~T9301uevt();
  
  Bool_t     OpenDataFile();
  Long_t     GetDataFileSize(ifstream &_fin);

  std::istream & ReadDataEvent_nmrU();
  Bool_t     GetCsvLine(ifstream &_fin);
  void       GetCsvString_nmrU(Int_t _sel);

  void       GetCsvVariable_nmrU(TString &_var, Int_t _varPosRow);
  void       GetCsvVariable_nmrU(Int_t &_var, Int_t _varPosRow);
  void       GetCsvVariable_nmrU(Double_t &_var, Int_t _varPosRow);
  
  void       CloseFile();

  Double_t   ev_num1;
  Double_t   pol;
  Double_t   area;
  Double_t   calib_c;
  Double_t   basel_fn;
  Double_t   qmeter_n;
  TString    qmeter_na;
  Double_t   magnet_i;
  Double_t   eio_f;
  Double_t   hp_f;
  Double_t   uwave_f;
  Double_t   uwave_p;
  Double_t   fm_amp;
  Double_t   fm_off;
  Int_t      nsweep;
  Int_t      nstep;
  Double_t   daq_f;
  Double_t   f0;
  Double_t   fmod;
  Double_t   rf_power;
  Double_t   gain;
  Double_t   dc_offs;
  Double_t   he3_p;
  Double_t   he3_t; 
  Double_t   he4_t; 
  Double_t   he4_p; 
  Double_t   sep_f;
  Double_t   main_f;
  Double_t   mag_lev;
  Double_t   rf_v;
  Double_t   ln2_lev;
  Double_t   vac_p;
  TString    cmnt;
  Double_t   top_chip;
  Double_t   bot_chip;
  Double_t   C_glass;
  Double_t   collect;
  Double_t   ev_num2;
  std::vector<Double_t> sig;
  
protected:

  void       InitCSVnmrUBranches();
  
private:   
  
  TString    fn;
  TString    fn_nmrU; 

  ifstream   fin;
  ifstream   fin_nmrU;

  TString    line;
  Int_t      line_cc;
  std::vector<TString> csv_arr;

};

#endif

