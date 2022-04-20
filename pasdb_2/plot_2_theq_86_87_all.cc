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
#include <Math/MinimizerOptions.h>

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
  /*ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit","Minimize");
  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);
  ROOT::Math::MinimizerOptions::SetDefaultTolerance(.0001);
  ROOT::Math::MinimizerOptions::SetDefaultPrecision(1.e-08);
  ROOT::Math::MinimizerOptions::SetDefaultMaxIterations(100000);
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000000);*/


  T7301theq *te = new T7301theq("");
  //TString dir1 = "mdata/data_86_87/root_res_cnfg_calib_teen/";
  //TString dir1 = "mdata/data_86_87/root_res_cnfg_calib_tail/";
  //TString dir1 = "mdata/data_86_87/root_new_cnfg_calib_teen/";
  TString dir1 = "mdata/data_86_87/root_new_cnfg_calib_tail/";

  
  TString flist_fn = "./smth_plot_86_87_calib_tail.txt";
  std::ifstream fin(flist_fn);
  if(!fin.good()){cout<<"File read failed"<<endl;exit(111);}
  
  Int_t npnt=5;
  TFile *in_f[npnt];
  TGraphErrorsu *gr[npnt];
  TGraphErrorsu *gr_p[npnt];
  Int_t col[npnt]={7,7,2,2,2};

  TGraphErrorsu *gr_ute = new TGraphErrorsu();
  gr_ute->SetDecorationAll(0, 4,1, 4,0.7,21);
  gr_ute->SetName("ute");
  gr_ute->SetTitle("ute");
  TGraphErrorsu *gr_u_p = new TGraphErrorsu();
  gr_u_p->SetDecorationAll(0, 4,1, 4,0.7,21);
  gr_u_p->SetName("u_p");
  gr_u_p->SetTitle("u_p 4HeP vs time");
  TGraphErrorsu *gr_lte = new TGraphErrorsu();
  gr_lte->SetDecorationAll(0, 2,1, 2,0.7,21);
  gr_lte->SetName("lte");
  gr_lte->SetTitle("lte");
  TGraphErrorsu *gr_lte_theor = new TGraphErrorsu();
  gr_lte_theor->SetDecorationAll(0, 2,1, 2,0.7,21);
  gr_lte_theor->SetName("l_te");
  gr_lte_theor->SetTitle("l_te TE Pol. vs #");
  TGraphErrorsu *gr_l_cal = new TGraphErrorsu();
  gr_l_cal->SetDecorationAll(0, 2,1, 2,0.7,21);
  gr_l_cal->SetName("l_cal");
  gr_l_cal->SetTitle("l_cal Cal. vs #");

  Int_t cc1 = 0, cc2 = 0;  

  Double_t scaling = 3.20305;
  //Double_t scaling = 1;
  
  for(Int_t ipnt=0;ipnt<npnt;ipnt++){
    TString i_fn;
    i_fn.ReadLine(fin);  if(!fin.good()){break;}
    TString hn = i_fn;
    hn.Remove(hn.Last('_'));
    i_fn.Prepend(dir1.Data());
     
    in_f[ipnt] = new TFile(i_fn.Data(),"READ");
    gr[ipnt] = (TGraphErrorsu*)in_f[ipnt]->Get(hn);
    gr[ipnt]->SetDecorationAll(0, col[ipnt],1, col[ipnt],0.7,21);

    TString hnn = hn + "_4HeP_t";
    gr_p[ipnt] = (TGraphErrorsu*)in_f[ipnt]->Get(hnn);
    gr_p[ipnt]->SetDecorationAll(0, col[ipnt],1, col[ipnt],0.7,21);
    
    Int_t np      = gr[ipnt]->GetN();
    Double_t *xp  = gr[ipnt]->GetX();
    Double_t *exp = gr[ipnt]->GetEX();
    Double_t *yp  = gr[ipnt]->GetY();
    Double_t *eyp = gr[ipnt]->GetEY();

    if(ipnt<=1){
      Int_t np_p      = gr_p[ipnt]->GetN();
      Double_t *xp_p  = gr_p[ipnt]->GetX();
      Double_t *exp_p = gr_p[ipnt]->GetEX();
      Double_t *yp_p  = gr_p[ipnt]->GetY();
      Double_t *eyp_p = gr_p[ipnt]->GetEY();

      if(np_p!=np){cout<<"np miss"<<endl;exit(333);}
      for(Int_t i=0;i<np;i++){
	gr_ute->SetPoint(cc1,xp[i]-1567188020,yp[i]);//
	gr_ute->SetPointError(cc1,exp[i],eyp[i]);
	cout<<ipnt<<" cc1 "<<cc1<<endl;

	//Double_t t_cor = 1.;
	Double_t t_cor = 2.;
	yp_p[i] = yp_p[i]*t_cor;
	gr_u_p->SetPoint(cc1,xp_p[i]-1567188020,yp_p[i]);//
	if(eyp_p[i]==0){eyp_p[i] = 0.0002*yp_p[i];}
	gr_u_p->SetPointError(cc1,exp_p[i],eyp_p[i]);
	cc1++;
      }
    }
    if(ipnt>1){
      for(Int_t i=0;i<np;i++){
	gr_lte->SetPoint(cc2,xp[i]+139-1567188020,yp[i]/scaling);//
	gr_lte->SetPointError(cc2,exp[i],eyp[i]/scaling);
	cout<<ipnt<<" cc2 "<<cc2<<endl;
	cc2++;
      }
    }
    //delete xp; xp = NULL;
    //delete yp; yp = NULL;
    //delete exp; exp = NULL;
    //delete eyp; eyp = NULL;
  }
    
  TCanvas *c=new TCanvas("c","c",1600,1000);
  c->Divide(2,2);
  c->GetPad(1)->SetTickx();
  c->GetPad(1)->SetTicky();
  c->GetPad(2)->SetTickx();
  c->GetPad(2)->SetTicky();
  c->GetPad(3)->SetTickx();
  c->GetPad(3)->SetTicky();
  c->GetPad(4)->SetTickx();
  c->GetPad(4)->SetTicky();

  c->cd(1);
  gr_ute->Draw("AP");
  gr_ute->GetXaxis()->SetLimits(0,4000);
  gr_ute->GetYaxis()->SetRangeUser(0.04e-3, 0.18e-3);
  gr_lte->Draw("P");
    
  c->cd(2);
  gr_u_p->Draw("AP");
  gr_u_p->GetXaxis()->SetLimits(0,4000);
  //gr_u_p->GetYaxis()->SetRangeUser(7.02, 7.2);
  gr_u_p->GetYaxis()->SetRangeUser(5, 16);
  TF1 *pol2_p = new TF1("ft","pol2",1770,3500);
  gr_u_p->Fit(pol2_p,"R");

  Int_t np_ll      = gr_lte->GetN();
  Double_t *xp_ll  = gr_lte->GetX();
  Double_t *yp_ll  = gr_lte->GetY();
  Int_t cc_l = 0;
  for(Int_t j=0;j<np_ll;j++){
    Double_t He4P_l = pol2_p->Eval(xp_ll[j]);
    Double_t tep = te->CalculateTEP("proton", .5, 5.00, He4P_l);
    Double_t calib = tep/yp_ll[j];
    gr_lte_theor->SetPoint(cc_l,j+1,tep);
    gr_l_cal->SetPoint(cc_l,j+1,calib);
    cc_l++;
  }
  c->cd(3);
  gr_lte_theor->Draw("AP");
  //gr_lte_theor->GetYaxis()->SetRangeUser(3.082e-3, 3.083e-3);
  gr_lte_theor->GetYaxis()->SetRangeUser(2e-3, 3.8e-3);
 
  c->cd(4);
  gr_l_cal->Draw("AP");
  gr_l_cal->GetYaxis()->SetRangeUser(15, 40);
  TF1 *pol0_c = new TF1("ft","pol0",0.,15.);
  pol0_c->SetLineColor(1);  pol0_c->SetLineWidth(1);
  gr_l_cal->Fit(pol0_c,"R");
  Double_t calconst = pol0_c->GetParameter(0);
  TString lat = Form("cal.cnst.: %2.3f",calconst);
  prt_latex(0.12,0.85,lat.Data());

  
  //c->SetLogy();
  //c->SetGridy();
  //c->SetGridx();
  //c->BuildLegend();
  //c->SaveAs("pdf/theq_all_teen.pdf");
  //c->SaveAs("pdf/theq_all_tail.pdf");
  //c->SaveAs("pdf/theq_new_teen.pdf");
  //c->SaveAs("pdf/theq_new_tail.pdf");
  //c->SaveAs("pdf/theq_4HePnom_tail.pdf");
  //c->SaveAs("pdf/theq_4HePcor_tail.pdf");

  TGraphErrorsu *grall_TP_theor = new TGraphErrorsu();
  grall_TP_theor->SetDecorationAll(0, 4,1, 4,0.4,21);

  Int_t ccp=0;
  for(Int_t p=2;p<20;p++){
    Double_t theor_T = te->CalcT(p);
    grall_TP_theor->SetPoint(ccp, p, theor_T);
    ccp++;
  }
  TCanvas *c1=new TCanvas("c1","c1",1600,1000);
  c1->Divide(2,2);
  c1->GetPad(1)->SetTickx();
  c1->GetPad(1)->SetTicky();
  c1->cd(1);
  grall_TP_theor->Draw("AP");

  theApp->Run();  

}

