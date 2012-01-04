#include "H3DPlot.hh"

#include <TH3.h>
#include <TList.h>
#include <TCanvas.h>

const QVector<RootPlot::DrawOption> H3DPlot::s_drawOptions = QVector<RootPlot::DrawOption>()
  << RootPlot::BLANK << RootPlot::ISO << RootPlot::LEGO;

H3DPlot::H3DPlot()
  : RootPlot()
  , m_xAxis(0)
  , m_yAxis(0)
  , m_zAxis(0)
  , m_histograms()
  , m_palette(RootStyle::DefaultPalette)
  , m_xAxisTitle()
  , m_yAxisTitle()
  , m_zAxisTitle()
  , m_drawOptions()
{
  m_drawOption = BLANK;
  m_type = RootPlot::H3DPlot;
}

H3DPlot::~H3DPlot()
{
  foreach(TH3D* h, m_histograms)
    delete h;
  m_histograms.clear();
  m_drawOptions.clear();
}

void H3DPlot::setPalette(RootStyle::PaletteType palette)
{
  m_palette = palette;
}

void H3DPlot::draw(TCanvas* canvas)
{
  if (!numberOfHistograms())
    return;
  canvas->cd();
  RootStyle::setPalette(m_palette);
  histogram(0)->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle + ";" + m_zAxisTitle));
  histogram(0)->Draw(qPrintable(drawOption(m_drawOption)));
  for (int i = 1; i < numberOfHistograms(); ++i)
    histogram(i)->Draw(qPrintable("SAME" + drawOption(m_drawOptions[i])));
  m_drawn = true;
  RootPlot::draw(canvas);
}

void H3DPlot::unzoom()
{
  if (m_drawn) {
    xAxis()->UnZoom();
    yAxis()->UnZoom();
    zAxis()->UnZoom();
  }
}

void H3DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i <= numberOfHistograms());
  histogram(i)->Clear();
}

void H3DPlot::clear()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    histogram(i)->Clear();
}

TH3D* H3DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i <= numberOfHistograms());
  return m_histograms[i];
}

void H3DPlot::addHistogram(TH3D* h, DrawOption option)
{
  m_histograms.append(h);
  m_drawOptions.append(option);
  if (!m_xAxis && !m_yAxis && !m_zAxis) {
    m_xAxis = h->GetXaxis();
    m_yAxis = h->GetYaxis();
    m_zAxis = h->GetZaxis();
  }
}

const QVector<RootPlot::DrawOption>& H3DPlot::drawOptions()
{
  return s_drawOptions;
}

void H3DPlot::setAxisTitle(const QString& x, const QString& y, const QString& z)
{
  m_xAxisTitle = x;
  m_yAxisTitle = y;
  m_zAxisTitle = z;
}

int H3DPlot::numberOfHistograms()
{
  return m_histograms.size();
}

TAxis* H3DPlot::xAxis()
{
  Q_ASSERT(m_xAxis);
  return m_xAxis;
}

TAxis* H3DPlot::yAxis()
{
  Q_ASSERT(m_yAxis);
  return m_yAxis;
}

TAxis* H3DPlot::zAxis()
{
  Q_ASSERT(m_zAxis);
  return m_zAxis;
}
