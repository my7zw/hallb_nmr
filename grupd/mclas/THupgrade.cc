#include "THupgrade.hh"

TH1Fu::TH1Fu(const char* hn, const char* ht, Int_t nbin, Double_t low, Double_t up):
TH1F(hn, ht, nbin, low, up){}
TH1Fu::TH1Fu(){}
TH1Fu::~TH1Fu(){}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Fu::SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst){
  SetFillColor(fc);
  SetLineColor(lc);
  SetLineWidth(lw);
  SetMarkerColor(mc);
  SetMarkerSize(ms);
  SetMarkerStyle(mst);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Fu::SetFillAll(Color_t fcolor, Style_t fstyle){
  SetFillColor(fcolor);
  SetFillStyle(fstyle);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Fu::SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth){
  SetLineColor(lcolor);
  SetLineStyle(lstyle);
  SetLineWidth(lwidth);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Fu::SetMarkerAll(Color_t mcolor, Size_t msize, Style_t mstyle){
  SetMarkerColor(mcolor);
  SetMarkerSize(msize);
  SetMarkerStyle(mstyle);
}

////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////

TH1Du::TH1Du(const char* hn, const char* ht, Int_t nbin, Double_t low, Double_t up):
TH1D(hn, ht, nbin, low, up){}
TH1Du::TH1Du(){}
TH1Du::~TH1Du(){}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Du::SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst){
  SetFillColor(fc);
  SetLineColor(lc);
  SetLineWidth(lw);
  SetMarkerColor(mc);
  SetMarkerSize(ms);
  SetMarkerStyle(mst);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Du::SetFillAll(Color_t fcolor, Style_t fstyle){
  SetFillColor(fcolor);
  SetFillStyle(fstyle);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Du::SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth){
  SetLineColor(lcolor);
  SetLineStyle(lstyle);
  SetLineWidth(lwidth);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH1Du::SetMarkerAll(Color_t mcolor, Size_t msize, Style_t mstyle){
  SetMarkerColor(mcolor);
  SetMarkerSize(msize);
  SetMarkerStyle(mstyle);
}

////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////

TH2Fu::TH2Fu(const char* hn, const char* ht, Int_t nbx, Double_t xl, Double_t xu,Int_t nby, Double_t yl, Double_t yu):
TH2F(hn, ht, nbx, xl, xu, nby, yl, yu){}
TH2Fu::TH2Fu(){}
TH2Fu::~TH2Fu(){}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Fu::SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst){
  SetFillColor(fc);
  SetLineColor(lc);
  SetLineWidth(lw);
  SetMarkerColor(mc);
  SetMarkerSize(ms);
  SetMarkerStyle(mst);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Fu::SetFillAll(Color_t fcolor, Style_t fstyle){
  SetFillColor(fcolor);
  SetFillStyle(fstyle);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Fu::SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth){
  SetLineColor(lcolor);
  SetLineStyle(lstyle);
  SetLineWidth(lwidth);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Fu::SetMarkerAll(Color_t mcolor, Size_t msize, Style_t mstyle){
  SetMarkerColor(mcolor);
  SetMarkerSize(msize);
  SetMarkerStyle(mstyle);
}

////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////

TH2Du::TH2Du(const char* hn, const char* ht, Int_t nbx, Double_t xl, Double_t xu,Int_t nby, Double_t yl, Double_t yu):
TH2D(hn, ht, nbx, xl, xu, nby, yl, yu){}
TH2Du::TH2Du(){}
TH2Du::~TH2Du(){}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Du::SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst){
  SetFillColor(fc);
  SetLineColor(lc);
  SetLineWidth(lw);
  SetMarkerColor(mc);
  SetMarkerSize(ms);
  SetMarkerStyle(mst);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Du::SetFillAll(Color_t fcolor, Style_t fstyle){
  SetFillColor(fcolor);
  SetFillStyle(fstyle);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Du::SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth){
  SetLineColor(lcolor);
  SetLineStyle(lstyle);
  SetLineWidth(lwidth);
}

////////////////////////////////////////////////////////////////////////////////////////

void TH2Du::SetMarkerAll(Color_t mcolor, Size_t msize, Style_t mstyle){
  SetMarkerColor(mcolor);
  SetMarkerSize(msize);
  SetMarkerStyle(mstyle);
}

////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////

TGraphu::TGraphu(Int_t n, Float_t* x, Float_t* y):TGraph(n,x,y){}
TGraphu::TGraphu(Int_t n, Double_t* x, Double_t* y):TGraph(n,x,y){}
TGraphu::TGraphu(){}
TGraphu::~TGraphu(){}

////////////////////////////////////////////////////////////////////////////////////////

void TGraphu::SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst){
  SetFillColor(fc);
  SetLineColor(lc);
  SetLineWidth(lw);
  SetMarkerColor(mc);
  SetMarkerSize(ms);
  SetMarkerStyle(mst);
}

////////////////////////////////////////////////////////////////////////////////////////

void TGraphu::SetFillAll(Color_t fcolor, Style_t fstyle){
  SetFillColor(fcolor);
  SetFillStyle(fstyle);
}

////////////////////////////////////////////////////////////////////////////////////////

void TGraphu::SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth){
  SetLineColor(lcolor);
  SetLineStyle(lstyle);
  SetLineWidth(lwidth);
}

////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////

TGraphErrorsu::TGraphErrorsu(Int_t n, Float_t* x, Float_t* y, Float_t* ex, Float_t* ey):
TGraphErrors(n,x,y,ex,ey){}
TGraphErrorsu::TGraphErrorsu(Int_t n, Double_t* x, Double_t* y, Double_t* ex, Double_t* ey):
TGraphErrors(n,x,y,ex,ey){}
TGraphErrorsu::TGraphErrorsu(const char* fn, const char* frmt, Option_t* opt):
TGraphErrors(fn, frmt, opt){}
TGraphErrorsu::TGraphErrorsu(){}
TGraphErrorsu::~TGraphErrorsu(){}

////////////////////////////////////////////////////////////////////////////////////////

void TGraphErrorsu::SetDecorationAll(Color_t fc,Color_t lc,Width_t lw,Color_t mc,Size_t ms,Style_t mst){
  SetFillColor(fc);
  SetLineColor(lc);
  SetLineWidth(lw);
  SetMarkerColor(mc);
  SetMarkerSize(ms);
  SetMarkerStyle(mst);
}

////////////////////////////////////////////////////////////////////////////////////////

void TGraphErrorsu::SetFillAll(Color_t fcolor, Style_t fstyle){
  SetFillColor(fcolor);
  SetFillStyle(fstyle);
}

////////////////////////////////////////////////////////////////////////////////////////

void TGraphErrorsu::SetLineAll(Color_t lcolor, Style_t lstyle, Width_t lwidth){
  SetLineColor(lcolor);
  SetLineStyle(lstyle);
  SetLineWidth(lwidth);
}

////////////////////////////////////////////////////////////////////////////////////////
