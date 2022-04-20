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
#include "TPaveStats.h"
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
  //TString dir = "mdata/data_60_1/root_res_cnfg_smasig/";
  TString dir = "mdata/data_60_1/root_res_cnfg_bigsig/";

  Double_t db_set[21] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48,
			 51, 54, 57, 60};

  /*Double_t db_err[21] = {0.02,
			 0.05,
			 0.11,
			 0.16,
			 0.35,
			 0.4,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45,
			 0.45};*/


  Double_t db_err[21] = {0.02,
			 0.05,
			 0.0948683298,
			 0.164924225,
			 0.1835755975,
			 0.1902629759,
			 0.4509988914,
			 0.4606517123,
			 0.4775981575,
			 0.4802082881,
			 0.486929153,
			 0.5052722039,
			 0.5116639522,
			 0.6446704585,
			 0.656886596,
			 0.663551053,
			 0.6754257916,
			 0.6772739475,
			 0.6820557162,
			 0.6884765791,
			 0.688621812};



  Double_t k_l[21] = {0.2856476366,
		      0.2210980967,
		      0.1649609773,
		      0.1200866373,
		      0.0848554369,
		      0.0597107797,
		      0.0429496901,
		      0.0302020125,
		      0.0213442765,
		      0.0149770574,
		      0.0105120503,
		      0.0073773912,
		      0.0051767489,
		      0.003642294,
		      0.002561117,
		      0.0018380581,
		      0.0012783974,
		      0.0008936492,
		      0.0006454562,
		      0.0004553557,
		      0.0003283427};

  Double_t k_ler[21] = {3.62E-05,
			3.75E-05,
			0.0001338489,
			2.09E-05,
			2.30E-05,
			1.83E-05,
			1.81E-05,
			2.05E-05,
			1.85E-05,
			1.83E-05,
			2.13E-05,
			1.38E-05,
			1.82E-05,
			1.65E-05,
			1.64E-05,
			1.83E-05,
			2.00E-05,
			1.55E-05,
			1.61E-05,
			1.75E-05,
			1.62E-05};

  Double_t k_u[21] = {0.1733092447,
		      0.1309955738,
		      0.0964735831,
		      0.0697188391,
		      0.0492399086,
		      0.0347034279,
		      0.024878726,
		      0.0174987974,
		      0.0123453592,
		      0.0086831967,
		      0.0061420076,
		      0.0043033381,
		      0.0030531965,
		      0.0021207714,
		      0.001505806,
		      0.0010786656,
		      0.0007367688,
		      0.0005151277,
		      0.0003819863,
		      0.0002682354,
		      0.0001888557};

  Double_t k_uer[21] = {0.0001063027,
			0.0001174899,
			4.33E-05,
			6.32E-05,
			4.52E-05,
			2.57E-05,
			3.17E-05,
			3.06E-05,
			3.51E-05,
			2.53E-05,
			2.99E-05,
			2.66E-05,
			2.57E-05,
			1.83E-05,
			2.58E-05,
			2.64E-05,
			2.50E-05,
			2.80E-05,
			2.34E-05,
			2.22E-05,
			2.65E-05};

  
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
  
  TGraphErrors *gr1pt_u1 = new TGraphErrors();
  gr1pt_u1->SetName("60_1_UVA9807c_1pt");  gr1pt_u1->SetTitle("60_1_UVA9807c_1pt");
  gr1pt_u1->SetMarkerStyle(21);  gr1pt_u1->SetMarkerColor(4); gr1pt_u1->SetFillColor(0);

  TGraphErrors *gr1pt_ku = new TGraphErrors();
  gr1pt_ku->SetName("60_1_UVA9807c_k");  gr1pt_ku->SetTitle("60_1_UVA9807c_k");
  gr1pt_ku->SetMarkerStyle(22);  gr1pt_ku->SetMarkerColor(4); gr1pt_ku->SetFillColor(0);

  TGraphErrors *gr1pt_ur = new TGraphErrors();
  gr1pt_ur->SetName("60_1_UVA9807c_ur");  gr1pt_ur->SetTitle("60_1_UVA9807c_ur");
  gr1pt_ur->SetMarkerStyle(21);  gr1pt_ur->SetMarkerColor(4); gr1pt_ur->SetFillColor(0);

  TGraphErrors *gr1pt_urer = new TGraphErrors();
  gr1pt_urer->SetName("60_1_UVA9807c_urer");  gr1pt_urer->SetTitle("60_1_UVA9807c_urer");
  gr1pt_urer->SetMarkerStyle(21);  gr1pt_urer->SetMarkerColor(4); gr1pt_urer->SetFillColor(0);

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
    //gr1pt_u1->SetPointError(i, 0., u_sig_s[i]);
    //gr1pt_u1->SetPointError(i, db_err[i], u_sig_s[i]);
    gr1pt_u1->SetPointError(i, db_err[i]/6., u_sig_s[i]);

    gr1pt_ku->SetPoint(i, db_set[i], k_u[i]);
    //gr1pt_ku->SetPointError(i, 0., k_uer[i]);
    //gr1pt_ku->SetPointError(i, db_err[i], k_uer[i]);
    gr1pt_ku->SetPointError(i, db_err[i]/6., k_uer[i]);

    gr1pt_ur->SetPoint(i, db_set[i], k_u[i]/u_sig_m[i]);
    gr1pt_urer->SetPoint(i, db_set[i], k_uer[i]/u_sig_s[i]);

    cout<<np<<" "<<i<<" "<<u_sig_s[i]<<" "<<k_uer[i]<<" "<<k_uer[i]/u_sig_s[i]<<endl;
    
    delete xp; xp = NULL;
    delete yp; yp = NULL;
  }

  //-----------------------------------------------------------------------
  
  TGraphErrors *gr1pt_l1 = new TGraphErrors();
  gr1pt_l1->SetName("60_1_LANL213ch1c_1pt");  gr1pt_l1->SetTitle("60_1_LANL213ch1c_1pt");
  gr1pt_l1->SetMarkerStyle(21);  gr1pt_l1->SetMarkerColor(2); gr1pt_l1->SetFillColor(0);

  TGraphErrors *gr1pt_kl = new TGraphErrors();
  gr1pt_kl->SetName("60_1_LANL213ch1c_k");  gr1pt_kl->SetTitle("60_1_LANL213ch1c_k");
  gr1pt_kl->SetMarkerStyle(22);  gr1pt_kl->SetMarkerColor(2); gr1pt_kl->SetFillColor(0);

  TGraphErrors *gr1pt_lr = new TGraphErrors();
  gr1pt_lr->SetName("60_1_LANL_lr");  gr1pt_lr->SetTitle("60_1_LANL_lr");
  gr1pt_lr->SetMarkerStyle(21);  gr1pt_lr->SetMarkerColor(2); gr1pt_lr->SetFillColor(0);

  TGraphErrors *gr1pt_lrer = new TGraphErrors();
  gr1pt_lrer->SetName("60_1_LANL_lrer");  gr1pt_lrer->SetTitle("60_1_LANL_lrer");
  gr1pt_lrer->SetMarkerStyle(21);  gr1pt_lrer->SetMarkerColor(2); gr1pt_lrer->SetFillColor(0);

  Double_t l_sig_m[21];
  Double_t l_sig_s[21];

  cc=0;
  for(Int_t i=0;i<21;i++){
    Int_t np     = gr_l1[i]->GetN();
    Double_t *xp = gr_l1[i]->GetX();
    Double_t *yp = gr_l1[i]->GetY();

    //Double_t scaling = 1.70408;
    Double_t scaling = 1.;
    
    l_sig_m[i] = TMath::Mean(np,yp)/scaling;
    l_sig_s[i] = TMath::RMS(np,yp)/sqrt(np);
    gr1pt_l1->SetPoint(i, db_set[i], l_sig_m[i]);
    //gr1pt_l1->SetPointError(i, 0., l_sig_s[i]);
    //gr1pt_l1->SetPointError(i, db_err[i], l_sig_s[i]);
    gr1pt_l1->SetPointError(i, db_err[i]/6., l_sig_s[i]);

    gr1pt_kl->SetPoint(i, db_set[i], k_l[i]);
    //gr1pt_kl->SetPointError(i, 0., k_ler[i]);
    //gr1pt_kl->SetPointError(i, db_err[i], k_ler[i]);
    gr1pt_kl->SetPointError(i, db_err[i]/6., k_ler[i]);

    gr1pt_lr->SetPoint(i, db_set[i], k_l[i]/l_sig_m[i]);
    gr1pt_lrer->SetPoint(i, db_set[i], k_ler[i]/l_sig_s[i]);

    cout<<np<<" "<<i<<" "<<l_sig_s[i]<<" "<<k_ler[i]<<" "<<k_ler[i]/l_sig_s[i]<<endl;
    delete xp; xp = NULL;
    delete yp; yp = NULL;
  }

  //----------------------------------------------------------------
  
  
  TCanvas *c1=new TCanvas("c1","c1",1600,1000);
  gr1pt_u1->Draw("AP");
  gr1pt_l1->Draw("P");
  gr1pt_ku->Draw("P");
  gr1pt_kl->Draw("P");

  gr1pt_u1->GetYaxis()->SetRangeUser(0.0001,0.4);
  //c1->SetGridy();
  //c1->SetGridx();
  c1->SetTicky();
  c1->SetTickx();
  c1->SetLogy();

  c1->BuildLegend(0.1,0.25,0.4,0.4);
  Double_t fx1 = 10., fx2 = 61.;
  TF1 *exp_u1 = new TF1("ft","expo",fx1,fx2);
  TF1 *exp_l1 = new TF1("ft","expo",fx1,fx2);
  TF1 *exp_ku = new TF1("ft","expo",fx1,fx2);
  TF1 *exp_kl = new TF1("ft","expo",fx1,fx2);
  gr1pt_u1->Fit(exp_u1,"R");
  gr1pt_l1->Fit(exp_l1,"R");
  gr1pt_ku->Fit(exp_ku,"R");
  gr1pt_kl->Fit(exp_kl,"R");

  TF1 *u1_expo = new TF1("u1_ft","expo",0.,61.);
  u1_expo->SetParameters(exp_u1->GetParameters());
  u1_expo->SetLineColor(1); u1_expo->SetLineWidth(1);
  u1_expo->Draw("same");

  TF1 *l1_expo = new TF1("l1_ft","expo",0.,61.);
  l1_expo->SetParameters(exp_l1->GetParameters());
  l1_expo->SetLineColor(1); l1_expo->SetLineWidth(1);
  l1_expo->Draw("same");

  TF1 *ku_expo = new TF1("ku_ft","expo",0.,61.);
  ku_expo->SetParameters(exp_ku->GetParameters());
  ku_expo->SetLineColor(1); ku_expo->SetLineWidth(1);
  ku_expo->Draw("same");

  TF1 *kl_expo = new TF1("kl_ft","expo",0.,61.);
  kl_expo->SetParameters(exp_kl->GetParameters());
  kl_expo->SetLineColor(1); kl_expo->SetLineWidth(1);
  kl_expo->Draw("same");

  c1->Update();
  TPaveStats *pu1 = (TPaveStats*)gr1pt_u1->GetListOfFunctions()->FindObject("stats");
  pu1->SetName("u1");
  pu1->SetX1NDC(.5);  pu1->SetY1NDC(.767);  pu1->SetX2NDC(.700);  pu1->SetY2NDC(.900);
  
  TPaveStats *pl1 = (TPaveStats*)gr1pt_l1->GetListOfFunctions()->FindObject("stats");
  pl1->SetName("l1");
  pl1->SetX1NDC(.7);  pl1->SetY1NDC(.767);  pl1->SetX2NDC(.900);  pl1->SetY2NDC(.900);

  TPaveStats *pku = (TPaveStats*)gr1pt_ku->GetListOfFunctions()->FindObject("stats");
  pku->SetName("ku");
  pku->SetX1NDC(.5);  pku->SetY1NDC(.634);  pku->SetX2NDC(.700);  pku->SetY2NDC(.767);

  TPaveStats *pkl = (TPaveStats*)gr1pt_kl->GetListOfFunctions()->FindObject("stats");
  pkl->SetName("kl");
  pkl->SetX1NDC(.7);  pkl->SetY1NDC(.634);  pkl->SetX2NDC(.900);  pkl->SetY2NDC(.767);

  c1->Modified();  
  c1->Update();  

  //c1->SaveAs("pdf/xer_fit_sma_dber1.pdf");
  //c1->SaveAs("pdf/xer_fit_sma_dber2.pdf");
  c1->SaveAs("pdf/lin_xer_fit_big.pdf");
  //c1->SaveAs("pdf/xer_fit_sma_nosqrt.pdf");
  //c1->SaveAs("pdf/xer_fit_big_nosqrt.pdf");

  
  TGraph *g_chi2_u1 = new TGraph();
  TGraph *g_chi2_l1 = new TGraph();
  TGraph *g_chi2_ku = new TGraph();
  TGraph *g_chi2_kl = new TGraph();

  g_chi2_u1->SetMarkerStyle(21);  g_chi2_u1->SetMarkerColor(4);
  g_chi2_l1->SetMarkerStyle(21);  g_chi2_l1->SetMarkerColor(2);
  g_chi2_ku->SetMarkerStyle(22);  g_chi2_ku->SetMarkerColor(4);
  g_chi2_kl->SetMarkerStyle(22);  g_chi2_kl->SetMarkerColor(2);


  TF1 *exp_u1_t;
  TF1 *exp_l1_t;
  TF1 *exp_ku_t;
  TF1 *exp_kl_t;

  for(Int_t i=0;i<18;i++){
    Double_t db_last = 61.;
    
    TF1 *exp_u1 = new TF1("ft","expo",db_set[i],db_last);
    TF1 *exp_l1 = new TF1("ft","expo",db_set[i],db_last);
    TF1 *exp_ku = new TF1("ft","expo",db_set[i],db_last);
    TF1 *exp_kl = new TF1("ft","expo",db_set[i],db_last);

    gr1pt_u1->Fit(exp_u1,"QNR");
    gr1pt_l1->Fit(exp_l1,"QNR");
    gr1pt_ku->Fit(exp_ku,"QNR");
    gr1pt_kl->Fit(exp_kl,"QNR");

    Double_t chi_u1 = exp_u1->GetChisquare()/exp_u1->GetNDF();
    Double_t chi_l1 = exp_l1->GetChisquare()/exp_l1->GetNDF();
    Double_t chi_ku = exp_ku->GetChisquare()/exp_ku->GetNDF();
    Double_t chi_kl = exp_kl->GetChisquare()/exp_kl->GetNDF();

    g_chi2_u1->SetPoint(i,db_set[i],chi_u1);
    g_chi2_l1->SetPoint(i,db_set[i],chi_l1);
    g_chi2_ku->SetPoint(i,db_set[i],chi_ku);
    g_chi2_kl->SetPoint(i,db_set[i],chi_kl);

    if(db_set[i]==6){
      exp_u1_t = exp_u1;
      exp_l1_t = exp_l1;
      exp_ku_t = exp_ku;
      exp_kl_t = exp_kl;
    }

  }

  TCanvas *c2=new TCanvas("c2","c2",1600,1000);
  g_chi2_u1->Draw("AP");
  g_chi2_l1->Draw("P");
  g_chi2_ku->Draw("P");
  g_chi2_kl->Draw("P");

  //g_chi2_u1->GetYaxis()->SetRangeUser(0.0001,0.4);
  c2->SetGridy();  c2->SetGridx();
  //c2->SetLogy();
  //c2->BuildLegend(0.1,0.25,0.4,0.4);
  //c2->SaveAs("pdf/xer_chi_sma.pdf");
  //c2->SaveAs("pdf/xer_chi_sma_dber1.pdf");
  //c2->SaveAs("pdf/xer_chi_big.pdf");
  //c2->SaveAs("pdf/xer_chi_big_dber1.pdf");


  TGraph *g_fitdev_u1 = new TGraph();
  TGraph *g_fitdev_l1 = new TGraph();
  TGraph *g_fitdev_ku = new TGraph();
  TGraph *g_fitdev_kl = new TGraph();

  g_fitdev_u1->SetMarkerStyle(21);  g_fitdev_u1->SetMarkerColor(4);
  g_fitdev_l1->SetMarkerStyle(21);  g_fitdev_l1->SetMarkerColor(2);
  g_fitdev_ku->SetMarkerStyle(22);  g_fitdev_ku->SetMarkerColor(4);
  g_fitdev_kl->SetMarkerStyle(22);  g_fitdev_kl->SetMarkerColor(2);

  for(Int_t i=0;i<21;i++){
    Double_t fiteval_u1 = exp_u1_t->Eval(db_set[i]);
    Double_t fiteval_l1 = exp_l1_t->Eval(db_set[i]);
    Double_t fiteval_ku = exp_ku_t->Eval(db_set[i]);
    Double_t fiteval_kl = exp_kl_t->Eval(db_set[i]);
    
    Double_t y_u1 = 200.*(u_sig_m[i]-fiteval_u1)/(u_sig_m[i]+fiteval_u1);
    Double_t y_l1 = 200.*(l_sig_m[i]-fiteval_l1)/(l_sig_m[i]+fiteval_l1);
    Double_t y_ku = 200.*(k_u[i]    -fiteval_ku)/(k_u[i]    +fiteval_ku);
    Double_t y_kl = 200.*(k_l[i]    -fiteval_kl)/(k_l[i]    +fiteval_kl);
    g_fitdev_u1->SetPoint(i, db_set[i], y_u1);
    g_fitdev_l1->SetPoint(i, db_set[i], y_l1);
    g_fitdev_ku->SetPoint(i, db_set[i], y_ku);
    g_fitdev_kl->SetPoint(i, db_set[i], y_kl);
  }
  
  TCanvas *c3=new TCanvas("c3","c3",1600,1000);
  g_fitdev_u1->Draw("AP");
  g_fitdev_l1->Draw("P");
  g_fitdev_ku->Draw("P");
  g_fitdev_kl->Draw("P");

  g_fitdev_u1->GetYaxis()->SetRangeUser(-20,10);
  c3->SetGridy();  c3->SetGridx();
  ////c3->SaveAs("pdf/lin_diff_dev_xer.pdf");
  //c3->SaveAs("pdf/xer_fitdev_big_dber1_db6.pdf");
  theApp->Run();  

}

