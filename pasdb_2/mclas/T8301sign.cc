#include "T8301sign.hh"

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

T8301sign::T8301sign(TString _in_fn):T9301read(_in_fn){
  InitT8301Variables();
  pref = "";
  gStyle->SetTimeOffset(0);
}

////////////////////////////////////////////////////////////////////////////////////////

T8301sign::T8301sign(){ InitT8301Variables(); }
T8301sign::~T8301sign(){}

//CleanT8301();

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetBaselineInfo(TString _bl_fn){
  if(_bl_fn.EqualTo("")){bl_provided=0;}
  else{
    bl_provided=1;
    _bl_fn.Remove(0, 1+_bl_fn.Last('/'));
    _bl_fn.Remove(_bl_fn.Index(".root"));
    cout<<"mdata/"+_bl_fn+"_res.root"<<endl;
    TFile *bl_f = new TFile("mdata/"+_bl_fn+"_res.root","READ");
    if(bl_f->IsOpen()){
      bl_hSvsF         = (TH1D*)bl_f->Get("bl_h");
      cout<<"baseline his "<<bl_hSvsF->GetMean()<<endl;
      bl_ffn_conv_p2p3 = (TF1*)bl_f->Get("conv_p2p3");
      //bl_f->Close();
    }
    else{ cout<<"can't open file: mdata/"<<_bl_fn<<"_res.root, exiting..."<<endl; exit(222); }
    
    TString in_fn= "mdata/"+_bl_fn+"_res.txt";
    cout<<in_fn<<endl;
    std::ifstream fin;  fin.open(in_fn.Data());
    if(fin.good()){
      TString line;
      
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_fn           = line;
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_data_np      = line.Atoi();
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_daq_card_n   = line.Atoi();
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_daq_gain     = line.Atoi();
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_nmrL_tune_v  = line.Atof();
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_nmrL_phase_v = line.Atof();
      line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  bl_nmrL_if_attn = line.Atof();
      
      cout<<bl_fn<<endl;
      cout<<bl_data_np<<endl;
      cout<<bl_daq_card_n<<endl;
      cout<<bl_daq_gain<<endl;
      cout<<bl_nmrL_tune_v<<endl;
      cout<<bl_nmrL_phase_v<<endl;
      cout<<bl_nmrL_if_attn<<endl;
      
      fin.close();  
    }
    else{ cout<<"can't open file: "<<_bl_fn<<", exiting..."<<endl; exit(222); }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetConfigInfo(TString _cf_fn){
  //TString in_fn= "mdata/"+_cf_fn+"_res.txt";
  TString in_fn= _cf_fn;
  cout<<in_fn<<endl;
  std::ifstream fin;  fin.open(in_fn.Data());
  if(fin.good()){
    TString line;
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  fitx1 = line.Atof();
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  fitx2 = line.Atof();
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  fitx2_1 = line.Atof();
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  fitx3_1 = line.Atof();
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  fitx3 = line.Atof();
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  fitx4 = line.Atof();
    line.ReadLine(fin);  line.Remove(0,line.Index(":")+2);  f0_guess = line.Atof();

    cout<<fitx1<<endl;
    cout<<fitx2<<endl;
    cout<<fitx3<<endl;
    cout<<fitx4<<endl;
    cout<<f0_guess<<endl;
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetInitialInfo(){
  GetT9301EntryWithIndex(1);
  sg_data_np       = data_np;
  sg_daq_card_n    = daq_card_n;
  sg_daq_gain      = daq_gain;

  cout<<"center freq: "<<rf_f0<<endl;
  cout<<"number of data points: "<<data_np<<endl;
  cout<<"rf steps: "<<rf_nstep<<endl;
  cout<<"card number: "<<sg_daq_card_n<<endl;
  cout<<"card gain: "<<sg_daq_gain<<endl;

  if(is_nmrL){
    sg_nmrL_tune_v   = nmrL_tune_v;
    sg_nmrL_phase_v  = nmrL_phase_v;
    sg_nmrL_if_attn  = nmrL_if_attn;

    cout<<"tune voltage: "<<sg_nmrL_tune_v<<endl;
    cout<<"phase voltage: "<<sg_nmrL_phase_v<<endl;
    cout<<"attenuation: "<<sg_nmrL_if_attn<<endl;
 
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::InitOutFile(TString _dn){
  TString new_dn = base_dn;
  new_dn.Remove(1+new_dn.Last('_'));
  sgout_f = new TFile(new_dn+_dn+base_fn+"_area.root","RECREATE","T8301 with area data");
  //sgout_f = new TFile(new_dn+"res1/"+base_fn+"_area.root","RECREATE","T8301 with area data");
  //TString outfn = new_dn+_dn+base_fn+"_area.txt";
  TString outfn = new_dn+_dn+base_fn+"_evlist.txt";
  //fout2.open(outfn.Data());

}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::InitHistos(){
  sg_gPvsT = new TGraphErrorsu();
  sg_gPvsT->SetName(base_fn);             sg_gPvsT->SetTitle(base_fn+"_area");

  sg_gPvsT_T = new TGraphErrorsu();
  sg_gPvsT_T->SetName(base_fn+"_area_T"); sg_gPvsT_T->SetTitle(base_fn+"_area_T");
  sg_he4PvsT = new TGraphErrorsu();
  sg_he4PvsT->SetName(base_fn+"_4HeP_T"); sg_he4PvsT->SetTitle(base_fn+"_4HeP_T");

  sg_he4T_t = new TGraphErrorsu();
  sg_he4T_t->SetName(base_fn+"_4HeT_t"); sg_he4T_t->SetTitle(base_fn+"_4HeT_t");
  sg_he4P_t = new TGraphErrorsu();
  sg_he4P_t->SetName(base_fn+"_4HeP_t"); sg_he4P_t->SetTitle(base_fn+"_4HeP_t");

  sg_g_sgl1_ampl = new TGraphErrorsu();
  sg_g_sgl1_ampl->SetName(base_fn+"_sa1");  sg_g_sgl1_ampl->SetTitle(base_fn+"_peak1_ampl");
  sg_g_sgl1_sigm = new TGraphErrorsu();
  sg_g_sgl1_sigm->SetName(base_fn+"_ss1");  sg_g_sgl1_sigm->SetTitle(base_fn+"_peak1_sigm");

  sg_g_sgl2_ampl = new TGraphErrorsu();
  sg_g_sgl2_ampl->SetName(base_fn+"_sa2");  sg_g_sgl2_ampl->SetTitle(base_fn+"_peak2_ampl");
  sg_g_sgl2_sigm = new TGraphErrorsu();
  sg_g_sgl2_sigm->SetName(base_fn+"_ss2");  sg_g_sgl2_sigm->SetTitle(base_fn+"_peak2_sigm");

  sg_g_sgl_arat = new TGraphErrorsu();
  sg_g_sgl_arat->SetName(base_fn+"_sar");  sg_g_sgl_arat->SetTitle(base_fn+"_peak_arat");
  sg_g_sgl_srat = new TGraphErrorsu();
  sg_g_sgl_srat->SetName(base_fn+"_ssr");  sg_g_sgl_srat->SetTitle(base_fn+"_peak_srat");

  //sg_hSvsF = new TH1Du("sg_v", "sg_v", data_np+1, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);
  //sg_hSvsF = new TH1Du("sg_v", "sg_v", data_np, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);
  //sg_hSvsF->Sumw2();
  
  sg_g_noise_mean = new TGraphErrorsu();
  sg_g_noise_mean->SetName(base_fn+"noise_m");  sg_g_noise_mean->SetTitle(base_fn+"_noise_m");
  sg_g_noise_sigm = new TGraphErrorsu();
  sg_g_noise_sigm->SetName(base_fn+"noise_s");  sg_g_noise_sigm->SetTitle(base_fn+"_noise_s");
  sg_hSvsF_noise_all = new TH1D(base_fn+"all_noise",base_fn+"all_noise", 100, -0.002, 0.002);

  
  sg_g_qcv_dcoff = new TGraphErrorsu();//offset of qcurve minimum
  sg_g_qcv_dcoff->SetName(base_fn+"_dcoff");  sg_g_qcv_dcoff->SetTitle(base_fn+"_dcoff");
  sg_g_qcv_dcoff_T = new TGraphErrorsu();//offset of qcurve minimum
sg_g_qcv_dcoff_T->SetName(base_fn+"_dcoff_T");  sg_g_qcv_dcoff_T->SetTitle(base_fn+"_dcoff_T");
  sg_g_qcv_moff = new TGraphErrorsu();//offset of qcurve minimum
  sg_g_qcv_moff->SetName(base_fn+"_moff");  sg_g_qcv_moff->SetTitle(base_fn+"_moff");
  sg_g_qcv_ampl = new TGraphErrorsu();//qcurve max and min difference
  sg_g_qcv_ampl->SetName(base_fn+"_qamp");  sg_g_qcv_ampl->SetTitle(base_fn+"_qamp");
  sg_g_qcv_ampl_T = new TGraphErrorsu();//qcurve max and min difference
  sg_g_qcv_ampl_T->SetName(base_fn+"_qamp_T");  sg_g_qcv_ampl_T->SetTitle(base_fn+"_qamp_T");
  sg_g_qcv_skew = new TGraphErrorsu();//qcurve left and right elevation difference
  sg_g_qcv_skew->SetName(base_fn+"_skew");  sg_g_qcv_skew->SetTitle(base_fn+"_skew");
  sg_g_qcv_skew_T = new TGraphErrorsu();//qcurve left and right elevation difference
  sg_g_qcv_skew_T->SetName(base_fn+"_skew_T");  sg_g_qcv_skew_T->SetTitle(base_fn+"_skew_T");
  sg_g_qcv_msht = new TGraphErrorsu();//qcurve center shift from set center
  sg_g_qcv_msht->SetName(base_fn+"_msht");  sg_g_qcv_msht->SetTitle(base_fn+"_msht");
  sg_g_qcv_msht1 = new TGraphErrorsu();//qcurve center shift from set center
  sg_g_qcv_msht1->SetName(base_fn+"_msht1");  sg_g_qcv_msht1->SetTitle(base_fn+"_msht1");
  sg_g_qcv_msht1_T = new TGraphErrorsu();//qcurve center shift from set center
sg_g_qcv_msht1_T->SetName(base_fn+"_msht1_T");  sg_g_qcv_msht1_T->SetTitle(base_fn+"_msht1_T");
  sg_g_qcv_cubc = new TGraphErrorsu();//qcurve cubic term max and min difference
  sg_g_qcv_cubc->SetName(base_fn+"_cubc");  sg_g_qcv_cubc->SetTitle(base_fn+"_cubc");

}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T8301sign::DoesEventMatchBaseline(){
  Bool_t res = 1;  Bool_t res_main = 1;  Bool_t res_nmrL = 1;
  
  res_main = ( (bl_data_np       ==  sg_data_np) &&
	       (bl_daq_card_n    ==  sg_daq_card_n) &&
	       (bl_daq_gain      ==  sg_daq_gain) );    

  if(is_nmrL){
    res_nmrL = ( (bl_nmrL_tune_v   == sg_nmrL_tune_v)  &&
		 (bl_nmrL_phase_v  == sg_nmrL_phase_v) &&
		 (bl_nmrL_if_attn  == sg_nmrL_if_attn) );
  }
  res = res_main && res_nmrL;
  if(!res){cout<<"mismatch between TE and baseline file"<<endl;}

  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T8301sign::IsEventOK(){
  Bool_t res = 1;  Bool_t res_main = 1;  Bool_t res_nmrL = 1;
  
  res_main = ( (sg_data_np       ==  data_np) &&
	       (sg_daq_card_n    ==  daq_card_n) &&
	       (sg_daq_gain      ==  daq_gain) );    

  if(is_nmrL){
    res_nmrL = ( (sg_nmrL_tune_v   == nmrL_tune_v)  &&
		 (sg_nmrL_phase_v  == nmrL_phase_v) &&
		 (sg_nmrL_if_attn  == nmrL_if_attn) );
  }
  res = res_main && res_nmrL;
  if(!res){cout<<"something is wrong with TE file"<<endl;}

  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FillVectorAll(){
  sg_sig.push_back(*p_data_sig);
  sg_t.push_back(ev_time);
  sg_P.push_back(ev_4He_p);
  sg_T.push_back(ev_4He_t);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::SetSampleSize(Int_t _ss){ sg_nn_ave = _ss; }

////////////////////////////////////////////////////////////////////////////////////////

Bool_t T8301sign::FillVectorAve(){
  if(sg_cc_ave==0        ){
    sg_sig.clear();
    sg_t.clear();
    sg_P.clear();
    sg_T.clear();
    sg_is_ave = 0;
  }
  if(sg_cc_ave <sg_nn_ave){
    sg_sig.push_back(*p_data_sig);
    sg_t.push_back(ev_time);
    sg_P.push_back(ev_4He_p);
    sg_T.push_back(ev_4He_t);
    sg_cc_ave++;
  }
  if(sg_cc_ave==sg_nn_ave){ sg_is_ave = 1;                 sg_cc_ave = 0; }
  return sg_is_ave;
}

Bool_t T8301sign::FillVectorAveI(Int_t _i){
  if(sg_cc_ave==0        ){ sg_sig.clear();                sg_is_ave = 0; }
  if(sg_cc_ave <sg_nn_ave){ sg_sig.push_back(*p_data_sig); sg_cc_ave++;   }
  if(sg_cc_ave==sg_nn_ave){ sg_is_ave = 1;                 sg_cc_ave = 0; }

  /*fout2<<std::fixed<<std::setprecision(0)
       <<std::setw(4)<<_i<<" "
       <<std::setw(4)<<sg_nn_ave<<" "
       <<std::setw(4)<<sg_ave_ev_cc<<" "
       <<std::setw(4)<<sg_cc_ave<<" "
       <<std::setw(20)<<ev_time<<endl;*/
    
  return sg_is_ave;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetPrefix(){pref = Form("_%d_%d", sg_nn_ave, sg_ave_ev_cc);}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FillHistAve(){
  TString hn = base_fn + pref;
  sg_hSvsF = new TH1Du(hn, hn, data_np, rf_s-rf_fstep/2., rf_e+rf_fstep/2.);
  sg_hSvsF->Sumw2();
  //sg_hSvsF->Reset();
  Double_t fscan[sg_sig.size()] = {0.};
  for(Int_t j=0;j<sg_sig[0].size();j++){
    Double_t sign_coef  = 1.;
    if(is_nmrU){sign_coef = -1.;}
    for(Int_t i=0;i<sg_sig.size();i++){ fscan[i] = sign_coef*sg_sig[i].at(j); }
    
    Double_t sig_m = TMath::Mean(sg_sig.size(),fscan);
    Double_t sig_s = TMath::RMS(sg_sig.size(),fscan) / sqrt(sg_sig.size());
    if(sg_sig.size()<=1){ sig_s = 0.002*sig_m; //cout<<"------------"<<endl;
    }
    
    sg_hSvsF->SetBinContent(j+1, sig_m);
    sg_hSvsF->SetBinError(j+1, sig_s);
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GettPTAve(){
  sg_t_av    = TMath::Mean(sg_t.size(),sg_t.data());
  sg_t_av_er = TMath::RMS(sg_t.size(),sg_t.data()) / sqrt(sg_t.size());
  sg_P_av    = TMath::Mean(sg_P.size(),sg_P.data());
  sg_P_av_er = TMath::RMS(sg_P.size(),sg_P.data()) / sqrt(sg_P.size());
  sg_T_av    = TMath::Mean(sg_T.size(),sg_T.data());
  sg_T_av_er = TMath::RMS(sg_T.size(),sg_T.data()) / sqrt(sg_T.size());
  if(sg_t.size()<=1){sg_t_av_er = 0.0;}
  if(sg_P.size()<=1){sg_P_av_er = 0.0;}
  if(sg_T.size()<=1){sg_T_av_er = 0.0;}
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetBaselineSubtractedAve(){
  sg_hSvsF_subtr = (TH1D*)sg_hSvsF->Clone("sgh_subtr"+pref);
  if(bl_provided){ sg_hSvsF_subtr->Add(bl_hSvsF,-1.); }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetBackgroundHistAve(){
  sg_hSvsF_bkgrd = (TH1D*)sg_hSvsF_subtr->Clone("sgh_bkgrd"+pref);
  sg_hSvsF_bkgrd->SetLineColor(2);
  sg_hSvsF_bkgrd->SetMarkerColor(2);sg_hSvsF_bkgrd->SetMarkerStyle(3);
  Int_t hr_nbins = sg_hSvsF_bkgrd->GetNbinsX();
  //for(Int_t i=1;i<hr_nbins;i++){
  for(Int_t i=0;i<=hr_nbins;i++){
    Double_t x_i = sg_hSvsF_bkgrd->GetBinCenter(i);

    //if( (x_i<fitx1) ||  (x_i>fitx2 && x_i<fitx3) || (x_i>fitx4) ){
    if((x_i<fitx1)||(x_i>fitx2 && x_i<fitx2_1)||(x_i>fitx3_1 && x_i<fitx3)||(x_i>fitx4)){

      sg_hSvsF_bkgrd->SetBinContent(i, 0.);
      sg_hSvsF_bkgrd->SetBinError(i, 0.);
      //cout<<"bin "<<i<<" "<<x_i<<" "<<sg_hSvsF_bkgrd->GetBinContent(i)<<endl;
    }
  }
  sg_hSvsF_bkgrd_subtr = (TH1D*)sg_hSvsF_bkgrd->Clone("sgh_bkgrd_subtr"+pref);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FitP2(){
  ffn_p2 = new TF1("p2"+pref, fit_p2, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 4);
  ffn_p2->SetLineColor(8);  ffn_p2->SetMarkerColor(8);
  ffn_p2->SetParameter(3, f0_guess);
  if(is_nmrL && nmrL_log_ch){
    ffn_p2->SetParameter(0, sg_hSvsF_bkgrd->GetMinimum());
  }
  sg_hSvsF_bkgrd->Fit(ffn_p2,"NQMR+");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FitConvP3(){
  ffn_p3 = new TF1("p3"+pref, fit_p3, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 4);
  ffn_p3->SetLineColor(7);   ffn_p3->SetMarkerColor(7);
  ffn_p3->SetParameter(3, f0_guess);
  sg_hSvsF_bkgrd_subtr->Fit(ffn_p3,"NQMR+");  
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FitConvP4(){
  ffn_p3 = new TF1("p3"+pref, fit_p3, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 4);
  ffn_p3->SetLineColor(7);   ffn_p3->SetMarkerColor(7);
  ffn_p3->SetParameter(3, f0_guess);
  sg_hSvsF_bkgrd_subtr->Fit(ffn_p3,"NQMR+");  
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FitConvP2P3(){
  ffn_p2p3 = new TF1("p2p3"+pref, fit_p2p3, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 5);
  ffn_p2p3->SetLineColor(2);  ffn_p2p3->SetMarkerColor(2);
  ffn_p2p3->SetParameters(ffn_p2->GetParameter(0)+ffn_p3->GetParameter(0),
			  ffn_p2->GetParameter(1)+ffn_p3->GetParameter(1),//lin
			  ffn_p2->GetParameter(2),//qaud
			  ffn_p3->GetParameter(2),//cub
			  f0_guess);//shift

  sg_hSvsF_bkgrd->Fit(ffn_p2p3,"NQMR+same");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FitP1(){
  ffn_p1 = new TF1("p1"+pref, fit_p1, rf_s-rf_fstep/2., rf_e+rf_fstep/2., 3);
  ffn_p1->SetLineColor(8);  ffn_p1->SetMarkerColor(8);
  ffn_p1->FixParameter(2, rf_f0);
  sg_hSvsF->Fit(ffn_p1,"MR+");
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::FitGaus(){
  //TF1 *temp_gaus = new TF1("ft1","gaus",224.74,224.76);//warm single peak
  //TF1 *temp_gaus = new TF1("ft1","gaus",224.24,224.27);//cold 60dB
  TF1 *temp_gaus = new TF1("ft1","gaus",212.9,213.1);//real polarization
  sg_hSvsF_signal->Fit(temp_gaus,"MQNR");
  Double_t g_pars[3]; temp_gaus->GetParameters(g_pars);
  //ffn_g1 = new TF1("g1"+pref,"gaus",g_pars[1]-0.5*g_pars[2],g_pars[1]+0.5*g_pars[2]);
  ffn_g1 = new TF1("g1"+pref,"gaus",g_pars[1]-g_pars[2],g_pars[1]+g_pars[2]);
  sg_hSvsF_signal->Fit(ffn_g1,"MQNR");
  sg_sgl1_ampl = ffn_g1->GetParameter(0);
  sg_sgl1_sigm = ffn_g1->GetParameter(2);
  //cout<<"!!!!!!!!!!! mean "<<ffn_g1->GetParameter(1)<<endl;

  delete temp_gaus; temp_gaus = NULL;

  //temp_gaus = new TF1("ft1","gaus",225.71,225.76);//warm single peak
  //temp_gaus = new TF1("ft1","gaus",224.7,224.8);
  temp_gaus = new TF1("ft1","gaus",212.9,213.1);
  sg_hSvsF_signal->Fit(temp_gaus,"MQNR");
  temp_gaus->GetParameters(g_pars);
  ffn_g2 = new TF1("g2"+pref,"gaus",g_pars[1]-g_pars[2],g_pars[1]+g_pars[2]);
  sg_hSvsF_signal->Fit(ffn_g2,"MQNR");
  sg_sgl2_ampl = ffn_g2->GetParameter(0);
  sg_sgl2_sigm = ffn_g2->GetParameter(2);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetP2FitSubtracted(){
  //sg_hSvsF_bkgrd_subtr->Add(ffn_p2,-1.);
  Int_t hr_nbins = sg_hSvsF_bkgrd_subtr->GetNbinsX();
  for(Int_t i=1;i<hr_nbins;i++){
    Double_t y_his = sg_hSvsF_bkgrd_subtr->GetBinContent(i);
    if(y_his!=0){
      Double_t x_i = sg_hSvsF_bkgrd_subtr->GetBinCenter(i);
      Double_t y_ffn = ffn_p2->Eval(x_i);
      Double_t y_new= y_his - y_ffn;  
      //Double_t y_newer = sg_hSvsF_bkgrd_subtr->GetBinError(i) - ?;
      sg_hSvsF_bkgrd_subtr->SetBinContent(i, y_new);
      //sg_hSvsF_bkgrd_subtr->SetBinError(i, ?);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetSignalAve(){
  sg_hSvsF_signal = (TH1D*)sg_hSvsF_subtr->Clone("sgh_signal"+pref);
  sg_hSvsF_signal->Add(ffn_p2p3,-1.);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetNoiseHis(){
  sg_hSvsF_noise = new TH1D("sgh_noise"+pref, "sgh_noise"+pref, 100, -0.002, 0.002);
  Int_t hr_nbins = sg_hSvsF_signal->GetNbinsX();
  for(Int_t i=0;i<=hr_nbins;i++){
    Double_t x_i = sg_hSvsF_signal->GetBinCenter(i);
    //if( (x_i<fitx1) ||  (x_i>fitx2 && x_i<fitx3) || (x_i>fitx4) ){
    if( (x_i<fitx2) || (x_i>fitx3) ){
      sg_hSvsF_noise_all->Fill(sg_hSvsF_signal->GetBinContent(i)/fabs(sg_area_now));
      sg_hSvsF_noise->Fill(sg_hSvsF_signal->GetBinContent(i)/fabs(sg_area_now));

      //sg_hSvsF_noise_all->Fill(sg_hSvsF_signal->GetBinContent(i));
      //sg_hSvsF_noise->Fill(sg_hSvsF_signal->GetBinContent(i));

      //cout<<"bin "<<i<<" "<<x_i<<" "<<sg_hSvsF_bkgrd->GetBinContent(i)<<endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetTEAreaAve(){
  sg_area = CalculateArea(sg_hSvsF_signal);
  Double_t sum=0;
  for(Int_t i=1;i<sg_hSvsF_signal->GetNbinsX();i++){
    sum = sum + sg_hSvsF_signal->GetBinContent(i);
  }
  sg_gPvsT->SetPoint(sg_ave_ev_cc, ev_time, sg_area);
  sg_gPvsT->SetPointError(sg_ave_ev_cc, 0., sg_area_err);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetTEAreaAve_tPTAve(){
  sg_area = CalculateArea(sg_hSvsF_signal);
  Double_t sum=0;
  for(Int_t i=1;i<sg_hSvsF_signal->GetNbinsX();i++){
    sum = sum + sg_hSvsF_signal->GetBinContent(i);
  }
  sg_gPvsT->SetPoint(sg_ave_ev_cc, sg_t_av, sg_area);
  sg_gPvsT->SetPointError(sg_ave_ev_cc, 0, sg_area_err);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetAuxAve(){
  sg_gPvsT_T->SetPoint(sg_ave_ev_cc, ev_4He_t, sg_area);
  sg_gPvsT_T->SetPointError(sg_ave_ev_cc, 0., sg_area_err);

  sg_he4PvsT->SetPoint(sg_ave_ev_cc, ev_4He_p, ev_4He_t);

  sg_he4T_t->SetPoint(sg_ave_ev_cc, ev_time, ev_4He_t);

  sg_he4P_t->SetPoint(sg_ave_ev_cc, ev_time, ev_4He_p);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetAuxAve_tPTAve(){
  sg_gPvsT_T->SetPoint(sg_ave_ev_cc, sg_T_av, sg_area);
  sg_gPvsT_T->SetPointError(sg_ave_ev_cc, sg_T_av_er, sg_area_err);

  sg_he4PvsT->SetPoint(sg_ave_ev_cc, sg_P_av, sg_T_av);
  sg_he4PvsT->SetPointError(sg_ave_ev_cc, sg_P_av_er, sg_T_av_er);

  sg_he4T_t->SetPoint(sg_ave_ev_cc, sg_t_av, sg_T_av);
  sg_he4T_t->SetPointError(sg_ave_ev_cc, sg_t_av_er, sg_T_av_er);
  
  sg_he4P_t->SetPoint(sg_ave_ev_cc, sg_t_av, sg_P_av);
  sg_he4P_t->SetPointError(sg_ave_ev_cc, sg_t_av_er, sg_P_av_er);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetSignalValues(){
  sg_g_sgl1_ampl->SetPoint(sg_ave_ev_cc, ev_time, sg_sgl1_ampl);
  sg_g_sgl1_sigm->SetPoint(sg_ave_ev_cc, ev_time, sg_sgl1_sigm);
  sg_g_sgl2_ampl->SetPoint(sg_ave_ev_cc, ev_time, sg_sgl2_ampl);
  sg_g_sgl2_sigm->SetPoint(sg_ave_ev_cc, ev_time, sg_sgl2_sigm);
  Double_t arat = 0;
  if(sg_sgl2_ampl!=0){arat=sg_sgl1_ampl/sg_sgl2_ampl;}
  sg_g_sgl_arat->SetPoint(sg_ave_ev_cc, ev_time, arat);
  sg_g_sgl_srat->SetPoint(sg_ave_ev_cc, ev_time, sg_sgl1_sigm/sg_sgl2_sigm);
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetNoiseValues(Int_t _ev){
  Double_t noise_mean = sg_hSvsF_noise->GetMean();
  Double_t noise_sigm = sg_hSvsF_noise->GetStdDev();
  Double_t cut1 = noise_mean - 1.5*noise_sigm;
  Double_t cut2 = noise_mean + 1.5*noise_sigm;
  //if(noise_mean>cut1 && noise_mean<cut2){fout2<<_ev<<endl;}
  //sg_g_noise_mean->SetPoint(sg_ave_ev_cc, ev_time, 100.*noise_mean/fabs(sg_area_now));
  //sg_g_noise_sigm->SetPoint(sg_ave_ev_cc, ev_time, 100.*noise_sigm/fabs(sg_area_now));

  sg_g_noise_mean->SetPoint(sg_ave_ev_cc, ev_time, 100.*noise_mean);
  sg_g_noise_sigm->SetPoint(sg_ave_ev_cc, ev_time, 100.*noise_sigm);

  if(sg_area_now<0){
    cout<<"noise "<<noise_mean<<" "<<noise_sigm<<" "<<sg_area_now<<endl;}
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::GetQcurveValues(){
  sg_g_qcv_dcoff->SetPoint(sg_ave_ev_cc, ev_time, daq_dcoff_v);
  if(is_nmrL){sg_g_qcv_dcoff_T->SetPoint(sg_ave_ev_cc, nmrL_card_t, daq_dcoff_v);}

  Double_t max = 0.0;
  Double_t min = 0.0;

  if(bl_provided){
     max = sg_hSvsF->GetBinContent(sg_hSvsF->GetMaximumBin());
     min = sg_hSvsF->GetBinContent(sg_hSvsF->GetMinimumBin());}
  else{
     max = ffn_p2p3->GetMaximum();
     min = ffn_p2p3->GetMinimum();}
  
  sg_g_qcv_moff->SetPoint(sg_ave_ev_cc, ev_time, min);
  sg_g_qcv_ampl->SetPoint(sg_ave_ev_cc, ev_time, max-min);
  if(is_nmrL){sg_g_qcv_ampl_T->SetPoint(sg_ave_ev_cc, nmrL_card_t, max-min);}
  
  Int_t bin_sig_s = sg_hSvsF->FindBin(fitx2);
  Int_t bin_sig_e = sg_hSvsF->FindBin(fitx3);
  Double_t err=0;

  Double_t ar_l =  sg_hSvsF->IntegralAndError(1, bin_sig_s, err,"width");
  Double_t ar_r =  sg_hSvsF->IntegralAndError(bin_sig_e, 500, err,"width");
  Double_t skew = 200.*(ar_l-ar_r)/(ar_l+ar_r);
  sg_g_qcv_skew->SetPoint(sg_ave_ev_cc, ev_time, skew);
  if(is_nmrL){sg_g_qcv_skew_T->SetPoint(sg_ave_ev_cc, nmrL_card_t, skew);}


  Double_t min_shift = ffn_p2p3->GetParameter(4);
  sg_g_qcv_msht->SetPoint(sg_ave_ev_cc, ev_time, min_shift);

  //Double_t min_shift1 = sg_hSvsF->GetBinCenter(sg_hSvsF->GetMinimumBin());
  Double_t min_shift1 = ffn_p2p3->GetMinimumX();
  sg_g_qcv_msht1->SetPoint(sg_ave_ev_cc, ev_time, min_shift1);
  if(is_nmrL){sg_g_qcv_msht1_T->SetPoint(sg_ave_ev_cc, nmrL_card_t, min_shift1);}

  Double_t cubc = ffn_p3->GetParameter(2);
  sg_g_qcv_cubc->SetPoint(sg_ave_ev_cc, ev_time, cubc);
  
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::IncrementAveragedEvent(){sg_ave_ev_cc++;}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301sign::CalculateArea(TH1D *_h){
  Double_t res =0.,err=0.;
  Int_t bin_sig_s = _h->FindBin(fitx2);
  Int_t bin_sig_e = _h->FindBin(fitx3);
  //res =  _h->Integral(bin_sig_s, bin_sig_e);
  res =  _h->IntegralAndError(bin_sig_s, bin_sig_e, err,"width");
  sg_area_err = err; err = 0.;
  /*fout2<<std::fixed<<std::setprecision(0)
       <<std::setw(4)<<sg_nn_ave<<" "
       <<std::setw(4)<<sg_ave_ev_cc<<" "
       <<std::setw(20)<<ev_time<<" "
       <<std::setprecision(9)
       <<std::setw(20)<<res<<" "
       <<std::setw(20)<<sg_area_err<<endl;*/

  //res =  _h->IntegralAndError(bin_sig_s, bin_sig_e,"width");
  sg_area_now  =  _h->IntegralAndError(bin_sig_s, bin_sig_e, err,"");
  //cout<<"integral "<<fitx2<<" "<<fitx3<<" "<<bin_sig_s<<" "<<bin_sig_e<<" "<<res<<endl;
  return res;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301sign::fit_p2(Double_t *x , Double_t *par){
  Double_t result = par[0]+par[1]*(x[0]-par[3])+par[2]*pow(x[0]-par[3],2);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301sign::fit_p3(Double_t *x , Double_t *par){
  Double_t result = par[0]                    +
                    par[1]*   (x[0]-par[3])   +
                    par[2]*pow(x[0]-par[3],3);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301sign::fit_p2p3(Double_t *x , Double_t *par){
  Double_t result = par[0]                    +
                    par[1]*   (x[0]-par[4])   +
                    par[2]*pow(x[0]-par[4],2) +
                    par[3]*pow(x[0]-par[4],3);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301sign::fit_p4(Double_t *x , Double_t *par){
  Double_t result = par[0]                    +
                    par[1]*   (x[0]-par[5])   +
                    par[2]*pow(x[0]-par[5],2) +
                    par[3]*pow(x[0]-par[5],3) +
                    par[4]*pow(x[0]-par[5],4);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

Double_t T8301sign::fit_p1(Double_t *x , Double_t *par){
  //Double_t result = par[0]+par[1]*x[0];
  Double_t result = par[0]+par[1]*(x[0]-par[2]);
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::WriteAveEvent(){
  sgout_f->cd();
  sg_hSvsF->Write();
  sg_hSvsF_subtr->Write();
  sg_hSvsF_bkgrd->Write();
  sg_hSvsF_bkgrd_subtr->Write();
  sg_hSvsF_signal->Write();
  sg_hSvsF_noise->Write();
  ffn_p2->Write();
  ffn_p3->Write();
  ffn_p2p3->Write();
  //gFile->ls("-m");

  sg_hSvsF->Delete();
  sg_hSvsF_subtr->Delete();
  sg_hSvsF_bkgrd->Delete();
  sg_hSvsF_bkgrd_subtr->Delete();
  sg_hSvsF_signal->Delete();
  sg_hSvsF_noise->Delete();
  ffn_p2->Delete();
  ffn_p3->Delete();
  ffn_p2p3->Delete();  

}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::WriteSignResults(){

  sg_gPvsT_T->Write();
  sg_he4PvsT->Write();
  sg_he4T_t->Write();
  sg_he4P_t->Write();
  sg_gPvsT->Write();
  
  sg_g_sgl1_ampl->Write();
  sg_g_sgl1_sigm->Write();
  sg_g_sgl2_ampl->Write();
  sg_g_sgl2_sigm->Write();
  sg_g_sgl_arat->Write();
  sg_g_sgl_srat->Write();
  sg_g_noise_mean->Write();
  sg_g_noise_sigm->Write();
  sg_hSvsF_noise_all->Write();

  sg_g_qcv_dcoff->Write();//offset of qcurve minimum
  sg_g_qcv_dcoff_T->Write();//offset of qcurve minimum
  sg_g_qcv_moff->Write();//offset of qcurve minimum
  sg_g_qcv_ampl->Write();//qcurve max and min difference
  sg_g_qcv_ampl_T->Write();//qcurve max and min difference
  sg_g_qcv_skew->Write();//qcurve left and right elevation difference
  sg_g_qcv_skew_T->Write();//qcurve left and right elevation difference
  sg_g_qcv_msht->Write();//qcurve center shift from set center
  sg_g_qcv_msht1->Write();//qcurve center shift from set center
  sg_g_qcv_msht1_T->Write();//qcurve center shift from set center
  sg_g_qcv_cubc->Write();//qcurve cubic term max and min difference
 
  sgout_f->Close();
  //fout2.close();
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::InitT8301Variables(){
  sg_sig.clear();
  sg_t.clear();
  sg_P.clear();
  sg_T.clear();

  bl_data_np       = 0;
  bl_daq_card_n    = 0;
  bl_daq_gain      = 0;
  bl_nmrL_tune_v   = 0;
  bl_nmrL_phase_v  = 0;
  bl_nmrL_if_attn  = 0;

  sg_data_np       = 0;
  sg_daq_card_n    = 0;
  sg_daq_gain      = 0;
  sg_nmrL_tune_v   = 0;
  sg_nmrL_phase_v  = 0;
  sg_nmrL_if_attn  = 0;

  sg_nn_ave        = 0;
  sg_cc_ave        = 0;
  sg_is_ave        = 0;
  sg_ave_ev_cc     = 0;
  sg_t_av          = 0;
  sg_t_av_er       = 0;
  sg_P_av          = 0;
  sg_P_av_er       = 0;
  sg_T_av          = 0;
  sg_T_av_er       = 0;
  sg_area          = 0;
  sg_area_err      = 0;
  sg_area_now      = 0;
  sg_sgl1_ampl     = 0;
  sg_sgl1_sigm     = 0;
  sg_sgl2_ampl     = 0;
  sg_sgl2_sigm     = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

void T8301sign::SetFittingMinimizer(){
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit","Minimize");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);
  ROOT::Math::MinimizerOptions::SetDefaultTolerance(.0001);
  ROOT::Math::MinimizerOptions::SetDefaultPrecision(1.e-08);
  ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(100000);
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000000);
}

////////////////////////////////////////////////////////////////////////////////////////
