#include "H2DPlot.hh"

#include <TH2.h>
#include <TList.h>
#include <TCanvas.h>

const QVector<RootPlot::DrawOption> H2DPlot::s_drawOptions = QVector<RootPlot::DrawOption>()
  << RootPlot::COLZ << RootPlot::CONT4Z << RootPlot::LEGO << RootPlot::LEGO2 << RootPlot::LEGOCOLZ
  << RootPlot::SURF1 << RootPlot::COLZTEXT;

H2DPlot::H2DPlot()
  : RootPlot()
  , m_histogram()
  , m_palette(RootStyle::DefaultPalette)
  , m_xAxisTitle()
  , m_yAxisTitle()
  , m_zAxisTitle()
{
  m_drawOption = COLZ;
  m_type = RootPlot::H2DPlot;
}

H2DPlot::~H2DPlot()
{
  delete m_histogram;
}

void H2DPlot::setPalette(RootStyle::PaletteType palette)
{
  m_palette = palette;
}

void H2DPlot::draw(TCanvas* canvas)
{
  if (!m_histogram)
    return;
  canvas->cd();
  canvas->Clear();
  RootStyle::setPalette(m_palette);
  if (!m_drawn)
    m_histogram->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle + ";" + m_zAxisTitle));
  m_histogram->Draw(qPrintable(drawOption(m_drawOption)));
  m_drawn = true;
  RootPlot::draw(canvas);
}

void H2DPlot::unzoom()
{
  if (!m_histogram)
    return;
  m_histogram->GetXaxis()->UnZoom();
  m_histogram->GetYaxis()->UnZoom();
  m_histogram->GetZaxis()->UnZoom();
}

void H2DPlot::clear()
{
  if (!m_histogram)
    return;
  m_histogram->Clear();
}

TH2D* H2DPlot::histogram()
{
  return m_histogram;
}

void H2DPlot::setHistogram(TH2D* h)
{
  if (m_histogram)
    delete m_histogram;
  m_histogram = h;
}
  
const QVector<RootPlot::DrawOption>& H2DPlot::drawOptions()
{
  return s_drawOptions;
}
  
void H2DPlot::setAxisTitle(const QString& x, const QString& y, const QString& z)
{
  m_xAxisTitle = x;
  m_yAxisTitle = y;
  m_zAxisTitle = z;
}
