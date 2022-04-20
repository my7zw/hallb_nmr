#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>
#include <iomanip>

#include "TRint.h"
#include "TFile.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH1D.h"
#include "TGaxis.h"
#include "TF1.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "THupgrade.hh"



using std::cout;
using std::endl;

void prt_latex(Double_t _x, Double_t _y, const char *tex, Int_t _col){
  TLatex *t = new TLatex(); t->SetNDC(); t->SetTextColor(_col); t->SetTextSize(0.04);
  t->DrawLatex(_x,_y,tex);
}

int main(int argc, char **argv)
{
  //TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  gROOT->ForceStyle();
  gStyle->SetLineScalePS(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TGaxis::SetMaxDigits(3);
  //TString dir = "mdata/data_86_0/root_res_cnfg_1/";
  //TString dir = "mdata/data_86_0/root_res_cnfg_te/";
  TString dir = "mdata/data_86_87/root_res_cnfg_teen_n50/";
  
  //TString nn_us = "2019-08-30_14h36m04s_area.root";

  TString nn_us = "2019-08-30_14h48m42s_area.root";
  TString nn_ub = "2019-08-30_16h30m22s_area.root";
  TString nn_ls = "QCV3650035213_area.root";
  //TString nn_lb = "QCV3650035586_area.root";
  TString nn_lb = "QCV3650041681_area.root";

  TFile *in_f_us = new TFile(dir+nn_us,"READ");
  TFile *in_f_ub = new TFile(dir+nn_ub,"READ");
  TFile *in_f_ls = new TFile(dir+nn_ls,"READ");
  TFile *in_f_lb = new TFile(dir+nn_lb,"READ");

  //Int_t nav=1,iev=640;
  Int_t nav=50,iev=0;
  TString hn_sig = Form("sgh_signal_%d_%d",nav,iev);
  TString hn_bgr = Form("sgh_subtr_%d_%d",nav,iev);
  TString hn_fit = Form("p2p3_%d_%d",nav,iev);
  
  TH1Du *h_us     = (TH1Du*)in_f_us->Get(hn_sig);
  TH1Du *h_us_bgr = (TH1Du*)in_f_us->Get(hn_bgr);
  TF1   *h_us_fit = (TF1*)  in_f_us->Get(hn_fit);


  nav=50,iev=0;
  hn_sig = Form("sgh_signal_%d_%d",nav,iev);
  hn_bgr = Form("sgh_subtr_%d_%d",nav,iev);
  hn_fit = Form("p2p3_%d_%d",nav,iev);
  
  TH1Du *h_ub     = (TH1Du*)in_f_ub->Get(hn_sig);
  TH1Du *h_ub_bgr = (TH1Du*)in_f_ub->Get(hn_bgr);
  TF1  *h_ub_fit  = (TF1*)in_f_ub->Get(hn_fit);

  TH1Du *h_ls     = (TH1Du*)in_f_ls->Get(hn_sig);
  TH1Du *h_ls_bgr = (TH1Du*)in_f_ls->Get(hn_bgr);
  TF1  *h_ls_fit  = (TF1*)in_f_ls->Get(hn_fit);

  TH1Du *h_lb     = (TH1Du*)in_f_lb->Get(hn_sig);
  TH1Du *h_lb_bgr = (TH1Du*)in_f_lb->Get(hn_bgr);
  TF1  *h_lb_fit  = (TF1*)in_f_lb->Get(hn_fit);

  h_us->SetTitle("UVA_9205_small");
  h_us_bgr->SetTitle("UVA_9205_bgr_small");
  h_us_fit->SetTitle("UVA_9205_fit_small");


  h_ub->SetTitle("UVA_9205_big");
  h_ub_bgr->SetTitle("UVA_9205_bgr_big");
  h_ub_fit->SetTitle("UVA_9205_fit_big");
  
 
  h_ls->SetTitle("LANL_ch2_small");
  h_ls_bgr->SetTitle("LANL_ch2_bgr_small");
  h_ls_fit->SetTitle("LANL_ch2_fit_small");
  
 
  h_lb->SetTitle("LANL_ch2_big");
  h_lb_bgr->SetTitle("LANL_ch2_bgr_big");
  h_lb_fit->SetTitle("LANL_ch2_fit_big");

  
  h_us->SetDecorationAll(0, 4,1, 4,0.2,2);
  h_us_bgr->SetDecorationAll(0, 4,1, 4,1.1,3);
  h_us_fit->SetLineColor(1);
  h_us_fit->SetLineWidth(1);
  
  h_ub->SetDecorationAll(0, 4,1, 4,0.2,2);
  h_ub_bgr->SetDecorationAll(0, 4,1, 4,1.1,3);
  h_ub_fit->SetLineColor(1);
  h_ub_fit->SetLineWidth(1);
 
  h_ls->SetDecorationAll(0, 2,1, 2,0.2,2);
  h_ls_bgr->SetDecorationAll(0, 2,1, 2,1.1,3);
  h_ls_fit->SetLineColor(1);
  h_ls_fit->SetLineWidth(1);  

  h_lb->SetDecorationAll(0, 2,1, 2,0.2,2);
  h_lb_bgr->SetDecorationAll(0, 2,1, 2,1.1,3);
  h_lb_fit->SetLineColor(1);
  h_lb_fit->SetLineWidth(1);  

  TCanvas *c=new TCanvas("c","c",1600,900);
  c->Divide(3,2);

  for(Int_t i=1;i<7;i++){
    //c->GetPad(i)->SetGridx();
    //c->GetPad(i)->SetGridy();
    c->GetPad(i)->SetTickx();
    c->GetPad(i)->SetTicky();
  }
  c->cd(1);
  h_lb->GetYaxis()->UnZoom();
  h_ls->GetYaxis()->UnZoom();
  h_ub->GetYaxis()->UnZoom();
  h_us->GetYaxis()->UnZoom();
  //h_ls->DrawCopy("HIST");
  //h_us->DrawCopy("HISTsame");
  //c->GetPad(1)->BuildLegend(0.58,0.39,0.86,0.60);
  
  c->cd(4);
  //h_lb->GetYaxis()->SetRangeUser(-0.015,0.015);
  //h_lb->DrawCopy("HIST");
  //h_ub->DrawCopy("HISTsame");
  c->GetPad(4)->Update();
  
  Double_t amax_s = h_ls->GetBinContent(h_ls->GetMaximumBin());
  Double_t amax_b = h_lb->GetBinContent(h_lb->GetMaximumBin());
  //Double_t amin = h_lb->GetBinContent(h_lb->GetMinimumBin());
  h_us->Scale(amax_s/h_us->GetBinContent(h_us->GetMaximumBin()));
  h_ub->Scale(amax_b/h_ub->GetBinContent(h_ub->GetMaximumBin()));
  //h_ls->Scale(amax/h_ls->GetBinContent(h_ls->GetMaximumBin()));

  Double_t v1 = h_us->GetBinContent(h_us->GetMaximumBin());
  Double_t v2 = h_ub->GetBinContent(h_ub->GetMaximumBin());
  Double_t v3 = h_ls->GetBinContent(h_ls->GetMaximumBin());
  //cout<<"scale "<<amax/v1<<endl;
  //cout<<"scale "<<amax/v2<<endl;
  //cout<<"scale "<<amax/v3<<endl;

  c->cd(6);
  h_lb->GetYaxis()->UnZoom();
  //h_lb->GetXaxis()->SetRangeUser(224.30,224.38);

  TH1Du *h_ub_cl = (TH1Du*)h_ub->Clone("h_ub_cl");
  TH1Du *h_ub_df = (TH1Du*)h_ub->Clone("h_ub_df");
  Double_t offset= 0.0;
  cout<<h_ub->GetNbinsX()<<" "<<h_lb->GetNbinsX()<<endl;
  for(Int_t i=0; i<h_ub_cl->GetNbinsX(); i++){
    h_ub_cl->SetBinContent(i,h_ub->GetBinContent(i)+offset);
    h_ub_df->SetBinContent(i,h_lb->GetBinContent(i)-h_ub->GetBinContent(i));
  }
  h_ub_cl->Draw("HIST");
  h_ub_cl->GetXaxis()->SetLabelSize(0.055);
  h_ub_cl->GetXaxis()->SetNdivisions(403);
  h_ub_cl->GetYaxis()->SetLabelSize(0.055);
  h_ub_cl->GetYaxis()->SetNdivisions(305);
  h_ub_cl->GetYaxis()->SetRangeUser(-0.25,2.25);
  h_lb->Draw("HISTsame");

  c->cd(4);h_ub_df->Draw("HIST");

   
  c->cd(5);
  //h_lb->GetYaxis()->SetRangeUser(-0.015,0.015);
  //h_lb->GetXaxis()->SetRangeUser(224.68,224.8);
  //h_lb->DrawCopy("HIST");
  TH1Du *h_us_cl = (TH1Du*)h_us->Clone("h_us_cl");
  TH1Du *h_us_df = (TH1Du*)h_us->Clone("h_us_df");
  for(Int_t i=0; i<h_us_cl->GetNbinsX(); i++){
    h_us_cl->SetBinContent(i,h_us->GetBinContent(i)+offset);
    h_us_df->SetBinContent(i,h_ls->GetBinContent(i)-h_us->GetBinContent(i));
  }
  h_us_cl->Draw("HIST");
  h_us_cl->GetXaxis()->SetLabelSize(0.055);
  h_us_cl->GetXaxis()->SetNdivisions(403);
  h_us_cl->GetYaxis()->SetLabelSize(0.055);
  h_us_cl->GetYaxis()->SetNdivisions(305);
  h_us_cl->GetYaxis()->SetRangeUser(-0.001,0.008);
  h_ls->Draw("HISTsame");

  c->cd(1);h_us_df->Draw("HIST");

  c->cd(2);
  //h_lb_bgr->GetYaxis()->UnZoom();

  h_ls_bgr->Draw("HIST");
  h_ls_bgr->GetXaxis()->SetLabelSize(0.055);
  h_ls_bgr->GetXaxis()->SetNdivisions(403);
  h_ls_bgr->GetYaxis()->SetLabelSize(0.055);
  h_ls_bgr->GetYaxis()->SetNdivisions(305);
  h_ls_bgr->GetYaxis()->SetRangeUser(-0.005,0.075);
  h_us_bgr->Draw("HISTsame");
  h_ls_fit->Draw("HISTsame");
  h_us_fit->Draw("HISTsame");

  c->cd(3);
  h_lb_bgr->Draw("HIST");
  h_lb_bgr->GetXaxis()->SetLabelSize(0.055);
  h_lb_bgr->GetXaxis()->SetNdivisions(403);
  h_lb_bgr->GetYaxis()->SetLabelSize(0.055);
  h_lb_bgr->GetYaxis()->SetNdivisions(305);
  h_lb_bgr->GetYaxis()->SetRangeUser(-0.25,2.25);
  h_ub_bgr->Draw("HISTsame");
  h_lb_fit->Draw("HISTsame");
  h_ub_fit->Draw("HISTsame");
  //h_lb_bgr->GetYaxis()->SetRangeUser(-0.3,2.3);

  
  //c->SetGridy();
  //c->BuildLegend(0.6,0.14,0.89,0.36);
    
  c->Update();
  c->Modified();

  //c->SaveAs("pdf/pol_signals.pdf");
  c->SaveAs("pdf/pol_signals_upd.pdf");
  theApp->Run();  

}

