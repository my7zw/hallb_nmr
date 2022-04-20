#include "T8301view.hh"

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

T8301view::T8301view(TString _in_fn):T9301read(_in_fn){
  InitT8301Variables();
  
}

////////////////////////////////////////////////////////////////////////////////////////

T8301view::T8301view(){ InitT8301Variables(); }
T8301view::~T8301view(){}

//CleanT8301();

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::GetInitialInfo(){

  GetT9301EntryWithIndex(1);
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

Bool_t T8301view::IsEventOK(){
  Bool_t res = 1;
  Bool_t res_main = 1;
  Bool_t res_nmrL = 1;
  
  res_main = ( (bl_data_np       ==  data_np) &&
	       (bl_daq_card_n    ==  daq_card_n) &&
	       (bl_daq_gain      ==  daq_gain) );    

  if(is_nmrL){
    res_nmrL = ( (bl_nmrL_tune_v   == nmrL_tune_v)  &&
		 (bl_nmrL_phase_v  == nmrL_phase_v) &&
		 (bl_nmrL_if_attn  == nmrL_if_attn) );
  }
  res = res_main && res_nmrL;
  if(!res){
    cout<<"something is wrong with baseline file"<<endl;
  }

  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::PrepareBaselineVector(){
  bl_sig.push_back(*p_data_sig); //dereferencing p_data_sig to put in bl_sig
  //cout<<"preparing "<<p_data_sig->size()<<" "<<bl_sig.size()<<endl;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::PrintBaselineVector(){
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

void T8301view::FillAveragedBaselineHis(){

  //bl_hSvsF = (TH1D*)data_hSvsF->Clone("bl_h");
  bl_hSvsF = new TH1Du("bl_h", "bl_h", data_np+1, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);//bad

  Double_t fscan[bl_sig.size()] = {0.};
  for(Int_t j=0;j<bl_sig[0].size();j++){
    for(Int_t i=0;i<bl_sig.size();i++){
      fscan[i] = bl_sig[i].at(j);
    }
    Double_t sig_m = TMath::Mean(bl_sig.size(),fscan);
    Double_t sig_s = TMath::RMS(bl_sig.size(),fscan);
    cout<<std::fixed
	<<std::setprecision(0)<<std::setw(4)<<j<<" "
	<<std::setprecision(7)<<std::setw(13)<<sig_m<<" "
      	<<std::setprecision(7)<<std::setw(13)<<sig_s<<" "
	<<endl;
    
    bl_hSvsF->SetBinContent(j+1, sig_m);
    bl_hSvsF->SetBinError(j+1, sig_s);
    cout<<bl_sig[0].size()<<" "<<j<<" "<<bl_hSvsF->GetBinContent(j+1)<<" "<<bl_hSvsF->GetBinError(j+1)<<endl;
  }

  bl_hSvsF_subtr = (TH1D*)bl_hSvsF->Clone("bl_h_subtr");
  bl_hSvsF_resid = (TH1D*)bl_hSvsF->Clone("bl_h_resid");

  bl_hSvsF->Sumw2();
  bl_hSvsF_subtr->Sumw2();
  bl_hSvsF_resid->Sumw2();

}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::SetFittingMinimizer(){
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit","Minimize");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);
  ROOT::Math::MinimizerOptions::SetDefaultTolerance(.0001);
  ROOT::Math::MinimizerOptions::SetDefaultPrecision(1.e-08);
  ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(100000);
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000000);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::FitP2(){

  ffn_rpol2 = new TF1("rpol2","pol2",rf_s-rf_fstep/2., rf_e+rf_fstep/2.);//bad
  ffn_rpol2->SetLineColor(4);
  bl_hSvsF->Fit(ffn_rpol2,"NR");

  //TF1 *ffn_p2 = new TF1("p2", this, &T90DBmake::fit_p2,
  //rf_s-rf_fstep/2., rf_e+rf_fstep/2., 3);//bad
  ffn_p2 = new TF1("p2",fit_p2,rf_s-rf_fstep/2., rf_e+rf_fstep/2., 3);//bad
  ffn_p2->SetLineColor(8);
  ffn_p2->SetParameters(ffn_rpol2->GetParameter(0),
			ffn_rpol2->GetParameter(1),
			ffn_rpol2->GetParameter(2) );
  bl_hSvsF->Fit(ffn_p2,"R+");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::FitConvP3(){
  ffn_conv_p3 = new TF1("conv_p3",fit_conv_p3,
			rf_s-rf_fstep/2., rf_e+rf_fstep/2., 2);//bad
  //rf_f0*0.9986, rf_f0*1.0014, 5);//bad
  ffn_conv_p3->SetLineColor(7);
  bl_hSvsF_subtr->Fit(ffn_conv_p3,"MR+");  
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::FitConvP2P3(){
  ffn_conv_p2p3 = new TF1("conv_p2p3",fit_conv_p2p3,
			  rf_s-rf_fstep/2., rf_e+rf_fstep/2., 5);//bad
  //rf_f0*0.9986, rf_f0*1.0014, 5);//bad

  ffn_conv_p2p3->SetLineColor(1);
  ffn_conv_p2p3->SetParameters(ffn_p2->GetParameter(0),
			       ffn_p2->GetParameter(1),
			       ffn_p2->GetParameter(1),
			       ffn_conv_p3->GetParameter(0),
			       ffn_conv_p3->GetParameter(1)
			       );
  bl_hSvsF->Fit(ffn_conv_p2p3,"MR+");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::GetP2FitSubtracted(){
  bl_hSvsF_subtr->Add(ffn_p2,-1.);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::GetResidual(){
  bl_hSvsF_resid->Add(ffn_conv_p2p3,-1.);
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301view::fit_p2(Double_t *x , Double_t *par){
  //Double_t result = par[0]+par[1]*x[0]+par[2]*pow(x[0],2.)+par[3]*pow(x[0],3.);
  Double_t result = par[0]+par[1]*x[0]+par[2]*pow(x[0],2.);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301view::fit_conv_p3(Double_t *x , Double_t *par){
  // (x-f0)^3-(x-f0)
  Double_t y = x[0]-213.00;
  Double_t result = par[0]*pow(y,3)-y*par[1];
  //cout<<x[0]-213<<"  "<<par[4]<<endl;                                                  
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301view::fit_conv_p2p3(Double_t *x , Double_t *par){
  // start polynomial and x^3-x
  Double_t y = x[0]-213.00;
  Double_t result = (par[0]+par[1]*x[0]+par[2]*pow(x[0],2.))+(par[3]*pow(y,3)-y*par[4]);
  //cout<<x[0]-213<<"  "<<par[4]<<endl;
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301view::WriteBaselineResults(){
  blout_f = new TFile("mdata/"+base_fn+"_res.root","RECREATE","T8301 with baseline data");

  bl_hSvsF->Write();
  bl_hSvsF_subtr->Write();
  bl_hSvsF_resid->Write();
  
  ffn_rpol2->Write();
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

void T8301view::InitT8301Variables(){
  bl_sig.clear();

  bl_data_np       = 0;
  bl_daq_card_n    = 0;
  bl_daq_gain      = 0;
  bl_nmrL_tune_v   = 0;
  bl_nmrL_phase_v  = 0;
  bl_nmrL_if_attn  = 0;

}

////////////////////////////////////////////////////////////////////////////////////////
