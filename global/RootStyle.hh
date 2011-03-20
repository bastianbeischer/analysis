#ifndef RootStyle_hh
#define RootStyle_hh

#include <TStyle.h>
#include <TColor.h>

class RootStyle : public TStyle
{
public:
  enum PaletteType {DefaultPalette, ResiduePalette};
public:
  ~RootStyle();
  static void set();
  static void setPalette(PaletteType);
private:
  RootStyle();
  static RootStyle* s_instance;
};

RootStyle::RootStyle()
  : TStyle("RootStyle", "RootStyle")
{
  SetFrameFillColor(0);
  SetPadColor(0);
  SetCanvasColor(0);
  SetTitleFillColor(0);
  SetTitleBorderSize(1);
  SetStatColor(0);
  SetStatBorderSize(1);
  SetLegendBorderSize(1);
  SetPalette(1, 0);
  SetOptStat(0);
  SetOptFit(111);
  SetLineScalePS(1);
  SetHistLineWidth(2);
  SetLabelFont(42, "XYZ");
  SetLabelSize(0.025, "XYZ");
  SetStatFont(42);
  SetStatFontSize(0.025);
  SetTextFont(42);
  SetTitleFont(42, "XYZ");
  SetTitleSize(0.025, "XYZ");
  SetPadGridX(1);
  SetPadGridY(1);
  SetMarkerStyle(20);
  SetPaintTextFormat(".2f");
  SetNumberContours(99);
}

RootStyle* RootStyle::s_instance = 0;

void RootStyle::set()
{
  if (!s_instance)
    s_instance = new RootStyle;
  s_instance->cd();
}

RootStyle::~RootStyle()
{
}

void RootStyle::setPalette(PaletteType type)
{
  gStyle->SetNumberContours(99);
  if (type == ResiduePalette) {
    double stops[] = {0.00, 0.25, 0.50, 0.75, 1.00};
    double red[]   = {0.00, 0.00, 0.00, 0.50, 1.00};
    double green[] = {0.00, 0.00, 0.00, 0.00, 0.00};
    double blue[]  = {1.00, 0.50, 0.00, 0.00, 0.00};
    TColor::CreateGradientColorTable(5, stops, red, green, blue, gStyle->GetNumberContours());
    return;
  }
  gStyle->SetPalette(1, 0);
}

#endif
