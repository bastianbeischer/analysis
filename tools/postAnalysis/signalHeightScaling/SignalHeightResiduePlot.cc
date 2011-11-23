#include "SignalHeightResiduePlot.hh"

#include "PostAnalysisCanvas.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "Constants.hh"
#include "LocationSelectonWidget.hh"
#include "Helpers.hh"

#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TLatex.h>
#include <TFile.h>
#include <TPad.h>

#include <QDebug>
#include <QStringList>

SignalHeightResiduePlot::SignalHeightResiduePlot(Type type, TFile* file)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_type(type)
  , m_histograms()
{
  TH1D* histogram = 0;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      unsigned short id = element->id();
      QString plotName;
      if (m_type == Time) {
        setTitle("signal height residue plot time depentent");
        plotName = QString("signal height time correlation 0x%1 canvas").arg(id, 0, 16);
      } else if (m_type == Temperature) {
        setTitle("signal height residue plot temperature depentent");
        plotName = QString("signal height temperature correlation 0x%1 canvas").arg(id, 0, 16);
      }
      PostAnalysisCanvas* canvas = PostAnalysisCanvas::fromFile(file, plotName);
      TH2D* h2 = canvas->histograms2D().at(0);
      if (!histogram) {
        histogram = new TH1D(qPrintable(title()), "", 100, -1000., 1000.);
        setAxisTitle(h2->GetYaxis()->GetTitle(), "");
      }
      m_histograms.insert(id, h2);
      delete canvas;
      canvas = 0;
    }
  }
  LocationSelectonWidget* locationSelector = new LocationSelectonWidget;
  updateHistogram(histogram, Helpers::idealTrackerSignalHeight(locationSelector->situation()));
  addHistogram(histogram);

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.65, .85);
  latex->SetTitle(qPrintable(QString("n  = %1").arg(histogram->GetEntries())));
  addLatex(latex);

  latex = RootPlot::newLatex(.65, .82);
  latex->SetTitle(qPrintable(QString("mean  = %1").arg(histogram->GetMean())));
  addLatex(latex);

  latex = RootPlot::newLatex(.65, .79);
  latex->SetTitle(qPrintable(QString("rms  = %1").arg(histogram->GetRMS())));
  addLatex(latex);

  connect(locationSelector, SIGNAL(selectionChanged(Enums::Situation)), this, SLOT(updateLocation(Enums::Situation)));
  setSecondaryWidget(locationSelector);
}

SignalHeightResiduePlot::~SignalHeightResiduePlot()
{}

void SignalHeightResiduePlot::updateHistogram(TH1D* histogram, double referenceValue)
{
  if (histogram)
    histogram->Reset();
  foreach(unsigned short id, m_histograms.keys()) {
    TH2D* h2 = m_histograms[id];
    for (int binX = 0; binX < h2->GetNbinsX(); ++binX) {
      TH1* projectionHistogram = h2->ProjectionY("_py", binX + 1, binX + 1);
      if (projectionHistogram->GetEntries() > 30.) {
        TF1 function(qPrintable(QString(projectionHistogram->GetTitle()) + "Function"), "landau", projectionHistogram->GetXaxis()->GetXmin(), projectionHistogram->GetXaxis()->GetXmax());
        projectionHistogram->Fit(&function, "EQN0");
        double value = function.GetParameter(1);
        double residue = value - referenceValue;
        histogram->Fill(residue);
      }
      delete projectionHistogram;
    }
  }
}

void SignalHeightResiduePlot::updateLocation(Enums::Situation situation)
{
  updateHistogram(histogram(), Helpers::idealTrackerSignalHeight(situation));
  gPad->Modified();
  gPad->Update();
}

