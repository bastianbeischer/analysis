#include "TimeBetweenEventsPlot.hh"
#include "SimpleEvent.hh"

#include <TH1D.h>
#include <TLatex.h>

#include <QDebug>

TimeBetweenEventsPlot::TimeBetweenEventsPlot(int numberOfThreads)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTracker)
  , H1DPlot()
  , m_numberOfThreads(numberOfThreads)
  , m_lastEventTime(-1)
{
  setTitle("distribution of time between events");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 500, 0, 400);
  setAxisTitle("#Delta t / ms", "entries");
  addHistogram(histogram, H1DPlot::P);
  if (m_numberOfThreads > 1) {
    addLatex(RootPlot::newLatex(.3, .85));
    latex()->SetTextColor(kRed);
    latex()->SetTitle(qPrintable(QString("This plot has to be filled by only one thread.")));
  }
}

TimeBetweenEventsPlot::~TimeBetweenEventsPlot()
{
}

void TimeBetweenEventsPlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  if (m_numberOfThreads != 1)
    return;
  double eventTime = event->time() * 1000;
  Q_ASSERT(eventTime > m_lastEventTime);
  if (m_lastEventTime > -1)
    histogram()->Fill(eventTime - m_lastEventTime);
  m_lastEventTime = eventTime;
}
