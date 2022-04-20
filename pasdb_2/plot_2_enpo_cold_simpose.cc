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
  gStyle->SetLineScalePS(1);
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TString dir = "mdata/data_deucold/root_res_cnfg_1/";

  //TString nn_ls = "QCV3640715679_area.root";
  TString nn_lb = "QCV3625701939_area.root";
  TFile *in_f_lb = new TFile(dir+nn_lb,"READ");

  Int_t nav=10,iev=98;
  TString hn_raw = Form("QCV3625701939_%d_%d",nav,iev);
  TString hn_sig = Form("sgh_signal_%d_%d",nav,iev);
  TString hn_bgr = Form("sgh_subtr_%d_%d",nav,iev);
  TString hn_fit = Form("p2p3_%d_%d",nav,iev);

  TH1Du *h_lb_raw = (TH1Du*)in_f_lb->Get(hn_raw);
  TH1Du *h_lb_sig = (TH1Du*)in_f_lb->Get(hn_sig);
  TH1Du *h_lb_bgr = (TH1Du*)in_f_lb->Get(hn_bgr);
  TF1   *h_lb_fit = (TF1*)in_f_lb->Get(hn_fit);

  h_lb_raw->SetTitle("deut_big_raw");
  h_lb_sig->SetTitle("deut_big_sig");
  h_lb_bgr->SetTitle("deut_big_bgr");
  h_lb_fit->SetTitle("deut_big_fit");

  h_lb_raw->SetDecorationAll(0, 4,1, 4,0.2,2);
  h_lb_sig->SetDecorationAll(0, 2,1, 2,0.2,2);
  h_lb_bgr->SetDecorationAll(0, 2,1, 2,1.1,3);
  h_lb_fit->SetLineColor(1);
  h_lb_fit->SetLineWidth(1);  

  TCanvas *c=new TCanvas("c","c",1400,600);
  c->Divide(2,1);

  for(Int_t i=1;i<3;i++){
    //c->GetPad(i)->SetGridx();
    //c->GetPad(i)->SetGridy();
    c->GetPad(i)->SetTickx();
    c->GetPad(i)->SetTicky();
  }

  h_lb_raw->GetYaxis()->UnZoom();
  h_lb_sig->GetYaxis()->UnZoom();
  
  c->cd(1);
  h_lb_raw->Draw("HIST");
  h_lb_raw->GetXaxis()->SetLabelSize(0.055);
  h_lb_raw->GetXaxis()->SetNdivisions(505);
  h_lb_raw->GetYaxis()->SetLabelSize(0.055);
  h_lb_raw->GetYaxis()->SetNdivisions(505);
  h_lb_raw->GetYaxis()->SetRangeUser(-0.01,0.09);
  h_lb_fit->DrawCopy("HISTsame");

  c->cd(2);
  h_lb_sig->Draw("HIST");  
  h_lb_sig->GetXaxis()->SetLabelSize(0.055);
  h_lb_sig->GetXaxis()->SetNdivisions(505);
  h_lb_sig->GetYaxis()->SetLabelSize(0.055);
  h_lb_sig->GetYaxis()->SetNdivisions(505);
  h_lb_sig->GetYaxis()->SetRangeUser(-0.008,0.035);

      
  c->Update();
  c->Modified();

  c->SaveAs("pdf/deut_signals.pdf");
  theApp->Run();  

}

