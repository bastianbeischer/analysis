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

#endif
