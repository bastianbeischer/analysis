#ifndef BendingAnglePlot_hh
#define BendingAnglePlot_hh

#include "H1DPlot.hh"

class BendingAnglePlot : public H1DPlot {
  public:
    BendingAnglePlot();
    virtual ~BendingAnglePlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
    virtual void finalize();
		virtual void draw(TCanvas*) const;
	private:
		TH1* m_insideMagnetHistogram;
		TH1* m_outsideMagnetHistogram;
};

#endif
