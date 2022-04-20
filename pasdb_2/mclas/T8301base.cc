#include "T8301base.hh"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <TF1.h>
#include <TMath.h>
#include <Math/MinimizerOptions.h>

using std::cout;
using std::endl;

T8301base::T8301base(TString _in_fn):T9301read(_in_fn){
  InitT8301Variables();
}

////////////////////////////////////////////////////////////////////////////////////////

T8301base::T8301base(){ InitT8301Variables(); }
T8301base::~T8301base(){}

//CleanT8301();

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::GetInitialInfo(){
  GetT9301EntryWithIndex(1);
  //GetT9301EntryWithIndex(0);

  bl_data_np       = data_np;
  bl_daq_card_n    = daq_card_n;
  bl_daq_gain      = daq_gain;

  cout<<"number of data points: "<<data_np<<endl;
  cout<<"rf steps: "<<rf_nstep<<endl;
  cout<<"card number: "<<bl_daq_card_n<<endl;
  cout<<"card gain: "<<bl_daq_gain<<endl;

  if(is_nmrL){
    bl_nmrL_tune_v   = nmrL_tune_v;
    bl_nmrL_phase_v  = nmrL_phase_v;
    bl_nmrL_if_attn  = nmrL_if_attn;
    cout<<"tune voltage: "<<bl_nmrL_tune_v<<endl;
    cout<<"phase voltage: "<<bl_nmrL_phase_v<<endl;
    cout<<"attenuation: "<<bl_nmrL_if_attn<<endl;
  }
}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T8301base::IsEventOK(){
  Bool_t res = 1;
  Bool_t res_main = 1;
  Bool_t res_nmrL = 1;
  res_main = ( (bl_data_np    ==  data_np) &&
	       (bl_daq_card_n ==  daq_card_n) &&
	       (bl_daq_gain   ==  daq_gain) );    

  if(is_nmrL){
    res_nmrL = ( (bl_nmrL_tune_v  == nmrL_tune_v)  &&
		 (bl_nmrL_phase_v == nmrL_phase_v) &&
		 (bl_nmrL_if_attn == nmrL_if_attn) );}

  res = res_main && res_nmrL;
  if(!res){cout<<"something is wrong with baseline file"<<endl;}
  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::InitHistos(){
  //bl_hSvsF = new TH1Du("bl_h", "bl_h", data_np+1, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);
  bl_hSvsF = new TH1Du("bl_h", "bl_h", data_np, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);
  bl_hSvsF->Sumw2();
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::FillVectorAll(){ bl_sig.push_back(*p_data_sig); }

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::SetSampleSize(Int_t _ss){ bl_nn_ave = _ss; }

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T8301base::FillVectorAve(){
  if(bl_cc_ave==0        ){ bl_sig.clear();                bl_is_ave = 0; }
  if(bl_cc_ave <bl_nn_ave){ bl_sig.push_back(*p_data_sig); bl_cc_ave++;   }
  if(bl_cc_ave==bl_nn_ave){ bl_is_ave = 1;                 bl_cc_ave = 0; }
  return bl_is_ave;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::FillHistAve(){
  bl_hSvsF->Reset();
  
  Double_t fscan[bl_sig.size()] = {0.};
  cout<<"avr sample size: "<<bl_sig.size()<<endl;
  for(Int_t j=0;j<bl_sig[0].size();j++){
    Double_t sign_coef  = 1.;
    if(is_nmrU){sign_coef = -1.;}    
    for(Int_t i=0;i<bl_sig.size();i++){ fscan[i] = sign_coef*bl_sig[i].at(j); }
    
    Double_t sig_m = TMath::Mean(bl_sig.size(),fscan);
    Double_t sig_s = TMath::RMS(bl_sig.size(),fscan) / sqrt(bl_sig.size());
    if(bl_sig.size()<=1){ sig_s = 0.0003*sig_m; }
    bl_hSvsF->SetBinContent(j+1, sig_m);
    bl_hSvsF->SetBinError(j+1, sig_s);
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::CloneHistos(){
  bl_hSvsF_cl = (TH1D*)bl_hSvsF->Clone("base_clone");
  bl_hSvsF_subtr = (TH1D*)bl_hSvsF->Clone("bl_h_subtr");
  bl_hSvsF_resid = (TH1D*)bl_hSvsF->Clone("bl_h_resid");

  
  //bl_hSvsF_subtr->Sumw2();
  //bl_hSvsF_resid->Sumw2();
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::FitP2(){
  ffn_p2 = new TF1("p2", fit_p2, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 4);
  ffn_p2->SetLineColor(8);  ffn_p2->SetMarkerColor(8);
  ffn_p2->SetParameter(3, 213.);
  bl_hSvsF->Fit(ffn_p2,"MR+");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::FitConvP3(){
  ffn_conv_p3 = new TF1("conv_p3", fit_conv_p3, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 4);
  ffn_conv_p3->SetLineColor(7);   ffn_conv_p3->SetMarkerColor(7);
  ffn_conv_p3->SetParameter(3, 213.);
  bl_hSvsF_subtr->Fit(ffn_conv_p3,"MR+");  
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::FitConvP2P3(){
  ffn_conv_p2p3 = new TF1("conv_p2p3", fit_conv_p2p3, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 5);
  ffn_conv_p2p3->SetLineColor(2);  ffn_conv_p2p3->SetMarkerColor(2);
  ffn_conv_p2p3->SetParameters(ffn_p2->GetParameter(0)+ffn_conv_p3->GetParameter(0),
			       ffn_p2->GetParameter(1)+ffn_conv_p3->GetParameter(1),//lin
			       ffn_p2->GetParameter(2),//qaud
			       ffn_conv_p3->GetParameter(2),//cub
			       213.);//shift

  bl_hSvsF->Fit(ffn_conv_p2p3,"MR+same");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::GetP2FitSubtracted(){ bl_hSvsF_subtr->Add(ffn_p2,-1.); }

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::GetResidual(){
  bl_hSvsF_resid->Add(ffn_conv_p2p3,-1.);

  ffn_p2_p2p3_dif = new TGraph();
  for(Int_t i=1;i<=bl_hSvsF->GetNbinsX();i++){ 
    Double_t r1 = ffn_conv_p2p3->Eval(bl_hSvsF->GetBinCenter(i));
    Double_t r2 = ffn_p2->Eval(bl_hSvsF->GetBinCenter(i));
    ffn_p2_p2p3_dif->SetPoint(i-1,bl_hSvsF->GetBinCenter(i),100.*(r1-r2)/((r1+r2)/2));
  }
    
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301base::fit_p2(Double_t *x , Double_t *par){
  Double_t result = par[0]+par[1]*(x[0]-par[3])+par[2]*pow(x[0]-par[3],2);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301base::fit_conv_p3(Double_t *x , Double_t *par){
  //Double_t result = par[0]*(x[0]-par[2])+par[1]*pow(x[0]-par[2],3);
  Double_t result = par[0]                    +
                    par[1]*   (x[0]-par[3])   +
                    par[2]*pow(x[0]-par[3],3);

  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301base::fit_conv_p2p3(Double_t *x , Double_t *par){
  Double_t result = par[0]                    +
                    par[1]*   (x[0]-par[4])   +
                    par[2]*pow(x[0]-par[4],2) +
                    par[3]*pow(x[0]-par[4],3);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::WriteBaselineResults(){
  blout_f = new TFile("mdata/"+base_fn+"_res.root","RECREATE","T8301 with baseline data");

  bl_hSvsF->Write();
  bl_hSvsF_subtr->Write();
  bl_hSvsF_resid->Write();
  
  ffn_p2->Write();
  ffn_conv_p3->Write();
  ffn_conv_p2p3->Write();
  
  blout_f->Close();
  
  TString out_fn= "mdata/"+base_fn+"_res.txt";
  std::ofstream      fout;
  fout.open(out_fn.Data());
  if(fout.good()){
    //fout<<"baseline file name: "<<out_fn<<endl;
    fout<<"baseline file name: "<<base_fn<<endl;
    fout<<"number of rf steps: "<<bl_data_np<<endl;
    fout<<"qmeter card number: "<<bl_daq_card_n<<endl;
    fout<<"daq gain setting: "<<bl_daq_gain<<endl;
    if(is_nmrL){
    fout<<"tune voltage: "<<bl_nmrL_tune_v<<endl;
    fout<<"phase voltage: "<<bl_nmrL_phase_v<<endl;
    fout<<"attenuation setting: "<<bl_nmrL_if_attn<<endl;
    }
  }
  fout.close();
  
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::InitT8301Variables(){
  bl_sig.clear();

  bl_data_np       = 0;
  bl_daq_card_n    = 0;
  bl_daq_gain      = 0;
  bl_nmrL_tune_v   = 0;
  bl_nmrL_phase_v  = 0;
  bl_nmrL_if_attn  = 0;
  bl_nn_ave        = 1;
  bl_cc_ave        = 0;
  bl_is_ave        = 0;

}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::PrintBaselineVector(){
  cout<<"printing "<<endl;
  for(Int_t i=0;i<bl_sig.size();i++){
    //for(Int_t j=0;j<bl_sig.at(i).size();j++){
    for(Int_t j=0;j<bl_sig[i].size();j++){//this syntax is probably more intuitive 
      cout<<std::fixed
	  <<std::setprecision(0)<<std::setw(4)<<i<<" "
	  <<std::setprecision(0)<<std::setw(4)<<j<<" "
	//<<std::setprecision(7)<<std::setw(13)<<bl_sig.at(i).at(j)<<" "
	  <<std::setprecision(7)<<std::setw(13)<<bl_sig[i].at(j)<<" "
	  <<endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301base::SetFittingMinimizer(){
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit","Minimize");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);
  ROOT::Math::MinimizerOptions::SetDefaultTolerance(.0001);
  ROOT::Math::MinimizerOptions::SetDefaultPrecision(1.e-08);
  ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(100000);
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000000);
}

////////////////////////////////////////////////////////////////////////////////////////
