#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <iomanip>

#include "TRint.h"
#include "TFile.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH1D.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "THupgrade.hh"

#include "mclas/T7301theq.hh"

using std::cout;
using std::endl;

void prt_latex(Double_t _x, Double_t _y, const char *tex){
  TLatex *t = new TLatex(); t->SetNDC(); t->SetTextColor(1); t->SetTextSize(0.04);
  t->DrawLatex(_x,_y,tex);
}

int main(int argc, char **argv)
{
  //TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);

  gStyle->SetLineScalePS(1);
  T7301theq *te = new T7301theq("");
  
  //TString dir = "mdata/data_86_87/root_res_cnfg_calib_n1/";//te
  //TString dir = "mdata/data_86_87/root_res_cnfg_calib_en/";
  TString dir = "mdata/data_86_87/root_res_cnfg_calib_tail/";
  TString fn = "2019-08-30_14h36m04s_area.root";
  TString hn = fn; hn.Remove(hn.Last('_'));
  //TString hn = "2019-08-30_14h36m04s";
  fn.Prepend(dir.Data());
  //cout<<fn<<endl;

  TFile *in_f_u = new TFile(fn.Data(),"READ");
  TGraphErrorsu *gr_u = (TGraphErrorsu*)in_f_u->Get(hn);
  gr_u->SetDecorationAll(0, 4,1, 4,0.7,21);
  gr_u->SetName("86_UVA_A");
  gr_u->SetTitle("86_UVA Area vs time");

  TString hnn = hn + "_4HeP_t";
  TGraphErrorsu *gr_u_p = (TGraphErrorsu*)in_f_u->Get(hnn);
  gr_u_p->SetDecorationAll(0, 4,1, 4,0.7,21);
  gr_u_p->SetName("86_UVA_P");
  gr_u_p->SetTitle("86_UVA 4HeP vs time");

  hnn = hn + "_4HeT_t";
  TGraphErrorsu *gr_u_t = (TGraphErrorsu*)in_f_u->Get(hnn);
  gr_u_t->SetDecorationAll(0, 4,1, 4,0.7,21);
  gr_u_t->SetName("86_UVA_T");
  gr_u_t->SetTitle("86_UVA 4HeT vs time");

  Int_t np      = gr_u->GetN();
  Double_t *xp  = gr_u->GetX();
  Double_t *exp = gr_u->GetEX();
  Double_t *yp  = gr_u->GetY();
  Double_t *eyp = gr_u->GetEY();

  Double_t *He4P  = gr_u_p->GetY();

  TGraphErrorsu *gr_tep = new TGraphErrorsu();
  TGraphErrorsu *gr_cal = new TGraphErrorsu();
  gr_tep->SetDecorationAll(0, 2,1, 2,0.7,21);
  gr_tep->SetName("86_UVA_A");
  gr_tep->SetTitle("86_UVA TE Pol. vs #");

  gr_cal->SetDecorationAll(0, 2,1, 2,0.7,21);
  gr_cal->SetName("86_UVA_A");
  gr_cal->SetTitle("86_UVA Cal. vs #");

  Int_t cc = 0;
  for(Int_t j=0;j<np;j++){
    Double_t tep = te->CalculateTEP("proton", .5, 5.00, He4P[j]);
    Double_t calib = tep/yp[j];
    cout<<np<<" "<<j<<" "<<std::fixed<<std::setprecision(0)
	<<std::setw(20)<<xp[j]
	<<std::setw(20)<<std::setprecision(9)<<yp[j]
      	<<std::setw(20)<<std::setprecision(6)<<tep
      	<<std::setw(20)<<std::setprecision(6)<<calib
	<<endl;
    gr_tep->SetPoint(cc,j+1,tep);
    gr_cal->SetPoint(cc,j+1,calib);
    cc++;
  }
  
  //delete xp; xp = NULL;
  //delete yp; yp = NULL;
  
  
  TCanvas *c=new TCanvas("c","c",1600,1000);
  c->Divide(2,2);
  c->cd(1);
  gr_u->Draw("AP");
  gr_u->GetXaxis()->SetTimeDisplay(1);
  gr_u->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
  gr_u->GetXaxis()->SetLabelOffset(0.02);
  gr_u->GetYaxis()->SetRangeUser(0.04e-3, 0.18e-3);
  
  c->cd(2);
  gr_u_p->Draw("AP");
  gr_u_p->GetXaxis()->SetTimeDisplay(1);
  gr_u_p->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
  gr_u_p->GetXaxis()->SetLabelOffset(0.02);

  c->cd(3);
  gr_u_t->Draw("AP");
  gr_u_t->GetXaxis()->SetTimeDisplay(1);
  gr_u_t->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
  gr_u_t->GetXaxis()->SetLabelOffset(0.02);

    //c->SetLogy();
  //c->SetGridy();
  //c->SetGridx();
  //c->BuildLegend();
  //c->SaveAs("pdf/theq_apt_te.pdf");
  //c->SaveAs("pdf/theq_apt_en.pdf");
  //c->SaveAs("pdf/theq_apt_tail.pdf");

  TGraphErrorsu *gr_cal_r = new TGraphErrorsu();
  gr_cal_r->SetDecorationAll(0, 2,1, 2,0.7,21);
  gr_cal_r->SetName("86_UVA_cal_r");
  gr_cal_r->SetTitle("86_UVA TE Cal. vs p0 fit range");

  Double_t fit_over = 3;
  Int_t cc1=0;
  for(Int_t j=fit_over;j<np;j++){
    TF1 *pol0_u = new TF1("ft","pol0",np-fit_over-j,np);
    gr_cal->Fit(pol0_u,"QNR");
    Double_t cal_av = pol0_u->GetParameter(0);
    Double_t cal_av_er = pol0_u->GetParError(0);
    gr_cal_r->SetPoint(cc1, j, cal_av);
    gr_cal_r->SetPointError(cc1, 0., cal_av_er);
    cc1++;
  }
  
  TCanvas *c1=new TCanvas("c1","c1",1600,1000);
  c1->Divide(2,2);
  
  c1->cd(1);
  gr_tep->Draw("AP");
  gr_tep->GetYaxis()->SetRangeUser(0.307e-2, 0.309e-2);
  
  c1->cd(2);
  gr_cal->Draw("AP");
  gr_cal->GetYaxis()->SetRangeUser(15.,40.);
  
  c1->cd(3);
  gr_cal_r->Draw("AP");
  gr_cal_r->GetYaxis()->SetRangeUser(20.,30.);
  TF1 *pol0_c = new TF1("ft","pol0",0.,8.);
  pol0_c->SetLineColor(1);  pol0_c->SetLineWidth(1);
  gr_cal_r->Fit(pol0_c,"R");
  Double_t calconst = pol0_c->GetParameter(0);
  TString lat = Form("cal.cnst.: %2.3f",calconst);
  prt_latex(0.12,0.85,lat.Data());


  c1->Update();
  //c1->SaveAs("pdf/theq_cal_te.pdf");
  //c1->SaveAs("pdf/theq_cal_en.pdf");
  //c1->SaveAs("pdf/theq_cal_tail.pdf");

  //gr_u->GetXaxis()->SetRangeUser(1567187920, 1567192605);
  //gr_u->GetYaxis()->SetRangeUser(-0.0002,0.0004);
  //c->Update(); c->Modified();

  theApp->Run();  

}

