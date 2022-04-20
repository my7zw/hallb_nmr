#include "T7301theq.hh"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>

#include <TFile.h>
#include <TStyle.h>
#include <TMath.h>
#include <Math/MinimizerOptions.h>

using std::cout;
using std::endl;

T7301theq::T7301theq(TString _in_fn){
  InitT7301Variables();

  nucleon_mag_moment  = 5.05078353e-27;  //SI J/T
  boltzmann           = 1.38064852e-23 ; // SI units J/K

  proton_g            =  5.585694702;
  neutron_g           = -3.82608545;
  deuteron_g          = 0.85741; // needs to be multiplied with nucleon mag moment (kind of a g factor)
  proton_fact         = nucleon_mag_moment * proton_g /boltzmann;
  deuteron_fact       =  nucleon_mag_moment * deuteron_g /boltzmann;
  
  //setup the coefficients for P to T calculation
  aLowT[0] =  1.392408;
  aLowT[1] =  0.527153;
  aLowT[2] =  0.166756;
  aLowT[3] =  0.050988;
  aLowT[4] =  0.026514;
  aLowT[5] =  0.001975;
  aLowT[6] = -0.017976;
  aLowT[7] =  0.005409;
  aLowT[8] =  0.013259;
  aLowT[9] =  0.0;
  bLow     =  5.6;
  cLow     =  2.9;

  aHighT[0] =  3.146631;
  aHighT[1] =  1.357655;
  aHighT[2] =  0.413923;
  aHighT[3] =  0.091159;
  aHighT[4] =  0.016349;
  aHighT[5] =  0.001826;
  aHighT[6] = -0.004325;
  aHighT[7] = -0.004973;
  aHighT[8] =  0.0;
  aHighT[9] =  0.0;
  bHigh     = 10.3;
  cHigh     = 1.9;
  
  // setup TGparh for our later interpolation
  Double_t tt[13] = {0.650, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
		     1.0, 1.05, 1.1, 1.15, 1.2, 1.25};
  Double_t pp[13] = {0.1101, 0.2923, 0.6893, 1.475, 2.914, 5.380, 9.381,
		     15.58, 24.79, 38.02, 56.47, 81.52, 114.7};
  lowT = new TGraph(13,pp,tt);

  
  gStyle->SetTimeOffset(0);
}

////////////////////////////////////////////////////////////////////////////////////////

T7301theq::T7301theq(){ InitT7301Variables(); }
T7301theq::~T7301theq(){}

//CleanT7301();

////////////////////////////////////////////////////////////////////////////////////////

Double_t T7301theq::CalcT(Double_t _p){
  //Calculates Temp as a function of pressure in Torr
  //follows Journal of Physical and Chemical Ref. Data 27, 1217 (1998)
  
  Double_t temp =0.;
  Double_t pa = _p*133.322; // convert pressure into pascal
  
  if(_p>.0009 && _p<.826){
    temp = lowT->Eval(pa,0,"S"); }  //cubic spline
  else if(_p>=.826 && _p<37.82){
    temp = CalculateT(aLowT,bLow,cLow,pa);}
  else if(_p>=37.82 && _p<1471.){
    temp = CalculateT(aHighT,bHigh,cHigh,pa);}
  
  return temp;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T7301theq::CalculateT(Double_t *_a, Double_t _b, Double_t _c, Double_t _p){
  //calculate temperature
  Double_t con = (TMath::Log(_p)-_b)/_c;
  Double_t t_i = 0.0;
  for(Int_t k=0 ;k<10;k++){
    t_i = t_i+_a[k]*pow(con,k);
  }
  return t_i;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T7301theq::CalculateTEP(TString _particle, Double_t _S, Double_t _B, Double_t _p){
  // this routine calculates the TE polarization for a pressure P and a give field       
  // It calculates the Brouillouiin  function for p and d currently                      
  Double_t TEPol;
  Double_t fact =0.; // calculates the constants                                         
  Double_t Temp = CalcT(_p); // get temperature from calculation                   
  Double_t arg1 = (2.*_S+1.)/(2.*_S);
  Double_t arg2 = (1.)/(2.*_S);
  if(_particle.EqualTo("proton")){ fact = proton_fact *_S *_B /Temp; }
  else if(_particle.EqualTo("deuteron")){ fact = deuteron_fact *_S *_B /Temp; }
  else{ cout<<"No particle found for TE calculation************\n\n\n ";}

  TEPol = arg1*cosh(arg1*fact)/sinh(arg1*fact)-arg2*cosh(arg2*fact)/sinh(arg2*fact);

  return TEPol;
}

////////////////////////////////////////////////////////////////////////////////////////

void T7301theq::ReadPressureFrom(TString _fn){
  db = new T9301read(_fn);
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T7301theq::GetPressureForTimestamp(Double_t _time){
  Double_t res = 0.;
  Int_t nev = db->GetT9301Entries();
  std::vector<Double_t> time_dev; time_dev.clear();
  std::vector<Int_t> time_ind; time_ind.clear();
  Int_t cc=0;
  Double_t time_gap = 60;
  for(Int_t i=0;i<nev;i++){
    db->GetT9301Entry(i);
    Double_t dev = fabs(db->ev_time-_time);
    if(i==0 && db->ev_time>_time){cout<<"prior to file start time"<<endl; res=-1.;}
    if(i==nev-1 && db->ev_time<_time){cout<<"beyond file end time"<<endl; res=-2.;}
    if(dev<time_gap){
      time_dev.push_back(dev);
      time_ind.push_back(i);
    }
  }

  if(time_dev.size()!=0){
    Int_t close=TMath::LocMin(time_dev.size(), time_dev.data() );
    db->GetT9301Entry(time_ind.at(close));
    res = db->ev_4He_p;
  }
  else{res=-3.;}

  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

void T7301theq::InitT7301Variables(){

}

////////////////////////////////////////////////////////////////////////////////////////
