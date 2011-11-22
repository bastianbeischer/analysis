#include "SignalHeightResiduePlot.hh"

#include "PostAnalysisCanvas.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "Constants.hh"

#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TLatex.h>
#include <TFile.h>

#include <QDebug>
#include <QStringList>

SignalHeightResiduePlot::SignalHeightResiduePlot(Type type, TFile* file)
  : PostAnalysisPlot()
  , H1DPlot()
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
      if (type == Time) {
        setTitle("signal height residue plot time depentent");
        plotName = QString("signal height time correlation 0x%1 canvas").arg(id, 0, 16);
      } else if (type == Temperature) {
        setTitle("signal height residue plot temperature depentent");
        plotName = QString("signal height temperature correlation 0x%1 canvas").arg(id, 0, 16);
      }
      PostAnalysisCanvas* canvas = PostAnalysisCanvas::fromFile(file, plotName);
      TH2D* h2 = canvas->histograms2D().at(0);
      if (!histogram) {
        histogram = new TH1D(qPrintable(title()), "", 100, -1000., 1000.);
        setAxisTitle(h2->GetYaxis()->GetTitle(), "");
      }
      for (int binX = 0; binX < h2->GetNbinsX(); ++binX) {
        TH1* projectionHistogram = h2->ProjectionY("_py", binX + 1, binX + 1);
        if (projectionHistogram->GetEntries() > 30.) {
          TF1 function(qPrintable(QString(projectionHistogram->GetTitle()) + "Function"), "landau", projectionHistogram->GetXaxis()->GetXmin(), projectionHistogram->GetXaxis()->GetXmax());
          projectionHistogram->Fit(&function, "EQN0");
          double value = function.GetParameter(1);
          double residue = value - Constants::idealTrackerSignalHeight;
          histogram->Fill(residue);
        }
      }
      delete canvas;
      canvas = 0;
    }
  }
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
}

SignalHeightResiduePlot::~SignalHeightResiduePlot()
{}

