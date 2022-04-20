#ifndef T9301read_hh 
#define T9301read_hh 

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "T9301make.hh"

class T9301read: public T9301make{

public :   
 
  T9301read(TString _in_fn); 
  T9301read();
  ~T9301read();

  void      ReadT9301Tree();
  Int_t     GetT9301Entries();
  Int_t     GetT9301Entry(Long64_t _entry);
  Int_t     GetT9301EntryWithIndex(Int_t _index_rn);
  void      ShowEvent(Long64_t _entry=-1);

  std::vector<Double_t> *p_data_sig;
  TString               *p_ev_time_str;
  TString               *p_nmrU_card_name;

protected:

private:

  void       SetT9301InputParameters();
  void       SetT9301BranchesAddress();


};

#endif

