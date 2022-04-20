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

  gStyle->SetLineScalePS(1);
  gStyle->SetOptFit(1111);
  TString suf = "";
  TString dir = "mdata/data_86_qcv/root_new_cnfg_teen_0/";
  TString flist_fn = "./smth_plot_86_qcv.txt";
  std::ifstream fin(flist_fn);
  if(!fin.good()){cout<<"File read failed"<<endl;exit(111);}
  

  Int_t nset_u=1, npnt_u=10;
  TFile *in_f_u[nset_u][npnt_u];

  TGraph *gr_u[nset_u][npnt_u];
  TGraph *gr_dcoff[nset_u][npnt_u];
  TGraph *gr_dcoff_T[nset_u][npnt_u];
  TGraph *gr_moff[nset_u][npnt_u];
  TGraph *gr_qamp[nset_u][npnt_u];
  TGraph *gr_qamp_T[nset_u][npnt_u];
  TGraph *gr_skew[nset_u][npnt_u];
  TGraph *gr_skew_T[nset_u][npnt_u]; 
  TGraph *gr_msht[nset_u][npnt_u];
  TGraph *gr_msht1[nset_u][npnt_u];
  TGraph *gr_msht1_T[nset_u][npnt_u];
  TGraph *gr_cubc[nset_u][npnt_u];

  for(Int_t iset=0;iset<nset_u;iset++){
    for(Int_t ipnt=0;ipnt<npnt_u;ipnt++){
      TString i_fn;
      i_fn.ReadLine(fin);  if(!fin.good()){break;}
      TString hn = i_fn;
      hn.Remove(hn.Last('_'));
      i_fn.Prepend(dir.Data());
     
      in_f_u[iset][ipnt] = new TFile(i_fn.Data(),"READ");
      gr_u[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hn);

      TString hnn = hn + "_dcoff";
      gr_dcoff[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_dcoff_T";
      //gr_dcoff_T[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_moff";
      gr_moff[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_qamp";
      gr_qamp[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_qamp_T";
      //gr_qamp_T[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_skew";
      gr_skew[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_skew_T";
      //gr_skew_T[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_msht";
      gr_msht[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_msht1";
      gr_msht1[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_msht1_T";
      //gr_msht1_T[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);
      hnn = hn + "_cubc";
      gr_cubc[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hnn);

      cout<<i_fn<<endl;
    }
  }


  //-------------------------------------------------------------------
		
  Double_t ev_start = 0, cc=0;

  Int_t gmst_u[nset_u] = {21};
  TString gn_u[nset_u] = {"uva"};
		
  ev_start = 0;
  TGraphErrorsu *grall_u[nset_u];
  TGraphErrorsu *grall_dcoff[nset_u];
  //TGraphErrorsu *grall_dcoff_T[nset_u];
  TGraphErrorsu *grall_moff[nset_u];
  TGraphErrorsu *grall_qamp[nset_u];
  //TGraphErrorsu *grall_qamp_T[nset_u]; 
  TGraphErrorsu *grall_skew[nset_u];
  //TGraphErrorsu *grall_skew_T[nset_u]; 
  TGraphErrorsu *grall_msht[nset_u];
  TGraphErrorsu *grall_msht1[nset_u];
  //TGraphErrorsu *grall_msht1_T[nset_u];
  TGraphErrorsu *grall_cubc[nset_u];
  
  for(Int_t iset=0;iset<nset_u;iset++){
    cout<<"iset "<<iset<<endl;
    grall_u[iset] = new TGraphErrorsu();
    grall_u[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    grall_u[iset]->SetName("86_0_"+gn_u[iset]);
    grall_u[iset]->SetTitle("86_0_"+gn_u[iset]);
    
    grall_dcoff[iset] = new TGraphErrorsu();
    //grall_dcoff_T[iset] = new TGraphErrorsu();
    grall_moff[iset] = new TGraphErrorsu();
    grall_qamp[iset] = new TGraphErrorsu();
    //grall_qamp_T[iset] = new TGraphErrorsu();
    grall_skew[iset] = new TGraphErrorsu();
    //grall_skew_T[iset] = new TGraphErrorsu();
    grall_msht[iset] = new TGraphErrorsu();
    grall_msht1[iset] = new TGraphErrorsu();
    //grall_msht1_T[iset] = new TGraphErrorsu();
    grall_cubc[iset] = new TGraphErrorsu();
    
    grall_dcoff[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    //grall_dcoff_T[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    grall_moff[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    grall_qamp[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    //grall_qamp_T[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    grall_skew[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    //grall_skew_T[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]); 
    grall_msht[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    grall_msht1[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    //grall_msht1_T[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    grall_cubc[iset]->SetDecorationAll(0, gmst_u[iset],1, 2,0.7,gmst_u[iset]);
    
    grall_dcoff[iset]->SetName("86dcoff_"+gn_u[iset]);
    //grall_dcoff_T[iset]->SetName("86dcoff_T_"+gn_u[iset]);
    grall_moff[iset]->SetName("86moff_"+gn_u[iset]);
    grall_qamp[iset]->SetName("86qamp_"+gn_u[iset]);
    //grall_qamp_T[iset]->SetName("86qamp_T_"+gn_u[iset]);
    grall_skew[iset]->SetName("86skew_"+gn_u[iset]);
    //grall_skew_T[iset]->SetName("86skewT _"+gn_u[iset]);
    grall_msht[iset]->SetName("86msht_"+gn_u[iset]);
    grall_msht1[iset]->SetName("86msht1_"+gn_u[iset]);
    //grall_msht1_T[iset]->SetName("86msht1_T_"+gn_u[iset]);
    grall_cubc[iset]->SetName("86cubc_"+gn_u[iset]);
    
    grall_dcoff[iset]->SetTitle("86dcoff_"+gn_u[iset]);
    //grall_dcoff_T[iset]->SetTitle("86dcoff_T_"+gn_u[iset]);
    grall_moff[iset]->SetTitle("86moff_"+gn_u[iset]);
    grall_qamp[iset]->SetTitle("86qamp_"+gn_u[iset]);
    //grall_qamp_T[iset]->SetTitle("86qamp_T_"+gn_u[iset]);
    grall_skew[iset]->SetTitle("86skew_"+gn_u[iset]);
    //grall_skew_T[iset]->SetTitle("86skewT _"+gn_u[iset]);
    grall_msht[iset]->SetTitle("86msht_"+gn_u[iset]);
    grall_msht1[iset]->SetTitle("86msht1_"+gn_u[iset]);
    //grall_msht1_T[iset]->SetTitle("86msht1_T_"+gn_u[iset]);
    grall_cubc[iset]->SetTitle("86cubc_"+gn_u[iset]);
    
    cc=0;
    for(Int_t ipnt=0;ipnt<npnt_u;ipnt++){
      Int_t np     = gr_u[iset][ipnt]->GetN();
      Double_t *xp = gr_u[iset][ipnt]->GetX();
      Double_t *yp = gr_u[iset][ipnt]->GetY();
      
      Int_t np_dcoff     = gr_dcoff[iset][ipnt]->GetN();
      Double_t *xp_dcoff = gr_dcoff[iset][ipnt]->GetX();
      Double_t *yp_dcoff = gr_dcoff[iset][ipnt]->GetY();
      /*Int_t np_dcoff_T     = gr_dcoff_T[iset][ipnt]->GetN();
      Double_t *xp_dcoff_T = gr_dcoff_T[iset][ipnt]->GetX();
      Double_t *yp_dcoff_T = gr_dcoff_T[iset][ipnt]->GetY();*/

      Int_t np_moff     = gr_moff[iset][ipnt]->GetN();
      Double_t *xp_moff = gr_moff[iset][ipnt]->GetX();
      Double_t *yp_moff = gr_moff[iset][ipnt]->GetY();

      Int_t np_qamp     = gr_qamp[iset][ipnt]->GetN();
      Double_t *xp_qamp = gr_qamp[iset][ipnt]->GetX();
      Double_t *yp_qamp = gr_qamp[iset][ipnt]->GetY();
      /*Int_t np_qamp_T     = gr_qamp_T[iset][ipnt]->GetN();
      Double_t *xp_qamp_T = gr_qamp_T[iset][ipnt]->GetX();
      Double_t *yp_qamp_T = gr_qamp_T[iset][ipnt]->GetY();*/

      Int_t np_skew     = gr_skew[iset][ipnt]->GetN();
      Double_t *xp_skew = gr_skew[iset][ipnt]->GetX();
      Double_t *yp_skew = gr_skew[iset][ipnt]->GetY();

      /*Int_t np_skew_T     = gr_skew_T[iset][ipnt]->GetN();
      Double_t *xp_skew_T = gr_skew_T[iset][ipnt]->GetX();
      Double_t *yp_skew_T = gr_skew_T[iset][ipnt]->GetY();*/

      Int_t np_msht     = gr_msht[iset][ipnt]->GetN();
      Double_t *xp_msht = gr_msht[iset][ipnt]->GetX();
      Double_t *yp_msht = gr_msht[iset][ipnt]->GetY();

      Int_t np_msht1     = gr_msht1[iset][ipnt]->GetN();
      Double_t *xp_msht1 = gr_msht1[iset][ipnt]->GetX();
      Double_t *yp_msht1 = gr_msht1[iset][ipnt]->GetY();
      /*Int_t np_msht1_T     = gr_msht1_T[iset][ipnt]->GetN();
      Double_t *xp_msht1_T = gr_msht1_T[iset][ipnt]->GetX();
      Double_t *yp_msht1_T = gr_msht1_T[iset][ipnt]->GetY();*/

      Int_t np_cubc     = gr_cubc[iset][ipnt]->GetN();
      Double_t *xp_cubc = gr_cubc[iset][ipnt]->GetX();
      Double_t *yp_cubc = gr_cubc[iset][ipnt]->GetY();

      for(Int_t j=0;j<np;j++){
	//cout<<iset<<" "<<ipnt<<" "<<j<<" "<<cc<<endl;
	cout<<ipnt<<" "<<std::fixed<<std::setprecision(0)
	    <<std::setw(20)<<xp[j]<<" "<<xp_dcoff[j]<<endl;  

	
	//grall_u[iset]->SetPoint(cc, new_xx, yp[j]);
	grall_u[iset]->SetPoint(cc, xp[j]+139, yp[j]);
	grall_dcoff[iset]->SetPoint(cc, xp_dcoff[j]+139, yp_dcoff[j]);
	//grall_dcoff_T[iset]->SetPoint(cc, xp_dcoff_T[j], yp_dcoff_T[j]);
	grall_moff[iset]->SetPoint(cc, xp_moff[j]+139, yp_moff[j]);
	grall_qamp[iset]->SetPoint(cc, xp_qamp[j]+139, yp_qamp[j]);
	//grall_qamp_T[iset]->SetPoint(cc, xp_qamp_T[j], yp_qamp_T[j]);
	grall_skew[iset]->SetPoint(cc, xp_skew[j]+139, yp_skew[j]);
	//grall_skew_T[iset]->SetPoint(cc, xp_skew_T[j], yp_skew_T[j]);
	grall_msht[iset]->SetPoint(cc, xp_msht[j]+139, yp_msht[j]);
	grall_msht1[iset]->SetPoint(cc, xp_msht1[j]+139, yp_msht1[j]);
	//grall_msht1_T[iset]->SetPoint(cc, xp_msht1_T[j], yp_msht1_T[j]);
	grall_cubc[iset]->SetPoint(cc, xp_cubc[j]+139, yp_cubc[j]);

	//grall_u[iset]->SetPoint(cc, new_xx, yp[j]*fit_u[0]/fit_u[iset]);
	cc++;
      }
      delete xp; xp = NULL;
      delete yp; yp = NULL;
      delete xp_dcoff; xp_dcoff = NULL;
      delete yp_dcoff; yp_dcoff = NULL;
      //delete xp_dcoff_T; xp_dcoff_T = NULL;
      //delete yp_dcoff_T; yp_dcoff_T = NULL;
      delete xp_moff; xp_moff = NULL;
      delete yp_moff; yp_moff = NULL;
      delete xp_qamp; xp_qamp = NULL;
      delete yp_qamp; yp_qamp = NULL;
      //delete xp_qamp_T; xp_qamp_T = NULL;
      //delete yp_qamp_T; yp_qamp_T = NULL;
      delete xp_skew; xp_skew = NULL;
      delete yp_skew; yp_skew = NULL;
      //delete xp_skew_T; xp_skew_T = NULL;
      //delete yp_skew_T; yp_skew_T = NULL;
      delete xp_msht; xp_msht = NULL;
      delete yp_msht; yp_msht = NULL;
      delete xp_msht1; xp_msht1 = NULL;
      delete yp_msht1; yp_msht1 = NULL;
      //delete xp_msht1_T; xp_msht1_T = NULL;
      //delete yp_msht1_T; yp_msht1_T = NULL;
      delete xp_cubc; xp_cubc = NULL;
      delete yp_cubc; yp_cubc = NULL;

    }
  }

  TCanvas *c=new TCanvas("c","c",1600,1000);
  //c->Divide(2,2);
  c->cd();

  
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_u[iset]->Draw("AP");
      //grall_u[iset]->GetYaxis()->SetRangeUser(-0.0015,0.0015);
      grall_u[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_u[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_u[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_u[iset]->Draw("P");}
  }
  //c->SetLogy();
  c->SetGridy();
  c->SetGridx();
  //c->BuildLegend();
  //c->SaveAs("mdata/qc_area"+suf+".pdf");

  TCanvas *c1=new TCanvas("c1","c1",1600,1000);
  c1->Divide(1,3);
  /*c1->cd(1);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_dcoff[iset]->Draw("AP");
      //grall_dcoff[iset]->GetYaxis()->SetRangeUser(1.86,1.92);
      grall_dcoff[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_dcoff[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_dcoff[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_dcoff[iset]->Draw("P");}
    }*/
  c1->cd(2);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_moff[iset]->Draw("AP");
      //grall_moff[iset]->GetYaxis()->SetRangeUser(-0.054,-0.044);
      grall_moff[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_moff[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_moff[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_moff[iset]->Draw("P");}
  }
  
  c1->cd(3);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_qamp[iset]->Draw("AP");
      //grall_qamp[iset]->GetYaxis()->SetRangeUser(0.08,0.11);
      grall_qamp[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_qamp[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_qamp[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_qamp[iset]->Draw("P");}
  }
  c1->GetPad(1)->SetGridy();
  c1->GetPad(1)->SetGridx();
  c1->GetPad(2)->SetGridy();
  c1->GetPad(2)->SetGridx();
  c1->GetPad(3)->SetGridy();
  c1->GetPad(3)->SetGridx();
  //c1->SaveAs("mdata/qc_off_amp"+suf+".pdf"); 
    
  TCanvas *c2=new TCanvas("c2","c2",1600,1000);
  c2->Divide(1,2);
  c2->cd(1);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_skew[iset]->Draw("AP");
      //grall_skew[iset]->GetYaxis()->SetRangeUser(-90.,50.);
      grall_skew[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_skew[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_skew[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_skew[iset]->Draw("P");}
  }
  /*c2->cd(2);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_skew_T[iset]->Draw("AP");
      grall_skew_T[iset]->GetYaxis()->SetRangeUser(-90.,50.);
    }
    else{grall_skew_T[iset]->Draw("P");}
    }*/
  c2->GetPad(1)->SetGridy();
  c2->GetPad(1)->SetGridx();
  c2->GetPad(2)->SetGridy();
  c2->GetPad(2)->SetGridx();
  //c2->SaveAs("mdata/qc_skew"+suf+".pdf"); 

  TCanvas *c3=new TCanvas("c3","c3",1600,1000);
  c3->Divide(2,2);
  c3->cd(1);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_msht[iset]->Draw("AP");
      //grall_msht[iset]->GetYaxis()->SetRangeUser(212.7,213.3);
      grall_msht[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_msht[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_msht[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_msht[iset]->Draw("P");}
  }
  c3->cd(2);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_msht1[iset]->Draw("AP");
      //grall_msht1[iset]->GetYaxis()->SetRangeUser(212.98,213.02);
      grall_msht1[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_msht1[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_msht1[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_msht1[iset]->Draw("P");}
  }
  /*c3->cd(3);
  TF1 *msht1T_p1 = new TF1("ft","pol1");
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_msht1_T[iset]->Draw("AP");
      grall_msht1_T[iset]->GetYaxis()->SetRangeUser(212.98,213.02);
      grall_msht1_T[iset]->Fit(msht1T_p1);
    }
    else{grall_msht1_T[iset]->Draw("P");}
    }*/
  c3->cd(4);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_cubc[iset]->Draw("AP");
      //grall_cubc[iset]->GetYaxis()->SetRangeUser(0.08,0.12);
      //grall_cubc[iset]->GetYaxis()->SetRangeUser(-0.02,0.02);
      grall_cubc[iset]->GetXaxis()->SetTimeDisplay(1);
      grall_cubc[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      grall_cubc[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_cubc[iset]->Draw("P");}
    }
  c3->GetPad(1)->SetGridy();
  c3->GetPad(1)->SetGridx();
  c3->GetPad(2)->SetGridy();
  c3->GetPad(2)->SetGridx();
  c3->GetPad(3)->SetGridy();
  c3->GetPad(3)->SetGridx();
  c3->GetPad(4)->SetGridy();
  c3->GetPad(4)->SetGridx();

  //c3->SaveAs("mdata/qc_msht_cubc"+suf+".pdf");   


  TF1 *dcoff_p1 = new TF1("ft","pol1");

  //TCanvas *c4=new TCanvas("c4","c4",1600,1000);
  //c4->Divide(1,2);
  /*c4->cd(1);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_dcoff_T[iset]->Draw("AP");
      grall_dcoff_T[iset]->GetYaxis()->SetRangeUser(1.86,1.92);
      grall_dcoff_T[iset]->Fit(dcoff_p1);
    }
    else{grall_dcoff_T[iset]->Draw("P");}
    }*/
  
  /*c4->cd(2);
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_qamp_T[iset]->Draw("AP");
      grall_qamp_T[iset]->GetYaxis()->SetRangeUser(0.08,0.11);
    }
    else{grall_qamp_T[iset]->Draw("P");}
    }*/
  /*c4->GetPad(1)->SetGridy();
  c4->GetPad(1)->SetGridx();
  c4->GetPad(2)->SetGridy();
  c4->GetPad(2)->SetGridx();*/
  //c4->SaveAs("mdata/qc_off_amp_T"+suf+".pdf"); 

  theApp->Run();  

}

