#include "H1DPlot.hh"

#include <TH1.h>
#include <TList.h>
#include <TCanvas.h>
#include <TPad.h>

#include <QDebug>

const QVector<RootPlot::DrawOption> H1DPlot::s_drawOptions = QVector<DrawOption>()
  << BLANK << E1X0 << P << L << C << HIST << LP;

H1DPlot::H1DPlot()
  : RootPlot()
  , m_histograms()
  , m_xAxis(0)
  , m_yAxis(0)
  , m_xAxisTitle()
  , m_yAxisTitle()
  , m_drawOptions()
{
  m_drawOption = BLANK;
  m_type = RootPlot::H1DPlot;
}

H1DPlot::~H1DPlot()
{
  foreach (TH1D* h, m_histograms)
    delete h;
  m_histograms.clear();
}

void H1DPlot::draw(TCanvas* canvas)
{
  if (numberOfHistograms()) {
    canvas->cd();
    histogram(0)->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle));
    histogram(0)->Draw(qPrintable(drawOption(m_drawOption)));
    for (int i = 1; i < numberOfHistograms(); ++i)
      histogram(i)->Draw(qPrintable("SAME" + drawOption(m_drawOptions[i])));
    m_drawn = true;
  }
  RootPlot::draw(canvas);
}

void H1DPlot::unzoom()
{
  if (m_drawn) {
    xAxis()->UnZoom();
    yAxis()->UnZoom();
  }
}

void H1DPlot::clear()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    histogram(i)->Clear();
}

void H1DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  histogram(i)->Clear();
}

int H1DPlot::numberOfHistograms()
{
  return m_histograms.size();
}

TH1D* H1DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  return m_histograms[i];
}

void H1DPlot::addHistogram(TH1D* h, DrawOption option)
{
  m_histograms.append(h);
  m_drawOptions.append(option);
  if (!m_xAxis && !m_yAxis) {
    m_xAxis = h->GetXaxis();
    m_yAxis = h->GetYaxis();
  }
}

void H1DPlot::removeHistogram(int i)
{
  delete m_histograms.at(i);
  m_histograms[i] = 0;
  m_histograms.remove(i);
  m_drawOptions.remove(i);
}

const QVector<RootPlot::DrawOption>& H1DPlot::drawOptions()
{
  return s_drawOptions;
}

void H1DPlot::setAxisTitle(const QString& x, const QString& y)
{
  m_xAxisTitle = x;
  m_yAxisTitle = y;
}

TAxis* H1DPlot::xAxis()
{
  Q_ASSERT(m_xAxis);
  return m_xAxis;
}

TAxis* H1DPlot::yAxis()
{
  Q_ASSERT(m_yAxis);
  return m_yAxis;
}
