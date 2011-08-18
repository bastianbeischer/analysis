#include "ClusterShapePlot.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TGaxis.h>
#include <TStyle.h>
#include <TPad.h>

#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include <QVector>

#include <cmath>

ClusterShapePlot::ClusterShapePlot(unsigned short id) :
  AnalysisPlot(AnalysisPlot::ClusterShapeTracker),
  H1DPlot(),
  m_id(id),
  m_eventCounter(0)
{
  setTitle(QString("cluster shape 0x%1").arg(m_id,0,16));

  int nBins = 11;
  double x0 = -5.5;
  double x1 = 5.5;
  TH1D* histogram = new TH1D(qPrintable(title()),"", nBins, x0, x1);
  setAxisTitle("strip no. relative to maximum", "mean amplitude");
  histogram->GetXaxis()->SetNdivisions(520);
  addHistogram(histogram);

  TH1D* normHist = new TH1D(qPrintable(title() + "_normHisto"), "", nBins, x0, x1);
  normHist->SetLineColor(kRed);
  addHistogram(normHist);

  m_eventCountAxis = new TGaxis(x1, 0, x1, 1000, 0, 1000, 510, "+L");
  m_eventCountAxis->SetLineColor(kRed);
  m_eventCountAxis->SetTextColor(kRed);
  m_eventCountAxis->SetLabelColor(kRed);
  m_eventCountAxis->SetTitle("events / strip");
  m_eventCountAxis->SetTitleOffset(1.5);
  m_eventCountAxis->SetLabelFont(gStyle->GetLabelFont());
  m_eventCountAxis->SetLabelSize(gStyle->GetLabelSize());
  m_eventCountAxis->SetTitleFont(gStyle->GetTitleFont());
  m_eventCountAxis->SetTitleSize(gStyle->GetTitleSize());

  addLatex(RootPlot::newLatex(.15, .75));
}

ClusterShapePlot::~ClusterShapePlot()
{
  delete m_eventCountAxis;
}

void ClusterShapePlot::processEvent(const QVector<Hit*>& hits, const Particle* const, const SimpleEvent* const)
{
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    unsigned short elementId = (*it)->elementId();
    if (elementId == m_id) {
      Cluster* cluster = static_cast<Cluster*>(*it);
      const std::vector<Hit*>& subHits = cluster->hits();

      Setup* setup = Setup::instance();
      DetectorElement* element = setup->element(elementId);
      int iMax = element->sortedChannel(cluster->channel());

      const std::vector<Hit*>::const_iterator subEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator subIt = subHits.begin(); subIt != subEndIt; ++subIt) {
        // plot all hits, but shift by max
        Hit* subHit = *subIt;
        int i = element->sortedChannel(subHit->channel());
        histogram(0)->Fill(i - iMax, subHit->signalHeight());
        histogram(1)->Fill(i - iMax);
      }

      m_eventCounter++;
    }
  }
}

void ClusterShapePlot::draw(TCanvas* can)
{
  H1DPlot::draw(can);
  m_eventCountAxis->Draw();
}

void ClusterShapePlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("events = %1").arg(m_eventCounter)));
  double min = histogram(0)->GetMinimum();
  double max = 1.05*histogram(0)->GetMaximum();
  m_eventCountAxis->SetY1(min);
  m_eventCountAxis->SetY2(max);
  m_eventCountAxis->SetWmax((1.05/0.9)*m_eventCounter);
}

void ClusterShapePlot::finalize()
{
  int nBins = histogram(0)->GetNbinsX();
  for (int iBin = 1; iBin <= nBins; ++iBin) {
    int counts = histogram(1)->GetBinContent(iBin);
    if (counts > 0.01*m_eventCounter) {
      double binContent = histogram(0)->GetBinContent(iBin);
      histogram(0)->SetBinContent(iBin, binContent/counts);
    }
    else {
      histogram(0)->SetBinContent(iBin,0.);
    }
  }
  histogram(1)->Scale(.9*histogram(0)->GetBinContent(ceil(nBins/2.))/m_eventCounter);
}
