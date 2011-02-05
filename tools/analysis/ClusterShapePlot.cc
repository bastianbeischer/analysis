#include "ClusterShapePlot.hh"

#include <TH1D.h>
#include <TH1I.h>
#include <TLatex.h>
#include <TGaxis.h>
#include <TStyle.h>
#include <TPad.h>

#include "Hit.hh"
#include "Cluster.hh"

#include <QMap>
#include <QVector>

#include <cmath>
#include <iostream>

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
  histogram->GetXaxis()->SetTitle("strip no. relative to maximum");
  histogram->GetYaxis()->SetTitle("mean amplitude");
  histogram->GetYaxis()->SetTitleOffset(1.5);
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

void ClusterShapePlot::processEvent(const QVector<Hit*>& hits, Track*, SimpleEvent*)
{
  foreach(Hit* hit, hits) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    unsigned short element = cluster->detId() - cluster->channel();
    if (element == m_id) {
      std::vector<Hit*>& subHits = cluster->hits();

      // find max strip
      unsigned short iMax = 0;
      double max = 0.;

      QMap<double, Hit*> sortMap;// TODO: remove sorting after reprocessing with preAnalysis.
      for(unsigned short i = 0; i < subHits.size(); ++i) {
        sortMap[subHits.at(i)->position().x()] = subHits.at(i);
      }      

      QVector<Hit*> hits = sortMap.values().toVector();
      for(unsigned short i = 0; i < hits.size(); ++i) {
        double amplitude = hits.at(i)->signalHeight();
        if (amplitude > max) {
          max = amplitude;
          iMax = i;
        }
      }

      // plot all hits, but shift by max
      for (unsigned short i = 0; i < hits.size(); ++i) {
        histogram(0)->Fill(i - iMax, hits.at(i)->signalHeight());
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
  m_eventCountAxis->SetY1(gPad->GetUymin());
  m_eventCountAxis->SetY2(gPad->GetUymax());
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
