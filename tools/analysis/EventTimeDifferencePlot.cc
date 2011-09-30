#include "EventTimeDifferencePlot.hh"
#include "SimpleEvent.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

#include <cmath>

EventTimeDifferencePlot::EventTimeDifferencePlot(int numberOfThreads)
  : AnalysisPlot(AnalysisTopic::MiscellaneousTOF)
  , H1DPlot()
  , m_lastEventTime(-1)
  , m_active(numberOfThreads == 1)
{
  setTitle("event time difference");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 400, 0, 400);
  histogram->Sumw2();
  setAxisTitle("#Deltat / ms", "");
  addHistogram(histogram, H1DPlot::P);
  if (m_active) {
    const int nParams = 1;
    const double fitMin = 15;
    const double fitMax = 250;
    TF1* fit = new TF1("exponential distribution fit", this, &EventTimeDifferencePlot::exponentialDistribution,
      fitMin, fitMax, nParams, "EventTimeDifferencePlot", "exponentialDistribution");
    fit->SetLineColor(kGreen);
    fit->SetParameters(0, 30);
    fit->SetParLimits(0, 1., 100.);
    addFunction(fit);
    addLatex(RootPlot::newLatex(.4, .88));
    latex(0)->SetTitle("P(#Deltat) = #frac{1}{#tau} e^{#frac{- #Deltat}{#tau}}");
    addLatex(RootPlot::newLatex(.4, .81));
    addLatex(RootPlot::newLatex(.4, .74));
  } else { 
    const int prevNumberOfLatexs = numberOfLatexs();
    addLatex(RootPlot::newLatex(.2, .55));
    latex(prevNumberOfLatexs)->SetTextColor(kRed);
    latex(prevNumberOfLatexs)->SetTitle("This plot has to be filled by only one thread.");
  }
}

EventTimeDifferencePlot::~EventTimeDifferencePlot()
{
}

void EventTimeDifferencePlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  if (!m_active)
    return;
  double eventTime = event->time() * 1000;
  if (m_lastEventTime > -1)
    histogram()->Fill(eventTime - m_lastEventTime);
  m_lastEventTime = eventTime;
}

void EventTimeDifferencePlot::update()
{
  if (!m_active)
    return;
  double entries = histogram()->GetEntries();
  double nUnderflow = histogram()->GetBinContent(0);
  double nOverflow = histogram()->GetBinContent(histogram()->GetNbinsX() + 1);
  latex(2)->SetTitle(qPrintable(QString("%1 entries (%2 uflow, %3 oflow)").arg(entries).arg(nUnderflow).arg(nOverflow)));
}

void EventTimeDifferencePlot::finalize()
{
  if (!m_active)
    return;
  const double integral = histogram()->Integral("width");
  histogram()->Scale(1. / integral);
  histogram()->SetBinContent(0, histogram()->GetBinContent(0) * integral);
  histogram()->SetBinContent(histogram()->GetNbinsX() + 1, histogram()->GetBinContent(histogram()->GetNbinsX() + 1) * integral);
  histogram()->Fit(function(),"EQRN0");
  double tau = function()->GetParameter(0);
  double tauError = function()->GetParError(0);
  latex(1)->SetTitle(qPrintable(QString("#tau = %1 #pm %2").arg(tau, 0, 'g', 3).arg(tauError, 0, 'g', 3)));
}

double EventTimeDifferencePlot::exponentialDistribution(double* x, double* par)
{
  double deltaT = x[0];
  double tau = par[0];
  double f = 1. / tau * exp(-deltaT / tau);
  return f;
}
