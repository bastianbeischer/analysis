#include "EventTimeDifferencePlot.hh"

#include "SettingsManager.hh"
#include "Settings.hh"
#include "SimpleEvent.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

#include <cmath>

EventTimeDifferencePlot::EventTimeDifferencePlot(int numberOfThreads)
  : AnalysisPlot(Enums::MiscellaneousTOF)
  , H1DPlot()
  , m_lastEventTime(-1)
  , m_active(numberOfThreads == 1)
{
  setTitle("event time difference");
}

EventTimeDifferencePlot::~EventTimeDifferencePlot()
{
}

void EventTimeDifferencePlot::processEvent(const AnalyzedEvent* event)
{
  if (numberOfHistograms() == 0) {
    unsigned int nBins = 200;
    double xMin = 0.;
    double xMax = 60000.;
    double fitMin = 15;
    double fitMax = 8000;
    double parameter = -1;
    double maxParameter = -1;
    const Settings* settings = event->settings();
    if (settings) {
      Enums::Situation situation = settings->situation();
      if (situation == Enums::KirunaFlight) {
        nBins = 3000;
        xMin = 0;
        xMax = 40000;
        fitMin = 15;
        fitMax = 300;
        parameter = 30.;
        maxParameter = 100;
      }
      if (situation == Enums::KirunaMuons) {
        nBins = 200;
        xMin = 0;
        xMax = 60000;
        fitMin = 250;
        fitMax = 8000;
        parameter = 1000.;
        maxParameter = 3000.;
      }
    }
    TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, xMin, xMax);
    histogram->Sumw2();
    setAxisTitle("#Deltat / ms", "probability density");
    addHistogram(histogram, H1DPlot::P);
    if (m_active) {
      const int nParams = 1;
      TF1* fit = new TF1("exponential distribution fit", this, &EventTimeDifferencePlot::exponentialDistribution, fitMin, fitMax, nParams, "EventTimeDifferencePlot", "exponentialDistribution");
      fit->SetLineColor(kGreen);
      if (parameter > 0 && maxParameter > 0) {
        fit->SetParameters(0, parameter);
        fit->SetParLimits(0, 1., maxParameter);
      }
      addFunction(fit);
      addLatex(RootPlot::newLatex(.4, .88));
      latex(0)->SetTitle("P(#Deltat) = #frac{1}{#tau} e^{#frac{- #Deltat}{#tau}}");
      addLatex(RootPlot::newLatex(.4, .81));
      addLatex(RootPlot::newLatex(.4, .74));
      addLatex(RootPlot::newLatex(.4, .67));
    } else {
      const int prevNumberOfLatexs = numberOfLatexs();
      addLatex(RootPlot::newLatex(.2, .55));
      latex(prevNumberOfLatexs)->SetTextColor(kRed);
      latex(prevNumberOfLatexs)->SetTitle("This plot has to be filled by only one thread.");
    }
  }
  if (!m_active)
    return;
  double eventTime = event->simpleEvent()->time() * 1000;
  if (m_lastEventTime > -1)
    histogram()->Fill(eventTime - m_lastEventTime);
  m_lastEventTime = eventTime;
}

void EventTimeDifferencePlot::update()
{
  if (!m_active)
    return;
  const double entries = histogram()->GetEntries();
  const double nUnderflow = histogram()->GetBinContent(0);
  const double nOverflow = histogram()->GetBinContent(histogram()->GetNbinsX() + 1);
  const double lowEdge = histogram()->GetBinLowEdge(1);
  const double upEdge = histogram()->GetBinLowEdge(histogram()->GetNbinsX() + 1);
  latex(2)->SetTitle(qPrintable(QString("%1 entries (%2 uflow, %3 oflow)").arg(entries).arg(nUnderflow).arg(nOverflow)));
  latex(3)->SetTitle(qPrintable(QString("range %1 ms to %2 ms").arg(lowEdge).arg(upEdge)));
}

void EventTimeDifferencePlot::finalize()
{
  if (!m_active)
    return;
  const double integral = histogram()->Integral(1, histogram()->GetNbinsX(), "width");
  histogram()->Scale(1. / integral);
  histogram()->SetBinContent(0, histogram()->GetBinContent(0) * integral);
  histogram()->SetBinContent(histogram()->GetNbinsX() + 1, histogram()->GetBinContent(histogram()->GetNbinsX() + 1) * integral);
  histogram()->Fit(function(),"EQRN0");
  double tau = function()->GetParameter(0);
  double tauError = function()->GetParError(0);
  latex(1)->SetTitle(qPrintable(QString("#tau / ms = %1 #pm %2").arg(tau, 0, 'g', 3).arg(tauError, 0, 'g', 3)));
}

double EventTimeDifferencePlot::exponentialDistribution(double* x, double* par)
{
  double deltaT = x[0];
  double tau = par[0];
  double f = 1. / tau * exp(-deltaT / tau);
  return f;
}
