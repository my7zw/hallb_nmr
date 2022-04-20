#include "T9301uevt.hh"

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

T9301uevt::T9301uevt(TString _ifn){
  fn=_ifn;
  fn_nmrU=_ifn.Remove(_ifn.Index(".csv")) + "-RawSignal.csv";
  cout<<"accepted UVA csv evnt filename: "<<endl<<fn<<endl;
  cout<<"accepted UVA csv data filename: "<<endl<<fn_nmrU<<endl;
  line_cc=0;
}

////////////////////////////////////////////////////////////////////////////////////////

T9301uevt::T9301uevt(){}
T9301uevt::~T9301uevt(){}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T9301uevt::OpenDataFile(){
  fin.open (fn.Data(), ifstream::in);
  Bool_t stat=(fin.good() && GetDataFileSize(fin));
  
  fin_nmrU.open (fn_nmrU.Data(), ifstream::in);
  Bool_t stat_nmrU=(fin_nmrU.good() && GetDataFileSize(fin_nmrU));
  
  return (stat && stat_nmrU);
}

////////////////////////////////////////////////////////////////////////////////////////

Long_t T9301uevt::GetDataFileSize(ifstream &_fin){
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

std::istream & T9301uevt::ReadDataEvent_nmrU(){
  InitCSVnmrUBranches();
  if(line_cc==0){ GetCsvLine(fin); }//read first line - header 
  if( GetCsvLine(fin) ){
    GetCsvString_nmrU(1);
    
    GetCsvVariable_nmrU(ev_num1,  0);
    GetCsvVariable_nmrU(pol,      1);
    GetCsvVariable_nmrU(area,     2);
    GetCsvVariable_nmrU(calib_c,  3);
    GetCsvVariable_nmrU(basel_fn, 4);
    GetCsvVariable_nmrU(qmeter_n, 5);
    GetCsvVariable_nmrU(qmeter_na,6);
    GetCsvVariable_nmrU(magnet_i, 7);
    GetCsvVariable_nmrU(eio_f,    8);
    GetCsvVariable_nmrU(hp_f,     9);
    GetCsvVariable_nmrU(uwave_f, 10);
    GetCsvVariable_nmrU(uwave_p, 11);
    GetCsvVariable_nmrU(fm_amp,  12);
    GetCsvVariable_nmrU(fm_off,  13);
    GetCsvVariable_nmrU(nsweep,  14);
    GetCsvVariable_nmrU(nstep,   15);
    GetCsvVariable_nmrU(daq_f,   16);
    GetCsvVariable_nmrU(f0,      17);
    GetCsvVariable_nmrU(fmod,    18);
    GetCsvVariable_nmrU(rf_power,19);
    GetCsvVariable_nmrU(gain,    20);
    GetCsvVariable_nmrU(dc_offs, 21);
    GetCsvVariable_nmrU(he3_p,   22);
    GetCsvVariable_nmrU(he3_t,   23);
    GetCsvVariable_nmrU(he4_t,   24);
    GetCsvVariable_nmrU(he4_p,   25);
    GetCsvVariable_nmrU(sep_f,   26);
    GetCsvVariable_nmrU(main_f,  27);
    GetCsvVariable_nmrU(mag_lev, 28);
    GetCsvVariable_nmrU(rf_v,    29);
    GetCsvVariable_nmrU(ln2_lev, 30);
    GetCsvVariable_nmrU(vac_p,   31);
    GetCsvVariable_nmrU(cmnt,    32);
    GetCsvVariable_nmrU(top_chip,33);
    GetCsvVariable_nmrU(bot_chip,34);
    GetCsvVariable_nmrU(C_glass, 35);
    GetCsvVariable_nmrU(collect, 36);
    
    if( GetCsvLine(fin_nmrU) ){
      GetCsvString_nmrU(2);

      GetCsvVariable_nmrU(ev_num2, 0);
      if(ev_num1!=ev_num2){
	cout<<"UVA evnt number mismatch "<<ev_num1<<" "<<ev_num2<<endl;
	//cout<<"will write zeros for UVA evnt "<<ev_num1<<" "<<ev_num2<<endl;
	//InitCSVnmrUBranches();
	exit(222);
      }
      //else{
	for(Int_t i=0;i<nstep;i++){
	  Double_t v;
	  GetCsvVariable_nmrU(v, i+1);
	  sig.push_back(v);
	}
	//}
    }
  }
  
  return fin;
}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T9301uevt::GetCsvLine(ifstream &_fin){
  Bool_t res = line.ReadLine(_fin,0).good();
  line_cc++;
  if(res){ line.Remove(line.Index("\r")); }
  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301uevt::GetCsvString_nmrU(Int_t _sel){
  Ssiz_t pos=0;
  csv_arr.clear();
  while(pos>=0){
    pos = line.Index(",");
    TString var = line;
    if(pos>=0){var.Remove(pos);}//first chunk before ","
    csv_arr.push_back(var);
    line.Remove(0,pos+1);//remove first chunk with ","
  }

  if(_sel==1 && csv_arr.size()!=37){
    cout<<"csv evnt line is bad "<<"37 is not "<<csv_arr.size()<<endl; exit(222);}
  if(_sel==2 && csv_arr.size()!=nstep+1){
    cout<<"csv data line is bad "<<nstep<<" is not "<<csv_arr.size()<<endl; exit(222);}
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301uevt::GetCsvVariable_nmrU(TString &_var, Int_t _varPosRow){
  if(csv_arr.at(_varPosRow).EqualTo("")){_var=" ";}
  else{_var=csv_arr.at(_varPosRow);}
  //cout<<_varPosRow<<" "<<setprecision(0)<<setw(15)<<_var<<endl;
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301uevt::GetCsvVariable_nmrU(Int_t &_var, Int_t _varPosRow){
  Double_t res=0.;
  if(!csv_arr.at(_varPosRow).EqualTo("")){
    csv_arr.at(_varPosRow).ReplaceAll(" ","");
    if(csv_arr.at(_varPosRow).IsFloat()){
      res=csv_arr.at(_varPosRow).Atof();
      if(res!=floor(res)){cout<<"cvs var is not an integer: "<<line<<endl; exit(222);}
    }
    else{ cout<<"cvs var is not a number: "<<line<<endl; exit(222);}
  }
  _var=Int_t(res);
  //cout<<_varPosRow<<" "<<setprecision(0)<<setw(15)<<_var<<endl;
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301uevt::GetCsvVariable_nmrU(Double_t &_var, Int_t _varPosRow){
  Double_t res=0.;
  if((!csv_arr.at(_varPosRow).EqualTo("") && !csv_arr.at(_varPosRow).EqualTo("NaN"))){
    csv_arr.at(_varPosRow).ReplaceAll(" ","");
    if(csv_arr.at(_varPosRow).IsFloat()){ res=csv_arr.at(_varPosRow).Atof(); }
    else{ cout<<"cvs var is not a double: "<<line<<endl; exit(222);}
  }
  _var=res;
  //cout<<_varPosRow<<" "<<fixed<<setprecision(7)<<setw(15)<<_var<<endl;
}
 
////////////////////////////////////////////////////////////////////////////////////////

void T9301uevt::CloseFile(){
  fin.close();
  fin_nmrU.close();
}

////////////////////////////////////////////////////////////////////////////////////////

void T9301uevt::InitCSVnmrUBranches(){

  ev_num1     = 0;
  pol         = 0;
  area        = 0;
  calib_c     = 0;
  basel_fn    = 0;
  qmeter_n    = 0;
  qmeter_na   = "";
  magnet_i    = 0;
  eio_f       = 0;
  hp_f        = 0;
  uwave_f     = 0;
  uwave_p     = 0;
  fm_amp      = 0;
  fm_off      = 0;
  nsweep      = 0;
  nstep       = 0;
  daq_f       = 0;
  f0          = 0;
  fmod        = 0;
  rf_power    = 0;
  gain        = 0;
  dc_offs     = 0;
  he3_p       = 0;
  he3_t       = 0;
  he4_t       = 0;
  he4_p       = 0;
  sep_f       = 0;
  main_f      = 0;
  mag_lev     = 0;
  rf_v        = 0;
  ln2_lev     = 0;
  vac_p       = 0;
  cmnt        = "";
  top_chip    = 0;
  bot_chip    = 0;
  C_glass     = 0;
  collect     = 0;
  ev_num2     = 0;
  sig.clear();
}


////////////////////////////////////////////////////////////////////////////////////

