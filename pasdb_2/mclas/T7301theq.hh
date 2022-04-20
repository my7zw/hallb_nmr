#ifndef T7301theq_hh 
#define T7301theq_hh 

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TF1.h>
#include <TH1D.h>

#include "T9301read.hh"

class T7301theq{

public :   
 
  T7301theq(TString _in_fn); 
  T7301theq();
  ~T7301theq();

  Double_t          CalcT(Double_t _p);
  Double_t          CalculateT(Double_t *_a, Double_t _b, Double_t _c, Double_t _p);
  Double_t          CalculateTEP(TString _particle, Double_t _S, Double_t _B, Double_t _p);
  void              ReadPressureFrom(TString _fn);
  Double_t          GetPressureForTimestamp(Double_t _time);
  
  Double_t nucleon_mag_moment;
  Double_t boltzmann ; //J/K                                                            

  Double_t proton_g;
  Double_t neutron_g;
  Double_t deuteron_g;
  Double_t proton_fact;
  Double_t deuteron_fact;

  // next two lines are for the time vs pressure data
  std::map<ULong64_t, Float_t> TEmap;
  std::map<ULong64_t, Float_t>::iterator itlow,ithigh;
  
  // coefficiewnts for helium pressure to T calculation                                  
  Double_t aLowT[10];
  Double_t bLow;
  Double_t cLow;
  Double_t aHighT[10];
  Double_t bHigh;
  Double_t cHigh;
  TGraph  *lowT;  // used for temp calulation at low pressure                             
  
protected:
  
  TFile              *blout_f;
  T9301read *db;
  
private:
  
  void          InitT7301Variables();


};

#endif

