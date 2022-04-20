#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>
#include <iomanip>

#include "TRint.h"
#include "TMath.h"
#include "TFile.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "THupgrade.hh"



using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  //TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  gStyle->SetOptFit(1111);
  gStyle->SetLineScalePS(1);
  
  //TString dir = "mdata/data_60_1/root_res_cnfg_1/";
  //TString dir = "mdata/data_60_1/root_res_cnfg_smasig_n10/";
  //TString dir = "mdata/data_60_1/root_res_cnfg_bigsig_n10/";
  TString dir = "mdata/data_60_1/root_res_cnfg_bigsig/";

  Double_t db_set[21] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48,
			 51, 54, 57, 60};
  
  TString nn_u1[21] = {"2019-05-13_16h53m26s_area.root",
		      "2019-05-13_16h55m17s_area.root",
		      "2019-05-13_16h57m08s_area.root",
		      "2019-05-13_16h59m11s_area.root",
		      "2019-05-13_17h01m08s_area.root",
		      "2019-05-13_17h03m34s_area.root",
		      "2019-05-13_17h05m54s_area.root",
		      "2019-05-13_17h07m58s_area.root",
		      "2019-05-13_17h09m57s_area.root",
		      "2019-05-13_17h11m52s_area.root",
		      "2019-05-13_17h14m29s_area.root",
		      "2019-05-13_17h16m46s_area.root",
		      "2019-05-13_17h18m50s_area.root",
		      "2019-05-13_17h21m29s_area.root",
		      "2019-05-13_17h24m43s_area.root",
		      "2019-05-13_17h26m59s_area.root",
		      "2019-05-13_17h28m58s_area.root",
		      "2019-05-13_17h31m28s_area.root",
		      "2019-05-13_17h33m59s_area.root",
		      "2019-05-13_17h36m20s_area.root",
		      "2019-05-13_17h38m42s_area.root"};

  TString nn_l1[21] = {"QCV3640711425_area.root",
		       "QCV3640711646_area.root",
		       "QCV3640711868_area.root",
		       "QCV3640712110_area.root",
		       "QCV3640712349_area.root",
		       "QCV3640712602_area.root",
		       "QCV3640712847_area.root",
		       "QCV3640713044_area.root",
		       "QCV3640713288_area.root",
		       "QCV3640713545_area.root",
		       "QCV3640713740_area.root",
		       "QCV3640713929_area.root",
		       "QCV3640714105_area.root",
		       "QCV3640714339_area.root",
		       "QCV3640714555_area.root",
		       "QCV3640714757_area.root",
		       "QCV3640714946_area.root",
		       "QCV3640715156_area.root",
		       "QCV3640715334_area.root",
		       "QCV3640715505_area.root",
		       "QCV3640715679_area.root"};

  TFile *in_f_u1[21];
  TGraph *gr_u1[21];  
  for(Int_t i=0;i<21;i++){
    TString hn = nn_u1[i];
    hn.Remove(hn.Last('_'));
    nn_u1[i].Prepend(dir.Data());
    in_f_u1[i] = new TFile(nn_u1[i].Data(),"READ");
    gr_u1[i] = (TGraph*)in_f_u1[i]->Get(hn);
  }

  
  TFile *in_f_l1[21];
  TGraph *gr_l1[21];  
  for(Int_t i=0;i<21;i++){
    TString hn = nn_l1[i];
    hn.Remove(hn.Last('_'));
    nn_l1[i].Prepend(dir.Data());
    in_f_l1[i] = new TFile(nn_l1[i].Data(),"READ");
    gr_l1[i] = (TGraph*)in_f_l1[i]->Get(hn);
  }


  Int_t cc=0;
  Double_t ev_start = 0;
  
  //-----------------------------------------------------------------------
  
  TGraphErrors *grall_u1 = new TGraphErrors();
  grall_u1->SetName("60_1_UVA9807c");  grall_u1->SetTitle("60_1_UVA9807c");
  grall_u1->SetMarkerStyle(21);  grall_u1->SetMarkerColor(4); grall_u1->SetFillColor(0);

  TGraphErrors *gr1pt_u1 = new TGraphErrors();
  gr1pt_u1->SetName("60_1_UVA9807c_1pt");  gr1pt_u1->SetTitle("60_1_UVA9807c_1pt");
  gr1pt_u1->SetMarkerStyle(21);  gr1pt_u1->SetMarkerColor(4); gr1pt_u1->SetFillColor(0);

  TGraphErrors *gr1pt_u1_st = new TGraphErrors();
  gr1pt_u1_st->SetName("60_1_UVA9807c_1pt");  gr1pt_u1_st->SetTitle("60_1_UVA9807c_1pt");
  gr1pt_u1_st->SetMarkerStyle(21);  gr1pt_u1_st->SetMarkerColor(4);
  gr1pt_u1_st->SetFillColor(0);

  Double_t u_sig_m[21];
  Double_t u_sig_s[21];
    
  cc=0;
  for(Int_t i=0;i<21;i++){
    Int_t np     = gr_u1[i]->GetN();
    Double_t *xp = gr_u1[i]->GetX();
    Double_t *yp = gr_u1[i]->GetY();

    u_sig_m[i] = TMath::Mean(np,yp);
    u_sig_s[i] = TMath::RMS(np,yp)/ sqrt(np);
    gr1pt_u1->SetPoint(i, db_set[i], u_sig_m[i]);
    gr1pt_u1->SetPointError(i, 0., u_sig_s[i]);

    gr1pt_u1_st->SetPoint(i, db_set[i], 100.*u_sig_s[i]/u_sig_m[i]);
    //cout<<i<<" "<<u_sig_m[i]<<" "<<u_sig_s[i]<<endl;
    //cout<<i<<endl;
    //cout<<u_sig_m[i]<<endl;
    //cout<<u_sig_s[i]<<endl;
    for(Int_t j=0;j<np;j++){
      if(j==0){ev_start=xp[0];}
      Double_t new_xx = i*300 + xp[j]-ev_start;
      grall_u1->SetPoint(cc, new_xx, yp[j]);
      //cout<<i<<" "<<j<<" "<<cc<<" "<<new_xx<<" , "<<yp[j]<<endl;
      //cout<<yp[j]<<endl;
      cc++;
    }
    delete xp; xp = NULL;
    delete yp; yp = NULL;
  }

  //-----------------------------------------------------------------------
  
  TGraphErrors *grall_l1 = new TGraphErrors();
  grall_l1->SetName("60_1_LANL213ch1c");  grall_l1->SetTitle("60_1_LANL213ch1c");
  grall_l1->SetMarkerStyle(21);  grall_l1->SetMarkerColor(2); grall_l1->SetFillColor(0);

  TGraphErrors *gr1pt_l1 = new TGraphErrors();
  gr1pt_l1->SetName("60_1_LANL213ch1c_1pt");  gr1pt_l1->SetTitle("60_1_LANL213ch1c_1pt");
  gr1pt_l1->SetMarkerStyle(21);  gr1pt_l1->SetMarkerColor(2); gr1pt_l1->SetFillColor(0);

  TGraphErrors *gr1pt_l1_st = new TGraphErrors();
  gr1pt_l1_st->SetName("60_1_LANL213ch1c_1pt");  gr1pt_l1_st->SetTitle("60_1_LANL213ch1c_1pt");
  gr1pt_l1_st->SetMarkerStyle(21);  gr1pt_l1_st->SetMarkerColor(2); gr1pt_l1_st->SetFillColor(0);

  Double_t l_sig_m[21];
  Double_t l_sig_s[21];

  cc=0;
  for(Int_t i=0;i<21;i++){
    Int_t np     = gr_l1[i]->GetN();
    Double_t *xp = gr_l1[i]->GetX();
    Double_t *yp = gr_l1[i]->GetY();

    Double_t scaling = 1.70408;
    //Double_t scaling = 1.;
    
    l_sig_m[i] = TMath::Mean(np,yp)/scaling;
    l_sig_s[i] = TMath::RMS(np,yp)/ sqrt(np);
    gr1pt_l1->SetPoint(i, db_set[i], l_sig_m[i]);
    gr1pt_l1->SetPointError(i, 0., l_sig_s[i]);

    gr1pt_l1_st->SetPoint(i, db_set[i], 100.*l_sig_s[i]/l_sig_m[i]);
    //cout<<i<<" "<<l_sig_m[i]<<" "<<l_sig_s[i]<<endl;
    for(Int_t j=0;j<np;j++){
      if(j==0){ev_start=xp[0];}
      Double_t new_xx = i*300 + xp[j]-ev_start;
      grall_l1->SetPoint(cc, new_xx, yp[j]/scaling);
      //cout<<i<<" "<<j<<" "<<cc<<" "<<new_xx<<" , "<<yp[j]<<endl;
      cc++;
    }
    delete xp; xp = NULL;
    delete yp; yp = NULL;
  }

  //----------------------------------------------------------------

  TGraphErrors *gr_theor = new TGraphErrors();
  gr_theor->SetName("60_1_theor");  gr_theor->SetTitle("60_1_theor");
  gr_theor->SetMarkerStyle(21);  gr_theor->SetMarkerColor(1); gr_theor->SetFillColor(0);

  Double_t v_in = 0.1;
  for(Int_t i=0;i<21;i++){
    Double_t theor_y = v_in/pow(10.,db_set[i]/20.);
    //gr_theor->SetPoint(i, db_set[i], theor_y*1.8785);
    gr_theor->SetPoint(i, db_set[i], theor_y);
  }
  //----------------------------------------------------------------
  
  TCanvas *c=new TCanvas("c","c",1600,1000);
  //c->Divide(2,2);
  //c->cd(1);
  grall_u1->Draw("AP");
  grall_l1->Draw("P");
  grall_u1->GetYaxis()->SetRangeUser(0.0001,0.4);

  c->SetGridy();  c->SetGridx();
  c->SetLogy();
  //c->BuildLegend(0.6,0.14,0.89,0.36);
  c->Update();
  c->Modified();

  
  TCanvas *c1=new TCanvas("c1","c1",1600,1000);
  gr1pt_u1->Draw("AP");
  gr1pt_l1->Draw("P");
  gr1pt_u1->GetYaxis()->SetRangeUser(0.0001,0.4);
  c1->SetGridy();  c1->SetGridx();
  c1->SetLogy();

  TF1 *temp_expo = new TF1("ft","expo",20.,58.);
  gr1pt_u1->SetLineColor(1);
  gr1pt_u1->SetLineWidth(1);

  gr1pt_u1->Fit(temp_expo,"QR");
  TF1 *u_expo = new TF1("u_ft","expo",0.,61.);
  u_expo->SetParameters(temp_expo->GetParameters());
  u_expo->SetLineColor(1);
  u_expo->SetLineWidth(1);
  u_expo->Draw("same");

  gr1pt_l1->Fit(temp_expo,"QR","sames");
  TF1 *l_expo = new TF1("l_ft","expo",0.,61.);
  l_expo->SetParameters(temp_expo->GetParameters());
  l_expo->SetLineColor(1);
  l_expo->SetLineWidth(1);
  l_expo->Draw("same");

  gr_theor->Draw("P");
  gr_theor->Fit(temp_expo,"QR","sames");
  TF1 *theor_expo = new TF1("theor_ft","expo",0.,61.);
  theor_expo->SetParameters(temp_expo->GetParameters());
  theor_expo->SetLineColor(1);
  theor_expo->SetLineWidth(1);
  theor_expo->Draw("same");


   
  c1->BuildLegend(0.61,0.68,0.90,0.90);

  TCanvas *c2=new TCanvas("c2","c2",1600,1000);
  c2->SetGridy();  c2->SetGridx();
  c2->cd();
  TGraphErrors *gr_ufit_dev = new TGraphErrors();
  gr_ufit_dev->SetName("60_1_devUVA");  gr_ufit_dev->SetTitle("60_1_devUVA");
  gr_ufit_dev->SetMarkerStyle(21);  gr_ufit_dev->SetMarkerColor(4);
  gr_ufit_dev->SetFillColor(0);

  TGraphErrors *gr_lfit_dev = new TGraphErrors();
  gr_lfit_dev->SetName("60_1_devLANL");  gr_lfit_dev->SetTitle("60_1_devLANL");
  gr_lfit_dev->SetMarkerStyle(21);  gr_lfit_dev->SetMarkerColor(2);
  gr_lfit_dev->SetFillColor(0);

  TGraphErrors *gr_ul_dif = new TGraphErrors();
  gr_ul_dif->SetName("60_1_ULdif");  gr_ul_dif->SetTitle("60_1_ULdif");
  gr_ul_dif->SetMarkerStyle(21);  gr_ul_dif->SetMarkerColor(1);
  gr_ul_dif->SetFillColor(0);

  for(Int_t i=0;i<21;i++){
    Double_t ufiteval = u_expo->Eval(db_set[i]);
    Double_t au_m = (u_sig_m[i]-ufiteval);
    Double_t bu_m = (u_sig_m[i]+ufiteval);
    Double_t uy = 200.*au_m/bu_m;
    Double_t abu_s = u_sig_s[i];
    Double_t uy_s  = fabs(uy)*sqrt(abu_s*abu_s/au_m/au_m + abu_s*abu_s/bu_m/bu_m);
    gr_ufit_dev->SetPoint(i, db_set[i], uy);
    gr_ufit_dev->SetPointError(i, 0., uy_s);
    
    Double_t lfiteval = l_expo->Eval(db_set[i]);
    Double_t al_m = (l_sig_m[i]-lfiteval);
    Double_t bl_m = (l_sig_m[i]+lfiteval);
    Double_t ly = 200.*al_m/bl_m;
    Double_t abl_s = l_sig_s[i];
    Double_t ly_s  = fabs(ly)*sqrt(abl_s*abl_s/al_m/al_m + abl_s*abl_s/bl_m/bl_m);
    gr_lfit_dev->SetPoint(i, db_set[i], ly);
    gr_ufit_dev->SetPointError(i, 0., ly_s);
    cout<<i<<" "<<ly<<" "<<ly_s<<endl;
    
    Double_t a_m = (l_sig_m[i]-u_sig_m[i]);
    Double_t b_m = (l_sig_m[i]+u_sig_m[i]);
    Double_t dy = 200.*a_m/b_m;
    Double_t ab_s = sqrt(l_sig_s[i]*l_sig_s[i] + u_sig_s[i]*u_sig_s[i]);
    Double_t dy_s = fabs(dy)*sqrt(ab_s*ab_s/a_m/a_m + ab_s*ab_s/b_m/b_m);
    gr_ul_dif->SetPoint(i, db_set[i], dy);
    gr_ul_dif->SetPointError(i, 0., dy_s);
    //cout<<i<<" "<<dy<<" "<<dy_s<<endl;
  }

  gr_ufit_dev->Draw("AP");
  gr_ufit_dev->GetYaxis()->SetRangeUser(-15.,10.);
  gr_lfit_dev->Draw("P");
  gr_ul_dif->Draw("P");
  //c2->SaveAs("pdf/lin_diff_dev.pdf");

  
  TCanvas *c3=new TCanvas("c3","c3",1600,1000);
  c3->SetGridy();  c3->SetGridx();
  gr1pt_u1_st->Draw("AP");
  gr1pt_u1_st->GetYaxis()->SetRangeUser(-1.,6.5);
  gr1pt_l1_st->Draw("P");
  //c3->SaveAs("pdf/lin_stat_err.pdf");
  
  theApp->Run();  

}

