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

  //TString dir = "mdata/data_86_0/root_res_cnfg_1/";
  //TString dir = "mdata/data_86_0/root_res_cnfg_te/";
  TString dir = "mdata/data_86_87/root_res_cnfg_teen_n50/";
  

  TString nn_up = "2019-08-30_16h40m53s_area.root";
  TString nn_um = "2019-08-30_18h26m43s_area.root";
  TString nn_lp = "QCV3650041681_area.root";
  TString nn_lm = "QCV3650048659_area.root";

  /*TString nn_up = "2019-08-30_20h11m50s_area.root";
  TString nn_um = "2019-08-30_21h45m25s_area.root";
  TString nn_lp = "QCV3650054965_area.root";
  TString nn_lm = "QCV3650060208_area.root";*/

  
  TFile *in_f_up = new TFile(dir+nn_up,"READ");
  TFile *in_f_um = new TFile(dir+nn_um,"READ");
  TFile *in_f_lp = new TFile(dir+nn_lp,"READ");
  TFile *in_f_lm = new TFile(dir+nn_lm,"READ");

  //Int_t nav=1,iev=640;
  Int_t nav=50,iev=1;
  TString hn_sig = Form("sgh_signal_%d_%d",nav,iev);
  TString hn_bgr = Form("sgh_subtr_%d_%d",nav,iev);
  TString hn_fit = Form("p2p3_%d_%d",nav,iev);
  
  TH1Du *h_up     = (TH1Du*)in_f_up->Get(hn_sig);
  TH1Du *h_up_bgr = (TH1Du*)in_f_up->Get(hn_bgr);
  TF1   *h_up_fit = (TF1*)  in_f_up->Get(hn_fit);


  nav=50,iev=1;
  hn_sig = Form("sgh_signal_%d_%d",nav,iev);
  hn_bgr = Form("sgh_subtr_%d_%d",nav,iev);
  hn_fit = Form("p2p3_%d_%d",nav,iev);
  
  TH1Du *h_um     = (TH1Du*)in_f_um->Get(hn_sig);
  TH1Du *h_um_bgr = (TH1Du*)in_f_um->Get(hn_bgr);
  TF1  *h_um_fit  = (TF1*)in_f_um->Get(hn_fit);

  TH1Du *h_lp     = (TH1Du*)in_f_lp->Get(hn_sig);
  TH1Du *h_lp_bgr = (TH1Du*)in_f_lp->Get(hn_bgr);
  TF1  *h_lp_fit  = (TF1*)in_f_lp->Get(hn_fit);

  TH1Du *h_lm     = (TH1Du*)in_f_lm->Get(hn_sig);
  TH1Du *h_lm_bgr = (TH1Du*)in_f_lm->Get(hn_bgr);
  TF1  *h_lm_fit  = (TF1*)in_f_lm->Get(hn_fit);

  h_up->SetTitle("UVA_pls");
  h_up_bgr->SetTitle("UVA_pls_bgr");
  h_up_fit->SetTitle("UVA_pls_fit");


  h_um->SetTitle("UVA_mns");
  h_um_bgr->SetTitle("UVA_mns_bgr");
  h_um_fit->SetTitle("UVA_mns_fit");
  
 
  h_lp->SetTitle("LANL_pls");
  h_lp_bgr->SetTitle("LANL_pls_bgr");
  h_lp_fit->SetTitle("LANL_pls_fit");
  
  h_lm->SetTitle("LANL_mns");
  h_lm_bgr->SetTitle("LANL_mns_bgr");
  h_lm_fit->SetTitle("LANL_mns_fit");
  
  h_up->SetDecorationAll(0, 4,1, 4,0.2,2);
  h_up_bgr->SetDecorationAll(0, 4,1, 4,1.1,3);
  h_up_fit->SetLineColor(1);
  h_up_fit->SetLineWidth(1);
  
  h_um->SetDecorationAll(0, 4,1, 4,0.2,2);
  h_um_bgr->SetDecorationAll(0, 4,1, 4,1.1,3);
  h_um_fit->SetLineColor(1);
  h_um_fit->SetLineWidth(1);
 
  h_lp->SetDecorationAll(0, 2,1, 2,0.2,2);
  h_lp_bgr->SetDecorationAll(0, 2,1, 2,1.1,3);
  h_lp_fit->SetLineColor(1);
  h_lp_fit->SetLineWidth(1);  

  h_lm->SetDecorationAll(0, 2,1, 2,0.2,2);
  h_lm_bgr->SetDecorationAll(0, 2,1, 2,1.1,3);
  h_lm_fit->SetLineColor(1);
  h_lm_fit->SetLineWidth(1);  

  TCanvas *c=new TCanvas("c","c",1800,900);
  c->Divide(3,2);

  for(Int_t i=1;i<7;i++){
    //c->GetPad(i)->SetGridx();
    //c->GetPad(i)->SetGridy();
    c->GetPad(i)->SetTickx();
    c->GetPad(i)->SetTicky();
  }
  c->cd(1);
  h_lm->GetYaxis()->UnZoom();
  h_lp->GetYaxis()->UnZoom();
  h_um->GetYaxis()->UnZoom();
  h_up->GetYaxis()->UnZoom();
  //h_lp->DrawCopy("HIST");
  //h_up->DrawCopy("HISTsame");
  //c->GetPad(1)->BuildLegend(0.58,0.39,0.86,0.60);
  
  c->cd(4);
  //h_lm->GetYaxis()->SetRangeUser(-0.015,0.015);
  //h_lm->DrawCopy("HIST");
  //h_um->DrawCopy("HISTsame");
  c->GetPad(4)->Update();
  
  Double_t amax_p = h_lp->GetBinContent(h_lp->GetMaximumBin());
  Double_t amin_m = h_lm->GetBinContent(h_lm->GetMinimumBin());
  h_up->Scale(amax_p/h_up->GetBinContent(h_up->GetMaximumBin()));
  h_um->Scale(amin_m/h_um->GetBinContent(h_um->GetMinimumBin()));

  Double_t v1 = h_up->GetBinContent(h_up->GetMaximumBin());
  Double_t v2 = h_um->GetBinContent(h_um->GetMaximumBin());
  Double_t v3 = h_lp->GetBinContent(h_lp->GetMaximumBin());
  //cout<<"scale "<<amax/v1<<endl;
  //cout<<"scale "<<amax/v2<<endl;
  //cout<<"scale "<<amax/v3<<endl;
  c->cd(6);
  h_lm->GetYaxis()->UnZoom();
  //h_lm->GetXaxis()->SetRangeUser(224.30,224.38);

  TH1Du *h_um_cl = (TH1Du*)h_um->Clone("h_um_cl");
  TH1Du *h_um_df = (TH1Du*)h_um->Clone("h_um_df");
  Double_t offset= 0.0;
  cout<<h_um->GetNbinsX()<<" "<<h_lm->GetNbinsX()<<endl;
  for(Int_t i=0; i<h_um_cl->GetNbinsX(); i++){
    h_um_cl->SetBinContent(i,h_um->GetBinContent(i)+offset);
    h_um_df->SetBinContent(i,h_lm->GetBinContent(i)-h_um->GetBinContent(i));
  }

  h_um_cl->Draw("HIST");
  h_lm->Draw("HISTsame");
  c->cd(4);h_um_df->Draw("HIST");

   
  c->cd(5);
  //h_lm->GetYaxis()->SetRangeUser(-0.015,0.015);
  //h_lm->GetXaxis()->SetRangeUser(224.68,224.8);
  //h_lm->DrawCopy("HIST");
  TH1Du *h_up_cl = (TH1Du*)h_up->Clone("h_up_cl");
  TH1Du *h_up_df = (TH1Du*)h_up->Clone("h_up_df");
  for(Int_t i=0; i<h_up_cl->GetNbinsX(); i++){
    h_up_cl->SetBinContent(i,h_up->GetBinContent(i)+offset);
    h_up_df->SetBinContent(i,h_lp->GetBinContent(i)-h_up->GetBinContent(i));
  }
  h_up_cl->Draw("HIST");
  h_lp->Draw("HISTsame");
  c->cd(1);h_up_df->Draw("HIST");

  c->cd(2);
  //h_lm_bgr->GetYaxis()->UnZoom();

  h_lp_bgr->Draw("HIST");
  h_up_bgr->Draw("HISTsame");
  h_lp_fit->Draw("HISTsame");
  h_up_fit->Draw("HISTsame");
  //h_lp_bgr->GetYaxis()->SetRangeUser(0.,0.07); 
  

  
  c->cd(3);
  h_lm_bgr->Draw("HIST");
  h_um_bgr->Draw("HISTsame");
  h_lm_fit->Draw("HISTsame");
  h_um_fit->Draw("HISTsame");
  //h_lm_bgr->GetYaxis()->SetRangeUser(-0.3,2.3);

  
  //c->SetGridy();
  //c->BuildLegend(0.6,0.14,0.89,0.36);
    
  c->Update();
  c->Modified();

  //c->SaveAs("pdf/pol_signals.pdf");
  //c->SaveAs("pdf/pol_signals_upd.pdf");
  theApp->Run();  

}

