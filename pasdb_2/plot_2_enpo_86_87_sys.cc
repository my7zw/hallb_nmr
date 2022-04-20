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
  //TString dir = "mdata/data_86_87/root_new_cnfg_teen_n50/";
  //TString dir = "mdata/data_86_87/root_new_cnfg_tail_n50/";

  Int_t ncfg = 3;
  TString dir[ncfg] = {"mdata/data_86_87/root_new_cnfg_teen_-/",
		       "mdata/data_86_87/root_new_cnfg_teen_0/",
		       "mdata/data_86_87/root_new_cnfg_teen_+/"};

  
  TString flist_fn = "./smth_plot_86_87.txt";
  std::ifstream fin(flist_fn);
  if(!fin.good()){cout<<"File read failed"<<endl;exit(111);}
  
  Int_t nset_u=1, npnt_u=59;
  TFile *in_f_u[nset_u][npnt_u][ncfg];
  TGraph *gr_u[nset_u][npnt_u][ncfg];

  for(Int_t iset=0;iset<nset_u;iset++){
    for(Int_t ipnt=0;ipnt<npnt_u;ipnt++){
      TString i_fn;
      i_fn.ReadLine(fin);  if(!fin.good()){break;}
      TString hn = i_fn;
      hn.Remove(hn.Last('_'));

      for(Int_t icfg=0;icfg<ncfg;icfg++){
	TString i_fn_new = Form("%s%s",dir[icfg].Data(),i_fn.Data());
	//i_fn.Prepend(dir.Data());
     
	in_f_u[iset][ipnt][icfg] = new TFile(i_fn_new.Data(),"READ");
	gr_u[iset][ipnt][icfg] = (TGraph*)in_f_u[iset][ipnt][icfg]->Get(hn);

	//cout<<i_fn<<endl;
      }
    }
  }

  Int_t nset_l=1, npnt_l=60;
  TFile *in_f_l[nset_l][npnt_l][ncfg];
  TGraph *gr_l[nset_l][npnt_l][ncfg];
  TString check[npnt_l][ncfg];
  for(Int_t iset=0;iset<nset_l;iset++){
    for(Int_t ipnt=0;ipnt<npnt_l;ipnt++){
      TString i_fn;
      i_fn.ReadLine(fin);  if(!fin.good()){break;}
      TString hn = i_fn;
      hn.Remove(hn.Last('_'));

      for(Int_t icfg=0;icfg<ncfg;icfg++){
	TString i_fn_new = Form("%s%s",dir[icfg].Data(),i_fn.Data());
	//i_fn.Prepend(dir.Data());
     
	in_f_l[iset][ipnt][icfg] = new TFile(i_fn_new.Data(),"READ");
	gr_l[iset][ipnt][icfg] = (TGraph*)in_f_l[iset][ipnt][icfg]->Get(hn);
	check[ipnt][icfg]=hn;
	//cout<<i_fn<<endl;
      }
    }
  }


  //-------------------------------------------------------------------
  cout<<"ay"<<endl;
  Int_t gmst_u[nset_u] = {21};
  TString gn_u[nset_u] = {"UVA_92/03"};
		
  Double_t ev_start = 0;
  Int_t cc_u[ncfg]={0,0,0};

  //Double_t calib = 25.159*100;//te
  //Double_t calib = 21.709*100;//en
  Double_t calib = 21.824*100;//en

  //Double_t calib = 25.086*100;//lanl teen
  //Double_t calib = 25.433*100;//lanl tail

  TString grn[ncfg] = {"-","0","+"};
  
  TGraphErrorsu *grall_u[nset_u][ncfg];
  for(Int_t iset=0;iset<nset_u;iset++){
    for(Int_t icfg=0;icfg<ncfg;icfg++){
      grall_u[iset][icfg] = new TGraphErrorsu();
      grall_u[iset][icfg]->SetDecorationAll(0, 4,1, 4,0.7,gmst_u[iset]);
      grall_u[iset][icfg]->SetName("uva_"+grn[icfg]);
      grall_u[iset][icfg]->SetTitle("uva_"+grn[icfg]);
    }
  }

  for(Int_t iset=0;iset<nset_u;iset++){
    for(Int_t ipnt=0;ipnt<npnt_u;ipnt++){
      Int_t np[ncfg];
      Double_t *xp[ncfg];
      Double_t *yp[ncfg];
      Double_t *ype[ncfg];

      for(Int_t icfg=0;icfg<ncfg;icfg++){
	 np[icfg] = gr_u[iset][ipnt][icfg]->GetN();
	 xp[icfg] = gr_u[iset][ipnt][icfg]->GetX();
	 yp[icfg] = gr_u[iset][ipnt][icfg]->GetY();
	 ype[icfg] = gr_u[iset][ipnt][icfg]->GetEY();

	 
	 for(Int_t j=0;j<np[icfg];j++){
	   yp[icfg][j]=yp[icfg][j]*calib;
	   grall_u[iset][icfg]->SetPoint(cc_u[icfg], xp[icfg][j]-1567188020, yp[icfg][j]);
	   grall_u[iset][icfg]->SetPointError(cc_u[icfg], 0., ype[icfg][j]);
	   //cout<<ipnt<<" "<<std::fixed<<std::setprecision(0)
	   //<<std::setw(20)<<xp[icfg][j]<<endl;  
	   cc_u[icfg]++;
	 }
      }
    }
  }

  Int_t gmst_l[nset_l] = {21};
  TString gn_l[nset_l] = {"LANL_tuner"};
		
  ev_start = 0;
  TGraphErrorsu *grall_l[nset_l][ncfg];
  TGraphErrorsu *grarf_l[nset_l-48][ncfg];
  for(Int_t iset=0;iset<nset_l;iset++){
    for(Int_t icfg=0;icfg<ncfg;icfg++){
      grall_l[iset][icfg] = new TGraphErrorsu();
      grall_l[iset][icfg]->SetDecorationAll(0, 2,1, 2,0.7,gmst_l[iset]);
      grall_l[iset][icfg]->SetName("lanl_"+grn[icfg]);
      grall_l[iset][icfg]->SetTitle("lanl_"+grn[icfg]);

      grarf_l[iset][icfg] = new TGraphErrorsu();
      grarf_l[iset][icfg]->SetDecorationAll(0, 616,1, 616,0.7,gmst_l[iset]);
      grarf_l[iset][icfg]->SetName("lanlrf_"+grn[icfg]);
      grarf_l[iset][icfg]->SetTitle("lanlrf_"+grn[icfg]);
    }
  }

  Int_t cc_l[ncfg]={0,0,0};
  Int_t cc_l1[ncfg]={0,0,0};

  for(Int_t iset=0;iset<nset_l;iset++){
    for(Int_t ipnt=0;ipnt<npnt_l;ipnt++){
      Int_t np[ncfg];
      Double_t *xp[ncfg];
      Double_t *yp[ncfg];
      Double_t *ype[ncfg];
      
      for(Int_t icfg=0;icfg<ncfg;icfg++){
	 np[icfg] = gr_l[iset][ipnt][icfg]->GetN();
	 xp[icfg] = gr_l[iset][ipnt][icfg]->GetX();
	 yp[icfg] = gr_l[iset][ipnt][icfg]->GetY();
	 ype[icfg] = gr_l[iset][ipnt][icfg]->GetEY();

	 //if(ipnt>47){ cout<<check[ipnt]<<endl;}

	 for(Int_t j=0;j<np[icfg];j++){
	   //Double_t scaling = 1.0;
	   Double_t scaling = 3.20305;
	   //Double_t scaling = 2.267586;//dbm convert vrms
	   //Double_t scaling = 2.2675617;//dbm convert vpp
	   //Double_t scaling = 2.21534*1.000947359;//2.21743872229
	   if(ipnt>47){scaling = 2.2315818931;}

	   yp[icfg][j]=yp[icfg][j]*calib/scaling;
	   //if(ipnt<=47){
	     grall_l[iset][icfg]->SetPoint(cc_l[icfg],
					   xp[icfg][j]+139-1567188020,
					   yp[icfg][j]);
	     grall_l[iset][icfg]->SetPointError(cc_l[icfg], 0., ype[icfg][j]);
	     cc_l[icfg]++;
	     //}
	   //cout<<ipnt<<" "<<cc_l[icfg]<<endl;
	   if(ipnt>47){
	     grarf_l[iset][icfg]->SetPoint(cc_l1[icfg],
					   xp[icfg][j]+139-1567188020,
					   yp[icfg][j]);
	     grarf_l[iset][icfg]->SetPointError(cc_l1[icfg], 0., ype[icfg][j]);
	     cc_l1[icfg]++;
	   }
	   
	 }
      }
    }
  }

  Int_t np_u[ncfg];
  Double_t *xp_u[ncfg];
  Double_t *yp_u[ncfg];
  Double_t *ype_u[ncfg];

  Int_t np_l[ncfg];
  Double_t *xp_l[ncfg];
  Double_t *yp_l[ncfg];
  Double_t *ype_l[ncfg];

  Int_t np_lrf[ncfg];
  Double_t *xp_lrf[ncfg];
  Double_t *yp_lrf[ncfg];
  Double_t *ype_lrf[ncfg];

  
  for(Int_t icfg=0;icfg<ncfg;icfg++){
    np_u[icfg] = grall_u[0][icfg]->GetN();
    xp_u[icfg] = grall_u[0][icfg]->GetX();
    yp_u[icfg] = grall_u[0][icfg]->GetY();
    ype_u[icfg] = grall_u[0][icfg]->GetEY();

    np_l[icfg] = grall_l[0][icfg]->GetN();
    xp_l[icfg] = grall_l[0][icfg]->GetX();
    yp_l[icfg] = grall_l[0][icfg]->GetY();
    ype_l[icfg] = grall_l[0][icfg]->GetEY();

    np_lrf[icfg] = grarf_l[0][icfg]->GetN();
    xp_lrf[icfg] = grarf_l[0][icfg]->GetX();
    yp_lrf[icfg] = grarf_l[0][icfg]->GetY();
    ype_lrf[icfg] = grarf_l[0][icfg]->GetEY();
  }
  
  TGraphErrorsu *gave_u = new TGraphErrorsu();
  gave_u->SetDecorationAll(0, 4,1, 4,0.9,25);
  //gave_u->SetDecorationAll(0, 1,1, 1,0.7,25);
  gave_u->SetTitle("uva_av");

  TGraphErrorsu *gave_l = new TGraphErrorsu();
  gave_l->SetDecorationAll(0, 2,1, 2,1.2,22);
  //gave_l->SetDecorationAll(0, 1,1, 1,0.9,22);
  gave_l->SetTitle("lanl_av");

  TGraphErrorsu *gaverf_l = new TGraphErrorsu();
  gaverf_l->SetDecorationAll(0, 2,1, 2,1.2,23);
  //gaverf_l->SetDecorationAll(0, 1,1, 1,0.9,23);
  gaverf_l->SetTitle("lanl_av");

  TGraphErrorsu *gsys_p_u = new TGraphErrorsu();
  gsys_p_u->SetDecorationAll(0, 4,1, 4,0.7,25);
  gsys_p_u->SetTitle("uva_sys_p");
  
  TGraphErrorsu *gsys_m_u = new TGraphErrorsu();
  gsys_m_u->SetDecorationAll(0, 4,1, 4,0.7,24);
  gsys_m_u->SetTitle("uva_sys_m");

  TGraphErrorsu *gstd_u = new TGraphErrorsu();
  gstd_u->SetDecorationAll(0, 4,1, 4,0.7,27);
  gstd_u->SetTitle("uva_std");

  TGraphErrorsu *gsys_p_l = new TGraphErrorsu();
  gsys_p_l->SetDecorationAll(0, 2,1, 2,0.7,25);
  gsys_p_l->SetTitle("lanl_sys_p");

  TGraphErrorsu *gsys_m_l = new TGraphErrorsu();
  gsys_m_l->SetDecorationAll(0, 2,1, 2,0.7,24);
  gsys_m_l->SetTitle("lanl_sys_p");

  TGraphErrorsu *gstd_l = new TGraphErrorsu();
  gstd_l->SetDecorationAll(0, 2,1, 2,0.7,27);
  gstd_l->SetTitle("lanl_std");

  for(Int_t j=0;j<np_u[0];j++){
    Double_t ypav_u = (yp_u[0][j] + yp_u[1][j] + yp_u[2][j])/3.;
    Double_t sys_m_u = 2.*fabs(yp_u[1][j]-yp_u[0][j])/fabs(yp_u[1][j]+yp_u[0][j]);
    Double_t sys_p_u = 2.*fabs(yp_u[1][j]-yp_u[2][j])/fabs(yp_u[1][j]+yp_u[2][j]);
    Double_t sys_av_u = (sys_m_u + sys_p_u)/2.;
    
    Double_t std_u = pow(yp_u[0][j]-ypav_u,2) +
                     pow(yp_u[1][j]-ypav_u,2) +
                     pow(yp_u[2][j]-ypav_u,2);
    std_u = sqrt(std_u/3./2.)/ypav_u;
    
    gave_u->SetPoint(j, xp_u[0][j]+1567188020, ypav_u);
    //gave_u->SetPointError(j, 0., sys_av_u);
    gave_u->SetPointError(j, 0., std_u);

    gsys_p_u->SetPoint(j, xp_u[0][j], 100.*sys_p_u);
    gsys_m_u->SetPoint(j, xp_u[0][j], 100.*sys_m_u);

    gstd_u->SetPoint(j, xp_u[0][j], 100.*std_u);
    //cout<<j<<" "<<sys_p_u<<" "<<sys_m_u<<" "<<std_u<<endl;
    cout<<j<<" "<<std::fixed<<std::setprecision(0)
	<<std::setw(20)<<xp_u[0][j]+1567188020<<endl;  

  }

  Int_t cc1=0,cc2=0;
  for(Int_t j=0;j<np_l[0];j++){
    Double_t ypav_l = (yp_l[0][j] + yp_l[1][j] + yp_l[2][j])/3.;
    Double_t sys_m_l = 2.*fabs(yp_l[1][j]-yp_l[0][j])/fabs(yp_l[1][j]+yp_l[0][j]);
    Double_t sys_p_l = 2.*fabs(yp_l[1][j]-yp_l[2][j])/fabs(yp_l[1][j]+yp_l[2][j]);
    Double_t sys_av_l = (sys_m_l + sys_p_l)/2.;

    Double_t std_l = pow(yp_l[0][j]-ypav_l,2) +
                     pow(yp_l[1][j]-ypav_l,2) +
                     pow(yp_l[2][j]-ypav_l,2);
    std_l = sqrt(std_l/3./2.)/ypav_l;

    if(j<=99){
      gave_l->SetPoint(cc1, xp_l[0][j]+1567188020, ypav_l);
      //gave_l->SetPointError(j, 0., sys_av_l);
      gave_l->SetPointError(cc1, 0., std_l);
      cc1++;
    }
    //cout<<j<<" "<<std::fixed<<std::setprecision(0)
    //<<std::setw(20)<<xp_l[0][j]+1567188020<<endl;  

    if(j>99){//fouund by printing point number in graphs
      gaverf_l->SetPoint(cc2, xp_l[0][j]+1567188020, ypav_l);
      gaverf_l->SetPointError(cc2, 0., std_l);
      cc2++;
    }
    gsys_p_l->SetPoint(j, xp_l[0][j], 100.*sys_p_l);
    gsys_m_l->SetPoint(j, xp_l[0][j], 100.*sys_m_l);

    gstd_l->SetPoint(j, xp_l[0][j], 100.*std_l);
  }

  
  TCanvas *c=new TCanvas("c","c",1600,1000);
  //c->Divide(2,2);
  c->cd();

  for(Int_t icfg=0;icfg<ncfg;icfg++){  

    for(Int_t iset=0;iset<nset_u;iset++){
      if(iset==0&&icfg==0){ grall_u[iset][icfg]->Draw("AP"); }
      else{grall_u[iset][icfg]->Draw("P");}
    }
    for(Int_t iset=0;iset<nset_l;iset++){
      grall_l[iset][icfg]->Draw("P");
      grarf_l[iset][icfg]->Draw("P");
    }

  }
  //c->SetLogy();
  //c->SetGridy();
  //c->SetGridx();
  c->SetTicky();
  c->SetTickx();
  grall_u[0][0]->GetYaxis()->SetRangeUser(-100.,105.);
  //c->SaveAs("pdf/polrampnew_tail.pdf");

  
  //grall_u[0]->GetXaxis()->SetRangeUser(1567187920-1567188020, 1567192605-1567188020);
  //grall_u[0]->GetYaxis()->SetRangeUser(-0.2,0.6);
  c->Update(); c->Modified();
  //c->SaveAs("pdf/polrampnew_tail_z.pdf");


  TCanvas *c1=new TCanvas("c1","c1",1200,1000);
  c1->cd();
  gave_u->Draw("AP");
  gave_u->GetXaxis()->SetTimeDisplay(1);
  gave_u->GetXaxis()->SetNdivisions(406);
  //gave_u->GetXaxis()->SetTimeFormat("%d\/%m %H:%M");
  gave_u->GetXaxis()->SetTimeFormat("%H:%M");
  gave_u->GetXaxis()->SetTimeOffset(-4*3600,"gmt");
  gave_u->GetXaxis()->SetRangeUser(1567188020-1500, 1567223562+1500);
  //gave_u->GetYaxis()->SetRangeUser(-110.,110.);
  gave_u->GetYaxis()->SetRangeUser(-100.,100.);
  gave_u->GetYaxis()->SetNdivisions(505);
 
  gave_l->Draw("P");
  gaverf_l->Draw("P");
  c1->SetTicky();
  c1->SetTickx();
  c1->BuildLegend(0.15,0.25,0.35,0.4);
  //c1->SaveAs("pdf/polrampnew_sys.pdf");
  c1->SaveAs("pdf/polrampnew_sys_1.pdf");
  //c1->SaveAs("pdf/polrampnew_sys_2.pdf");

  //gave_u->GetXaxis()->SetRangeUser(1567187920-1567188020, 1567192605-1567188020);
  //gave_u->GetYaxis()->SetRangeUser(-0.2,0.6);


  //c1->SaveAs("pdf/polrampnew_sys_z.pdf");
  c1->Update(); c->Modified();
  TCanvas *c2=new TCanvas("c2","c2",1600,1000);
  c2->cd();
  gsys_p_u->Draw("AP");
  gsys_m_u->Draw("P");
  gsys_p_l->Draw("P");
  gsys_m_l->Draw("P");
  gstd_u->Draw("P");
  gstd_l->Draw("P");
  
  gsys_p_u->GetYaxis()->SetRangeUser(-100.,105.);
  c2->SetTicky();
  c2->SetTickx();
  c2->BuildLegend(0.1,0.25,0.4,0.4);



  theApp->Run();  

}

