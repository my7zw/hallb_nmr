#include <iostream>
#include "TString.h"
#include <cstdlib>
#include <iomanip>

#include "TCanvas.h"
#include "THupgrade.hh"


//#include "mclas/T9301pars.hh"
#include "mclas/T9301make.hh"
#include "mclas/T9301read.hh"


using std::cout;
using std::endl;

void PrintUsage(){
  cout<<endl<<"--------------------------------Usage--------------------------------"<<endl;
  cout<<"to fill: ./pas_db create"<<endl;
  cout<<"to print pas info: ./pas_db prtkin <kn_start>[1] <kn_stop>[108]"<<endl;
  cout<<"---------------------------------------------------------------------"<<endl<<endl;
}    

int main(int argc, char **argv)
{
  cout<<"argc = "<<argc<<endl;
  TString db_fn=argv[1];

  Int_t check=0;


  check++;

  T9301make *db = new T9301make(db_fn);
  db->FillT9301Tree();


  //TCanvas *c=new TCanvas("c","c",1600,1000);
  //c->Divide(2,2);

  
  /*T9301read *db = new T9301read(db_fn);
  Int_t nev = db->GetT9301Entries();
  cout<<nev<<endl;
  for(Int_t i=0;i<nev;i++){
    //cout<<"Bytes read: "<<db->GetT9301EntryWithIndex(i)<<endl;

    db->GetT9301Entry(i);
    //cout<<i<<endl;
    for(Int_t j=0;j<db->data_np;j++){
      cout<<i<<" "<<j<<" "<<db->p_data_sig->at(j)<<endl;
    }
    //cout<<"size "<<db->p_data_sig->size()<<endl;
    //cout<<"size "<<db->data_np<<" "<<db->p_ev_time_str->Data()<<endl;
    //cout<<"Printing kin: "<<i<<endl;
    //db->ShowEvent();


    //c->cd(1);
    //cout<<"mean "<<db->data_hSvsF->GetMean()<<endl;
    //c->cd(2);
    //db->data_gSvsF->Draw("AP");

    }*/
  
  return 0;
}

