#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>

#include "TRint.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "THupgrade.hh"


#include "mclas/T8301base.hh"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  //TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);

  gStyle->SetOptFit(1111);
  
  cout<<"argc = "<<argc<<" "<<argv[1]<<endl;
  TString db_fn=theApp->Argv(1);

  T8301base *bl = new T8301base(db_fn);
  Int_t nev = bl->GetT9301Entries();
  cout<<"number of events: "<<nev<<endl;

  TCanvas *c=new TCanvas("c","c",1600,1000);
  c->Divide(2,2);

  bl->GetInitialInfo();
  bl->InitHistos();
  bl->SetSampleSize(1);
  //for(Int_t i=2;i<nev;i++){
  for(Int_t i=0;i<nev;i++){
    bl->GetT9301Entry(i);
    bl->IsEventOK();
    bl->FillVectorAll();

    TH1D *his = (TH1D*)bl->data_hSvsF->Clone(Form("b_h%d",i));

    //if(i==0){bl->data_hSvsF->Draw();}
    //else{bl->data_hSvsF->Draw("same");}
    //if(i==0){his->Draw();}
    //else{his->Draw("same");}

    //bl->data_gSvsF->Draw("AP");
    c->Update();
    c->Modified();
    //usleep(100000);    
  }

  bl->FillHistAve();
  bl->CloneHistos();
  
  //----------------------------  
  c->cd(1);
  bl->FitP2();
  bl->GetP2FitSubtracted();
  
  c->cd(2);
  bl->bl_hSvsF_subtr->Draw();
  bl->FitConvP3();

  c->cd(3);
  bl->FitConvP2P3();
  
  c->cd(4);
  bl->GetResidual();  
  bl->bl_hSvsF_resid->Draw();
  bl->ffn_p2_p2p3_dif->Draw("Psame");

  //----------------------------  
  bl->WriteBaselineResults();


  theApp->Run();  
  //return 0;
}

