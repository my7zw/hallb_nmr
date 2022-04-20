#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>

#include "TRint.h"
#include "TSystem.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "THupgrade.hh"


#include "mclas/T8301view.hh"

using std::cout;
using std::endl;

void prt_latex(Double_t _x, Double_t _y, const char *tex){
  TLatex *t = new TLatex(); t->SetNDC(); t->SetTextColor(1); t->SetTextSize(0.04);
  t->DrawLatex(_x,_y,tex);
}

TString GetT9301TimeString(Double_t _unixtime){
  //check mclas/readme.txt                                                                     
  time_t time_stamp = _unixtime;
  char buf[100];
  strftime (buf, 100, "%F_%T", localtime(&time_stamp));
  TString time_str = buf;
  return time_str;
}

int main(int argc, char **argv)
{
  TRint *theApp = new TRint("pcal", &argc, argv, NULL, 0);
  
  cout<<"argc = "<<argc<<" "<<argv[1]<<endl;
  TString db_fn=theApp->Argv(1);

  T8301view *mv = new T8301view(db_fn);
  Int_t nev = mv->GetT9301Entries();
  cout<<nev<<endl;

  TCanvas *c=new TCanvas("c","c",800,1000);
  c->Divide(1,2);


  mv->GetInitialInfo();
  for(Int_t i=0;i<nev;i++){
    mv->GetT9301Entry(i);
    mv->IsEventOK();
    mv->PrepareBaselineVector();
    //cout<<mv->p_ev_time_str->Data()<<endl;

    c->cd(1);    
    TH1D *his = (TH1D*)mv->data_hSvsF->Clone(Form("b_h%d",i));
    his->Draw();
    cout<<"nbins "<<his->GetNbinsX()<<endl;
    c->cd(2);    
    mv->data_gSvsF->Draw("AP");
    cout<<"npnts "<<mv->data_gSvsF->GetN()<<endl;

    cout<<"gain "<<mv->daq_gain<<endl;
    TString lat = Form("event id: %15.0f",mv->ev_id);
    prt_latex(0.12,0.85,lat.Data());
    lat = GetT9301TimeString(mv->ev_time);
    prt_latex(0.12,0.80,lat.Data());
    lat = Form("Nsweeps/ev: %d",mv->data_nsweep);
    prt_latex(0.12,0.75,lat.Data());
    c->Update();
    c->Modified();
    //usleep(1000000);
    //pause();
    do{
      cout << '\n' << "Press a key to continue...";
    } while (std::cin.get() != '\n');
    
  }

  //return 0;
  //cout<<"1"<<endl;
  //new MyMainFrame(gClient->GetRoot(),200,200);
  //cout<<"2"<<endl;
  theApp->Run();

}

