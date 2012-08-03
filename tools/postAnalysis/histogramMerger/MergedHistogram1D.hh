#ifndef MergedHistogram1D_hh
#define MergedHistogram1D_hh

#include <QObject>
#include <QStringList>

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class QCheckBox;
class TCanvas;

class MergedHistogram1D : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  MergedHistogram1D(const QVector<PostAnalysisCanvas*>&);
  virtual ~MergedHistogram1D();
  virtual void draw(TCanvas*);
public slots:
  virtual void update();
private:
  QVector<QCheckBox*> m_checkBoxes;
  QVector<PostAnalysisCanvas*> m_canvases;
};

#endif
