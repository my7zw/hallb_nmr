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

  //TString dir = "mdata/data_86_87/root_res_cnfg_teen_n50/";
  //TString dir = "mdata/data_86_87/root_res_cnfg_tail_n50/";
  TString dir = "mdata/data_86_87/root_new_cnfg_teen_n50/";
  //TString dir = "mdata/data_86_87/root_new_cnfg_tail_n50/";

  TString flist_fn = "./smth_plot_86_87.txt";
  std::ifstream fin(flist_fn);
  if(!fin.good()){cout<<"File read failed"<<endl;exit(111);}
  
  Int_t nset_u=1, npnt_u=59;
  TFile *in_f_u[nset_u][npnt_u];
  TGraph *gr_u[nset_u][npnt_u];

  TString check_u[npnt_u];
  for(Int_t iset=0;iset<nset_u;iset++){
    for(Int_t ipnt=0;ipnt<npnt_u;ipnt++){
      TString i_fn;
      i_fn.ReadLine(fin);  if(!fin.good()){break;}
      TString hn = i_fn;
      hn.Remove(hn.Last('_'));
      i_fn.Prepend(dir.Data());
     
      in_f_u[iset][ipnt] = new TFile(i_fn.Data(),"READ");
      gr_u[iset][ipnt] = (TGraph*)in_f_u[iset][ipnt]->Get(hn);
      check_u[ipnt]=hn;
      cout<<i_fn<<endl;
    }
  }

  Int_t nset_l=1, npnt_l=60;
  TFile *in_f_l[nset_l][npnt_l];
  TGraph *gr_l[nset_l][npnt_l];
  TString check_l[npnt_l];
  TString check[npnt_l];
  for(Int_t iset=0;iset<nset_l;iset++){
    for(Int_t ipnt=0;ipnt<npnt_l;ipnt++){
      TString i_fn;
      i_fn.ReadLine(fin);  if(!fin.good()){break;}
      TString hn = i_fn;
      hn.Remove(hn.Last('_'));
      i_fn.Prepend(dir.Data());
     
      in_f_l[iset][ipnt] = new TFile(i_fn.Data(),"READ");
      gr_l[iset][ipnt] = (TGraph*)in_f_l[iset][ipnt]->Get(hn);
      check_l[ipnt]=hn;
      check[ipnt]=hn;
      cout<<i_fn<<endl;
    }
  }


  //-------------------------------------------------------------------
  cout<<"ay"<<endl;
  Int_t gmst_u[nset_l] = {21};
  TString gn_u[nset_l] = {"UVA_92/03"};
		
  Double_t ev_start = 0, cc=0;
  //Double_t calib = 25.159*100;//te
  //Double_t calib = 21.709*100;//en
  Double_t calib = 21.824*100;//en

  //Double_t calib = 25.086*100;//lanl teen
  //Double_t calib = 25.433*100;//lanl tail
  
  TGraphErrorsu *grall_u[nset_u];
  for(Int_t iset=0;iset<nset_u;iset++){
    grall_u[iset] = new TGraphErrorsu();
    grall_u[iset]->SetDecorationAll(0, gmst_u[iset],1, 4,0.7,gmst_u[iset]);
    grall_u[iset]->SetName("86_0_"+gn_u[iset]);
    grall_u[iset]->SetTitle("86_0_"+gn_u[iset]);

    cc=0;
    for(Int_t ipnt=0;ipnt<npnt_u;ipnt++){
      Int_t np     = gr_u[iset][ipnt]->GetN();
      Double_t *xp = gr_u[iset][ipnt]->GetX();
      Double_t *yp = gr_u[iset][ipnt]->GetY();

      Double_t flip = 1.;
      Double_t shiftx = 0.;
      if(ipnt>=15&&ipnt<=24 || ipnt>=35&&ipnt<=46){
	flip=-1.;
	shiftx = 6400.;
	cout<<ipnt<<" "<<check_u[ipnt]<<endl;}

      if(ipnt>=35&&ipnt<=46){
	shiftx = 4800.;}
	

      
      for(Int_t j=0;j<np;j++){
	if(j==0){ev_start=xp[0];}
	Double_t new_xx = ipnt*300 + xp[j]-ev_start;
	
	yp[j]=yp[j]*calib*flip;
	//if(yp[j]<0){yp[j]=-1.*yp[j]+0.0001;}
	//else{yp[j]=yp[j]+0.0001;}

	//grall_u[iset]->SetPoint(cc, new_xx, yp[j]);
	grall_u[iset]->SetPoint(cc, xp[j]-1567188020-shiftx, yp[j]);
	//grall_u[iset]->SetPoint(cc, new_xx, yp[j]*fit_u[0]/fit_u[iset]);

	//cout<<ipnt<<" "<<std::fixed<<std::setprecision(0)
	//<<std::setw(20)<<xp[j]<<endl;  
	cc++;
      }
      delete xp; xp = NULL;
      delete yp; yp = NULL;
    }
  }

  Int_t gmst_l[nset_l] = {21};
  TString gn_l[nset_l] = {"LANL_tuner"};
		
  ev_start = 0;
  TGraphErrorsu *grall_l[nset_l];
  TGraphErrorsu *grarf_l[nset_l-48];
  for(Int_t iset=0;iset<nset_l;iset++){
    grall_l[iset] = new TGraphErrorsu();
    grall_l[iset]->SetDecorationAll(0, gmst_l[iset],1, 2,0.7,gmst_l[iset]);
    grall_l[iset]->SetName("86_0_"+gn_l[iset]);
    grall_l[iset]->SetTitle("86_0_"+gn_l[iset]);

    grarf_l[iset] = new TGraphErrorsu();
    grarf_l[iset]->SetDecorationAll(0, gmst_l[iset],1, 616,0.7,gmst_l[iset]);
    grarf_l[iset]->SetName("87_"+gn_l[iset]);
    grarf_l[iset]->SetTitle("87_"+gn_l[iset]);
    
    cc=0;
    Int_t cc1=0;
    for(Int_t ipnt=0;ipnt<npnt_l;ipnt++){
      Int_t np     = gr_l[iset][ipnt]->GetN();
      Double_t *xp = gr_l[iset][ipnt]->GetX();
      Double_t *yp = gr_l[iset][ipnt]->GetY();

      Double_t flip = 1.;
      Double_t shiftx = 0.;
      if(ipnt>=17&&ipnt<=25 || ipnt>=37&&ipnt<=47){
	flip = -1.;
	shiftx = 6400.;
	cout<<ipnt<<" "<<check_l[ipnt]<<endl;
      }
      if( ipnt>=37&&ipnt<=47){
	shiftx = 4800.;
      }
      //if(ipnt>47){
	//cout<<check[ipnt]<<endl;
	//}

      for(Int_t j=0;j<np;j++){
	if(j==0){ev_start=xp[0];}
	Double_t new_xx = ipnt*300 + xp[j]-ev_start;
	//Double_t scaling = 1.0;
	Double_t scaling = 3.20305;
	//Double_t scaling = 2.21534;
	//Double_t scaling = 2.267586;//dbm convert vrms
	//Double_t scaling = 2.2675617;//dbm convert vpp
	//Double_t scaling = 2.21534*1.000947359;//2.21743872229
	yp[j]=yp[j]*flip;
	if(ipnt>47){scaling = 2.2315818931;}

	yp[j]=yp[j]*calib/scaling;
	//if(yp[j]<0){yp[j]=-1.*yp[j]+0.0001;}
	//else{yp[j]=yp[j]+0.0001;}
	
	//grall_l[iset]->SetPoint(cc, new_xx, yp[j]);
	grall_l[iset]->SetPoint(cc, xp[j]+139-1567188020-shiftx, yp[j]);
	//grall_l[iset]->SetPoint(cc, new_xx, yp[j]*fit_u[0]/fit_l[iset]);
	if(ipnt>47){
	  grarf_l[iset]->SetPoint(cc1, xp[j]+139-1567188020, yp[j]);
	  cc1++;
	}

	cc++;
      }
      delete xp; xp = NULL;
      delete yp; yp = NULL;
    }
  }

  TCanvas *c=new TCanvas("c","c",1600,1000);
  //c->Divide(2,2);
  c->cd();

  
  for(Int_t iset=0;iset<nset_u;iset++){
    if(iset==0){
      grall_u[iset]->Draw("AP");
      //grall_u[iset]->GetXaxis()->SetTimeDisplay(1);
      //grall_u[iset]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H\/%M}");   
      //grall_u[iset]->GetXaxis()->SetLabelOffset(0.02);
    }
    else{grall_u[iset]->Draw("P");}
  }
  for(Int_t iset=0;iset<nset_l;iset++){
    //if(iset==0){grall_u[iset]->Draw("AP");}
    grall_l[iset]->Draw("P");
    grarf_l[iset]->Draw("P");
  }
  //c->SetLogy();
  //c->SetGridy();
  //c->SetGridx();
  c->SetTicky();
  c->SetTickx();
  grall_u[0]->GetYaxis()->SetRangeUser(-100.,105.);
  //c->BuildLegend();
  //c->SaveAs("pdf/polramp_teen.pdf");
  //c->SaveAs("pdf/polramp_tail.pdf");

  //c->SaveAs("pdf/polrampnew_teen.pdf");
  //c->SaveAs("pdf/polrampnew_teen_lcalteen.pdf");
  //c->SaveAs("pdf/polrampnew_teen_lcaltail.pdf");
  //c->SaveAs("pdf/polrampnew_tail.pdf");

  
  //grall_u[0]->GetXaxis()->SetRangeUser(1567187920-1567188020, 1567192605-1567188020);
  //grall_u[0]->GetYaxis()->SetRangeUser(-0.2,0.6);
  c->Update(); c->Modified();
  //c->SaveAs("pdf/polramp_teen_z.pdf");
  //c->SaveAs("pdf/polramp_tail_z.pdf");
  
  //c->SaveAs("pdf/polrampnew_teen_z.pdf");
  //c->SaveAs("pdf/polrampnew_teen_z_lcalteen.pdf");
  //c->SaveAs("pdf/polrampnew_teen_z_lcaltail.pdf");
  //c->SaveAs("pdf/polrampnew_tail_z.pdf");

  theApp->Run();  

}

