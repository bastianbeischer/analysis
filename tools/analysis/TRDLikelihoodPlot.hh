#ifndef TRDLIKELIHOODPLOT_HH
#define TRDLIKELIHOODPLOT_HH

class TRDElectronIdentifierLikelihood;

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDLikelihoodPlot: public AnalysisPlot, public H1DPlot
{
public:
    TRDLikelihoodPlot(Topic topic);
    virtual void processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event);
    virtual void finalize();
private:
    bool truthMcIsElectron(SimpleEvent* event);
private:
    TH1D* m_NonTRHisto;
    TH1D* m_TRHisto;

    TRDElectronIdentifierLikelihood* m_EIdentifierLH;

};

#endif // TRDLIKELIHOODPLOT_HH
