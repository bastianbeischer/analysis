#include "H2DPlot.hh"

#include <TH2.h>
#include <TList.h>
#include <TCanvas.h>

const QVector<RootPlot::DrawOption> H2DPlot::s_drawOptions = QVector<RootPlot::DrawOption>()
  << RootPlot::COLZ << RootPlot::SCAT << RootPlot::CONT4Z << RootPlot::LEGO << RootPlot::LEGO2
  << RootPlot::SURF1 << RootPlot::COLZTEXT << RootPlot::BLANK;

H2DPlot::H2DPlot()
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
  m_drawOption = COLZ;
  m_type = RootPlot::H2DPlot;
}

H2DPlot::~H2DPlot()
{
  foreach(TH2D* h, m_histograms)
    delete h;
  m_histograms.clear();
  m_drawOptions.clear();
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
  histogram(0)->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle + ";" + m_zAxisTitle));
  histogram(0)->Draw(qPrintable(drawOption(m_drawOption)));
  for (int i = 1; i < numberOfHistograms(); ++i)
    histogram(i)->Draw(qPrintable("SAME" + drawOption(m_drawOptions[i])));
  m_drawn = true;
  RootPlot::draw(canvas);
}

void H2DPlot::unzoom()
{
  if (m_drawn) {
    xAxis()->UnZoom();
    yAxis()->UnZoom();
    zAxis()->UnZoom();
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

TH2D* H2DPlot::histogram(int i) const
{
  Q_ASSERT(0 <= i && i <= numberOfHistograms());
  return m_histograms[i];
}

void H2DPlot::addHistogram(TH2D* h, DrawOption option)
{
  m_histograms.append(h);
  m_drawOptions.append(option);
  if (!m_xAxis && !m_yAxis && !m_zAxis) {
    m_xAxis = h->GetXaxis();
    m_yAxis = h->GetYaxis();
    m_zAxis = h->GetZaxis();
  }
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

int H2DPlot::numberOfHistograms() const
{
  return m_histograms.size();
}

TAxis* H2DPlot::xAxis() const
{
  Q_ASSERT(m_xAxis);
  return m_xAxis;
}

TAxis* H2DPlot::yAxis() const
{
  Q_ASSERT(m_yAxis);
  return m_yAxis;
}

TAxis* H2DPlot::zAxis() const
{
  Q_ASSERT(m_zAxis);
  return m_zAxis;
}
