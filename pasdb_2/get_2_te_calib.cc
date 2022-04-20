#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>

#include "TRint.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "THupgrade.hh"


#include "mclas/T8301sign.hh"
#include "mclas/T7301theq.hh"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{

  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  gStyle->SetOptFit(1111);
  gStyle->SetLineScalePS(1);

  cout<<"argc = "<<argc<<" "<<argv[1]<<endl;
  TString ep_fn=theApp->Argv(1);
  TString bl_fn=theApp->Argv(2);
  //TString ep_fn=argv[1];
  //TString bl_fn=argv[2];

  if(!bl_fn){bl_fn="";}
  
  T8301sign *ep = new T8301sign(ep_fn);
  ep->GetBaselineInfo(bl_fn);
  //TString cf_fn = "mdata/data_86_87/analysis_cnfg_te.txt";
  //TString cf_fn = "mdata/data_86_87/analysis_cnfg_en.txt";
  TString cf_fn = "mdata/data_86_87/analysis_cnfg_tail.txt";
    
  ep->GetConfigInfo(cf_fn.Data());

  ep->GetInitialInfo();
  //ep->InitOutFile("res_cnfg_calib_n1/");
  //ep->InitOutFile("res_cnfg_calib_n5/");
  //ep->InitOutFile("res_cnfg_calib_en/");
  ep->InitOutFile("res_cnfg_calib_tail/");
  
  ep->InitHistos();
  ep->DoesEventMatchBaseline();
  
  Int_t nev = ep->GetT9301Entries();
  cout<<nev<<endl;
  //ep->SetSampleSize(50);
  //ep->SetSampleSize(2);
  ep->SetSampleSize(1);
  //ep->SetSampleSize(5);
  //ep->SetSampleSize(10);
  
  TCanvas *c=new TCanvas("c","c",1500,1000);
  c->Divide(2,2);

  ep_fn.Remove(0, 1+ep_fn.Last('/'));
  //cout<<ep_fn<<endl;
  TString cnfg = "_1";
  //TString pdfn ="pdf/cold/60_fit_"+ep_fn+cnfg+".pdf[";
  TString pdfn ="pdf/cold/70_fit_"+ep_fn+cnfg+".pdf[";
  //cout<<pdfn<<endl;
  //c->Print(pdfn.Data());
  
  Int_t cc=0;

  for(Int_t i=2;i<nev;i++){
    //for(Int_t i=525;i<nev;i++){

    if(ep_fn.Contains("2019-08-30_14h36m04s")&&i<552){continue;}
    if(ep_fn.Contains("2019-08-30_16h40m53s")&&i>152){continue;}
    if(ep_fn.Contains("2019-08-30_17h32m08s")&&i<802){continue;}
    if(ep_fn.Contains("2019-08-30_18h45m58s")&&i>102){continue;}
    if(ep_fn.Contains("2019-08-30_22h33m09s")&&i<37) {continue;}
    if(ep_fn.Contains("QCV3650051306")&&i<52) {continue;}
    if(ep_fn.Contains("QCV3650068143")&&i>127) {continue;}

    ep->GetT9301Entry(i);
    ep->IsEventOK();

    if(ep->FillVectorAve()){
    //if(ep->FillVectorAveI(i)){
      ep->GetPrefix();
      ep->FillHistAve();
      ep->GettPTAve();
      ep->GetBaselineSubtractedAve();
      ep->GetBackgroundHistAve();
      ep->FitP2();
      ep->GetP2FitSubtracted();
      ep->FitConvP3();
      ep->FitConvP2P3();
      ep->GetSignalAve();
      ep->FitGaus();
      //ep->GetTEAreaAve();
      ep->GetTEAreaAve_tPTAve();
      //ep->GetAuxAve();
      ep->GetAuxAve_tPTAve();
      ep->GetSignalValues();

      ep->GetNoiseHis();
      ep->GetNoiseValues(i);
      ep->GetQcurveValues();      
      ep->IncrementAveragedEvent();

      //if(cc==0){
      c->cd(1);
      c->GetPad(1)->SetGridy();
      c->GetPad(1)->SetGridx();
      ep->sg_hSvsF->SetLineColor(4);ep->sg_hSvsF->SetMarkerColor(4);
      ep->sg_hSvsF->DrawCopy();
      //ep->sg_hSvsF_bkgrd->Draw("same");
      //ep->ffn_p2p3->DrawCopy("Lsame");
      //ep->ffn_p2->Draw("Lsame");
      ep->sg_hSvsF_signal->SetLineColor(8);ep->sg_hSvsF_signal->SetMarkerColor(8);
      //ep->sg_hSvsF_signal->DrawCopy("same");
      if(ep->bl_provided){ep->bl_hSvsF->Draw("same");}

      c->cd(2);
      c->GetPad(2)->SetGridy();
      c->GetPad(2)->SetGridx();

      ep->sg_hSvsF_noise->Draw();
      /*ep->sg_hSvsF->Draw();
      ep->sg_hSvsF->GetYaxis()->SetRangeUser(-0.05,0.05);
      ep->sg_hSvsF_bkgrd->Draw("same");
      ep->ffn_p2->Draw("Lsame");
      ep->ffn_p2p3->Draw("Lsame");
      //ep->sg_hSvsF_signal->DrawCopy("same");*/

      c->cd(3);
      c->GetPad(3)->SetGridy();
      c->GetPad(3)->SetGridx();

      ep->sg_hSvsF_bkgrd->Draw();
      if(ep->bl_provided){ep->sg_hSvsF_subtr->Draw("same");}
      ep->ffn_p2p3->Draw("Lsame");

      c->cd(4);
      c->GetPad(4)->SetGridy();
      c->GetPad(4)->SetGridx();

      ep->sg_hSvsF_signal->Draw();
      //ep->sg_hSvsF_signal->GetYaxis()->SetRangeUser(-0.01,0.01);
      //ep->sg_hSvsF_signal->GetYaxis()->SetRangeUser(-0.01,0.08);
      ep->ffn_g1->Draw("Lsame");
      ep->ffn_g2->Draw("Lsame");
      c->GetPad(3)->SetGridy();
      c->Update();
      c->Modified();

      //pdfn ="pdf/cold/60_fit_"+ep_fn+cnfg+".pdf";
      pdfn ="pdf/cold/70_fit_"+ep_fn+cnfg+".pdf";
      //c->Print(pdfn.Data());
      //}
      /*if(cc==1){
      c2->cd(1);
      ep->sg_hSvsF->Draw();
      ep->sg_hSvsF_bkgrd->Draw("same");
      ep->ffn_p2p3->Draw("Lsame");
      if(ep->bl_provided){ep->bl_hSvsF->Draw("same");}
      c2->cd(2);
      ep->sg_hSvsF_bkgrd->Draw();
      if(ep->bl_provided){ep->sg_hSvsF_subtr->Draw("same");}
      ep->ffn_p2p3->Draw("Lsame");
      c2->cd(3);
      ep->sg_hSvsF_signal->Draw();
      ep->sg_hSvsF_signal->GetYaxis()->SetRangeUser(-0.01,0.01);
      c2->GetPad(3)->SetGridy();
      c2->Update();
      c2->Modified();

      }*/

      cout<<"event: "<<i<<" "<<cc<<endl;
      
      ep->WriteAveEvent();
      
      /*c1->cd();
      TH1D *his = (TH1D*)ep->sg_hSvsF->Clone(Form("b_h%d",i));
      if(i==0){his->Draw();}
      else{his->Draw("same");}  
      c1->Update();
      c1->Modified();*/
      usleep(100000);
      cc++;
    }


  }

  //pdfn ="pdf/cold/60_fit_"+ep_fn+cnfg+".pdf]";
  pdfn ="pdf/cold/70_fit_"+ep_fn+cnfg+".pdf]";
  //c->Print(pdfn.Data());

  //c->GetPad(1)->Print("pdf/lanl_sig_ex.pdf");
  TCanvas *c2=new TCanvas("c2","c2",1600,1000);
  c2->Divide(2,2);

  c2->cd(1);
  ep->sg_gPvsT->SetDecorationAll(0, 2,1, 2,0.4,24);
  //ep->sg_gPvsT->GetYaxis()->SetRangeUser(-4.2,-1.5);
  ep->sg_gPvsT->Draw("AP");
  c2->cd(2);
  //ep->sg_g_sgl1_ampl->SetDecorationAll(0, 2,1, 2,0.4,24);
  //ep->sg_g_sgl1_ampl->Draw("AP");
  ep->sg_hSvsF_noise_all->Draw();
  c2->cd(3);
  //ep->sg_g_sgl2_ampl->SetDecorationAll(0, 2,1, 2,0.4,24);
  //ep->sg_g_sgl2_ampl->Draw("AP");
  ep->sg_g_noise_mean->Draw("AP");
  c2->cd(4);
  //ep->sg_g_sgl_arat->SetDecorationAll(0, 2,1, 2,0.4,24);
  //ep->sg_g_sgl_arat->Draw("AP");
  ep->sg_g_noise_sigm->Draw("AP");

  

  ep->WriteSignResults();  
  theApp->Run();  
  //exit(2323);
}

