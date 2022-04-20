#ifndef THupgrade_hh
#define THupgrade_hh

#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TH2D.h>
#include <TGraphErrors.h>

class TH1Fu: public TH1F{
public:
  TH1Fu(const char* hn, const char* ht, Int_t nbin, Double_t low, Double_t up);
  TH1Fu();
  ~TH1Fu();

  void SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst);
  void SetFillAll(Color_t fcolor, Style_t fstyle);
  void SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth);
  void SetMarkerAll(Color_t tcolor, Size_t msize, Style_t mstyle);
};			 

//------------------------------------------------------------------------------------//

class TH1Du: public TH1D{
public:
  TH1Du(const char* hn, const char* ht, Int_t nbin, Double_t low, Double_t up);
  TH1Du();
  ~TH1Du();

  void SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst);
  void SetFillAll(Color_t fcolor, Style_t fstyle);
  void SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth);
  void SetMarkerAll(Color_t tcolor, Size_t msize, Style_t mstyle);
};			 

//------------------------------------------------------------------------------------//

class TH2Fu: public TH2F{
public:
  TH2Fu(const char* hn, const char* ht, Int_t nbx, Double_t xl, Double_t xu,Int_t nby, Double_t yl, Double_t yu);
  TH2Fu();
  ~TH2Fu();

  void SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst);
  void SetFillAll(Color_t fcolor, Style_t fstyle);
  void SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth);
  void SetMarkerAll(Color_t tcolor, Size_t msize, Style_t mstyle);
};			 

//------------------------------------------------------------------------------------//

class TH2Du: public TH2D{
public:
  TH2Du(const char* hn, const char* ht, Int_t nbx, Double_t xl, Double_t xu,Int_t nby, Double_t yl, Double_t yu);
  TH2Du();
  ~TH2Du();

  void SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst);
  void SetFillAll(Color_t fcolor, Style_t fstyle);
  void SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth);
  void SetMarkerAll(Color_t tcolor, Size_t msize, Style_t mstyle);
};			 

//------------------------------------------------------------------------------------//

class TGraphu: public TGraph{
public:
  TGraphu(Int_t n, Float_t* x, Float_t* y);
  TGraphu(Int_t n, Double_t* x, Double_t* y);
  TGraphu();
  ~TGraphu();

  void SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst);
  void SetFillAll(Color_t fcolor, Style_t fstyle);
  void SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth);
  void SetMarkerAll(Color_t tcolor, Size_t msize, Style_t mstyle);
};			 

//------------------------------------------------------------------------------------//

class TGraphErrorsu: public TGraphErrors{
public:
  TGraphErrorsu(Int_t n, Float_t* x, Float_t* y, Float_t* ex, Float_t* ey);
  TGraphErrorsu(Int_t n, Double_t* x, Double_t* y, Double_t* ex, Double_t* ey);
  TGraphErrorsu(const char* fn, const char* frmt, Option_t* opt);
  TGraphErrorsu();
  ~TGraphErrorsu();

  void SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst);
  void SetFillAll(Color_t fcolor, Style_t fstyle);
  void SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth);
  void SetMarkerAll(Color_t tcolor, Size_t msize, Style_t mstyle);
};			 


#endif
