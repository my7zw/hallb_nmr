#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <iomanip>

#include "TRint.h"
#include "TFile.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "THupgrade.hh"



using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  //TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);

  gStyle->SetLineScalePS(1);

  Double_t t_v1[11] = {0,1,2,3,4,5,6,7,8,9,10};
  Double_t t_v2[11],t_v3[11];
  /*Double_t ph1[11] = {-37.5, 32.2, 58.1, 83.6, 126.1, -142.3, -48.4, -6.5, 14.6, 29.01, 42.9};
  Double_t ph2[11] = {-45.2, 28.9, 54.6, 78.9, 117.5, -157.1, -55.7, -9.6, 12.4, 26.90, 40.4};
  Double_t ph3[11] = {-52.9, 25.7, 51.5, 74.6, 109.7, -171.6, -63.7,-12.9, 10.2, 25.90, 38.1};*/

  Double_t ph1[11] = {-48.8, 18.9, 38.5, 50.7, 61.6, 73.8, 90.7, 117.1, 157.2, -153.9,-110.3};
  Double_t ph2[11] = {-56.1, 16.1, 36.3, 48.5, 59.4, 71.3, 87.5, 112.7, 151.2, -160.1,-114.9};
  Double_t ph3[11] = {-66.5, 13.2, 34.2, 46.5, 57.2, 68.9, 84.5, 108.6, 145.7, -166.1,-119.7};
  
  Double_t ph4[11] = {91.4, 98.6, 105.7, 113.6, 123.2, 135.6, 152.8, 177.2, -148.7, -107.5, -68.1};
  Double_t ph5[11] = {91.3, 98.7, 105.5, 113.2, 122.8, 135.3, 152.1, 176.2, -149.8, -108.8, -69.2};
  Double_t ph6[11] = {91.1, 98.3, 105.2, 112.9, 122.5, 134.7, 151.5, 175.4, -150.9, -109.9, -70.4};
  
  Double_t ph1_cor[11];//32.7MHz
  Double_t ph2_cor[11];
  Double_t ph3_cor[11];

  Double_t ph1_df[11];
  Double_t ph2_df[11];

  Double_t ph4_cor[11];//213MHz
  Double_t ph5_cor[11];
  Double_t ph6_cor[11];

  Double_t ph4_df[11];
  Double_t ph5_df[11];

  for(Int_t i=0;i<11;i++){
    ph1_cor[i] = ph1[i];
    ph2_cor[i] = ph2[i];
    ph3_cor[i] = ph3[i];
    t_v2[i] = t_v1[i]+0.2;
    t_v3[i] = t_v1[i]+0.4;
    //if(i>4){
    if(i>8){
      ph1_cor[i] = ph1[i]+360;
      ph2_cor[i] = ph2[i]+360;
      ph3_cor[i] = ph3[i]+360;
    }
    ph1_df[i] = 1.-cos(TMath::Pi()*fabs(ph2_cor[i]-ph1_cor[i])/180.);
    ph2_df[i] = 1.-cos(TMath::Pi()*fabs(ph2_cor[i]-ph3_cor[i])/180.);

    ph4_cor[i] = ph4[i];
    ph5_cor[i] = ph5[i];
    ph6_cor[i] = ph6[i];
    if(i>7){
      ph4_cor[i] = ph4[i]+360;
      ph5_cor[i] = ph5[i]+360;
      ph6_cor[i] = ph6[i]+360;
    }
    ph4_df[i] = 1.-cos(TMath::Pi()*fabs(ph5_cor[i]-ph4_cor[i])/180.);
    ph5_df[i] = 1.-cos(TMath::Pi()*fabs(ph5_cor[i]-ph6_cor[i])/180.);
  }
  
  TGraphErrorsu *gph_323 = new TGraphErrorsu(11,t_v1,ph1,0,0);
  TGraphErrorsu *gph_327 = new TGraphErrorsu(11,t_v2,ph2,0,0);
  TGraphErrorsu *gph_331 = new TGraphErrorsu(11,t_v3,ph3,0,0);

  TGraphErrorsu *gph_323_c = new TGraphErrorsu(11,t_v1,ph1_cor,0,0);
  TGraphErrorsu *gph_327_c = new TGraphErrorsu(11,t_v2,ph2_cor,0,0);
  TGraphErrorsu *gph_331_c = new TGraphErrorsu(11,t_v3,ph3_cor,0,0);

  TGraphErrorsu *gph_323_df = new TGraphErrorsu(11,t_v1,ph1_df,0,0);
  TGraphErrorsu *gph_331_df = new TGraphErrorsu(11,t_v1,ph2_df,0,0);
  
  gph_323->SetDecorationAll(0, 4,1, 4,0.7,21);
  gph_327->SetDecorationAll(0, 2,1, 2,0.7,21);
  gph_331->SetDecorationAll(0, 418,1, 418,0.7,21);
  gph_323_c->SetDecorationAll(0, 1,1, 1,1.1,25);
  gph_327_c->SetDecorationAll(0, 1,1, 1,1.1,21);
  gph_331_c->SetDecorationAll(0, 1,1, 1,1.1,26);
  gph_323_df->SetDecorationAll(0, 4,1, 4,0.7,21);
  gph_331_df->SetDecorationAll(0, 418,1, 418,0.7,21);
  
  gph_323->SetTitle("Phase_org vs V tune at 32.3MHz");
  gph_327->SetTitle("Phase_org vs V tune at 32.7MHz");
  gph_331->SetTitle("Phase_org vs V tune at 33.1MHz");
  gph_323_c->SetTitle("Phase vs V tune at 32.3MHz");
  gph_327_c->SetTitle("Phase vs V tune at 32.7MHz");
  gph_331_c->SetTitle("Phase vs V tune at 33.1MHz");
  gph_323_df->SetTitle("qcurve_dev vs V tune at 32.3MHz");
  gph_331_df->SetTitle("qcurve_dev vs V tune at 33.1MHz");

  TGraphErrorsu *gph_2126 = new TGraphErrorsu(11,t_v1,ph4,0,0);
  TGraphErrorsu *gph_2130 = new TGraphErrorsu(11,t_v2,ph5,0,0);
  TGraphErrorsu *gph_2134 = new TGraphErrorsu(11,t_v3,ph6,0,0);

  TGraphErrorsu *gph_2126_c = new TGraphErrorsu(11,t_v1,ph4_cor,0,0);
  TGraphErrorsu *gph_2130_c = new TGraphErrorsu(11,t_v2,ph5_cor,0,0);
  TGraphErrorsu *gph_2134_c = new TGraphErrorsu(11,t_v3,ph6_cor,0,0);

  TGraphErrorsu *gph_2126_df = new TGraphErrorsu(11,t_v1,ph4_df,0,0);
  TGraphErrorsu *gph_2134_df = new TGraphErrorsu(11,t_v1,ph5_df,0,0);
  
  gph_2126->SetDecorationAll(0, 4,1, 4,0.7,21);
  gph_2130->SetDecorationAll(0, 2,1, 2,0.7,21);
  gph_2134->SetDecorationAll(0, 418,1, 418,0.7,21);
  gph_2126_c->SetDecorationAll(0, 1,1, 1,1.1,25);
  gph_2130_c->SetDecorationAll(0, 1,1, 1,1.1,21);
  gph_2134_c->SetDecorationAll(0, 1,1, 1,1.1,26);
  gph_2126_df->SetDecorationAll(0, 4,1, 4,0.7,21);
  gph_2134_df->SetDecorationAll(0, 418,1, 418,0.7,21);
  
  gph_2126->SetTitle("Phase_org vs V tune at 212.6MHz");
  gph_2130->SetTitle("Phase_org vs V tune at 213.0MHz");
  gph_2134->SetTitle("Phase_org vs V tune at 213.4MHz");
  gph_2126_c->SetTitle("Phase vs V tune at 212.6MHz");
  gph_2130_c->SetTitle("Phase vs V tune at 213.0MHz");
  gph_2134_c->SetTitle("Phase vs V tune at 213.4MHz");
  gph_2126_df->SetTitle("qcurve_dev vs V tune at 212.6MHz");
  gph_2134_df->SetTitle("qcurve_dev vs V tune at 213.4MHz");
  
  TCanvas *c=new TCanvas("c","c",1800,1000);
  c->Divide(2,2);
  c->cd(1);
  gph_323->Draw("AP");
  gph_327->Draw("P");
  gph_331->Draw("P");
  c->GetPad(1)->BuildLegend();
  c->GetPad(1)->SetTickx();
  c->GetPad(1)->SetTicky();

  c->cd(2);
  gph_323_c->Draw("AP");
  gph_327_c->Draw("P");
  gph_331_c->Draw("P");
  gph_323_c->GetXaxis()->SetLabelSize(0.055);
  gph_323_c->GetXaxis()->SetLimits(-1,11);
  gph_323_c->GetYaxis()->SetLabelSize(0.055);
  gph_323_c->GetYaxis()->SetRangeUser(-99,295);
  gph_323_c->GetYaxis()->SetNdivisions(410);

  c->GetPad(2)->BuildLegend(0.52, 0.14, 0.86, 0.31);
  c->GetPad(2)->SetTickx();
  c->GetPad(2)->SetTicky();

  c->cd(3);
  gph_323_df->Draw("AL");
  gph_331_df->Draw("L");
  c->GetPad(3)->BuildLegend();
  c->GetPad(3)->SetTickx();
  c->GetPad(3)->SetTicky();

  //c->SaveAs("pdf/phase_327.pdf");//this one is used

  TCanvas *cn=new TCanvas("cn","cn",200,10,700,500);
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  TPad *pad2 = new TPad("pad2","",0,0,1,1);
  pad2->SetFillStyle(4000); //will be transparent
  pad1->Draw();
  pad1->cd();
  gph_323_c->Draw("AP");
  gph_327_c->Draw("P");
  gph_331_c->Draw("P");
  pad1->Update();
  pad1->Modified();
  cn->cd();
  //compute the pad range with suitable margins
  Double_t ymin = 0;
  Double_t ymax = 0.016;
  Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
  Double_t xmin = 0;
  Double_t xmax = 11;
  Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
  pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
  pad2->Draw();
  pad2->cd();
  gph_323_df->Draw("L");
  gph_331_df->Draw("L");
  pad2->Update();
  TGaxis*axis = new TGaxis(xmax,ymin,xmax,ymax,ymin,ymax,510,"+L");
  //axis->SetLabelColor(kRed);
  axis->Draw();
  
  TCanvas *c1=new TCanvas("c1","c1",1800,1000);
  c1->Divide(2,2);
  c1->cd(1);
  gph_2126->Draw("AP");
  gph_2130->Draw("P");
  gph_2134->Draw("P");
  c1->GetPad(1)->BuildLegend(0.14, 0.15, 0.52, 0.37);
  c1->GetPad(1)->SetTickx();
  c1->GetPad(1)->SetTicky();

  c1->cd(2);
  gph_2126_c->Draw("AP");
  gph_2130_c->Draw("P");
  gph_2134_c->Draw("P");
  gph_2126_c->GetXaxis()->SetLabelSize(0.055);
  gph_2126_c->GetXaxis()->SetLimits(-1,11);
  gph_2126_c->GetYaxis()->SetLabelSize(0.055);
  gph_2126_c->GetYaxis()->SetRangeUser(60,320);
  gph_2126_c->GetYaxis()->SetNdivisions(410);

  c1->GetPad(2)->BuildLegend(0.52, 0.14, 0.86, 0.31);
  c1->GetPad(2)->SetTickx();
  c1->GetPad(2)->SetTicky();

  c1->cd(3);
  gph_2126_df->Draw("AL");
  gph_2134_df->Draw("L");

  c1->GetPad(3)->BuildLegend(0.13,0.64,0.51,0.85);
  c1->GetPad(3)->SetTickx();
  c1->GetPad(3)->SetTicky();

  //c1->SaveAs("pdf/phase_213.pdf"); this one is used
  //c->SetLogy();
  //c->SetGridy();
  //c->SetGridx();
  //
  //

  //grall_u[0]->GetXaxis()->SetRangeUser(1567187920, 1567192605);
  //grall_u[0]->GetYaxis()->SetRangeUser(-0.0002,0.0004);
  //c->Update(); c->Modified();

  theApp->Run();  

}

