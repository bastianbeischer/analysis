#ifndef RootStyle_hh
#define RootStyle_hh

#include <TStyle.h>

class RootStyle : public TStyle
{
public:
  ~RootStyle();
  static void set();

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

#endif
