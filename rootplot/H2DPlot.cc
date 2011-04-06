#include "H2DPlot.hh"

#include <TH2.h>
#include <THStack.h>
#include <TList.h>
#include <TCanvas.h>

const QVector<RootPlot::DrawOption> H2DPlot::s_drawOptions = QVector<RootPlot::DrawOption>()
  << RootPlot::COLZ << RootPlot::CONT4Z << RootPlot::LEGO << RootPlot::LEGO2
  << RootPlot::SURF1 << RootPlot::COLZTEXT;

H2DPlot::H2DPlot()
  : RootPlot()
  , m_stack(new THStack)
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
  for (int i = 0; i < numberOfHistograms(); ++i)
    delete histogram(i);
  delete m_stack;
}

void H2DPlot::setPalette(RootStyle::PaletteType palette)
{
  m_palette = palette;
}

void H2DPlot::draw(TCanvas* canvas)
{
  if (!numberOfHistograms())
    return;
  canvas->cd();
  RootStyle::setPalette(m_palette);
  if (!m_drawn)
    m_stack->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle + ";" + m_zAxisTitle));
  m_stack->Draw(qPrintable(drawOption(m_drawOption)));
  m_drawn = true;
  RootPlot::draw(canvas);
}

void H2DPlot::unzoom()
{
  if (m_drawn) {
    m_stack->GetXaxis()->UnZoom();
    m_stack->GetYaxis()->UnZoom();
    //m_stack->GetZaxis()->UnZoom();
  }
}

void H2DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i <= numberOfHistograms());
  histogram(i)->Clear();
}

void H2DPlot::clear()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    histogram(i)->Clear();
}

TH2D* H2DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i <= numberOfHistograms());
  return static_cast<TH2D*>(m_stack->GetHists()->At(i));
}

void H2DPlot::addHistogram(TH2D* h, DrawOption option)
{
  m_stack->Add(h, qPrintable(drawOption(option)));
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

int H2DPlot::numberOfHistograms()
{
  return m_stack->GetHists() ? m_stack->GetHists()->GetSize() : 0;
}
