#ifndef TRDLIKELIHOODPLOT_HH
#define TRDLIKELIHOODPLOT_HH

class TRDElectronIdentifierLikelihood;
class TLine;
class RootQtWidget;

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDLikelihoodPlot: public AnalysisPlot, public H1DPlot
{
public:
    TRDLikelihoodPlot(Topic topic);
    virtual void processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event);
    virtual void update();
    virtual void finalize();
    virtual void draw(TCanvas* canvas);
private:
    bool truthMcIsElectron(SimpleEvent* event, bool& ok);
    int getBinEff(double effWanted, const TH1D* hist, double& eff, double& effErr);
    double getEff(const TH1D* hist, int upToBin, double& effErr);
    double getRej(const TH1D* hist, int upToBin, double& rejErr);
    void updateNonTRRejVsTREffHisto();
private:
    TH1D* m_NonTRHisto;
    TH1D* m_TRHisto;
    TH1D* m_NonTRRejVsTREff;

    TLine* m_line50;
    TLine* m_line90;

    TRDElectronIdentifierLikelihood* m_EIdentifierLH;
    RootQtWidget* m_NonTRRejVsTREffWidget;

};

#endif // TRDLIKELIHOODPLOT_HH
