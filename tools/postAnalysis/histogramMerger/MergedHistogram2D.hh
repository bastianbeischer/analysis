#ifndef MergedHistogram2D_hh
#define MergedHistogram2D_hh

#include <QObject>
#include <QStringList>

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class PostAnalysisCanvas;
class QCheckBox;
class TCanvas;

class MergedHistogram2D : public QObject, public PostAnalysisPlot, public H2DPlot {
Q_OBJECT
public:
  MergedHistogram2D(const QVector<PostAnalysisCanvas*>&);
  virtual ~MergedHistogram2D();
  virtual void draw(TCanvas*);
public slots:
  virtual void update();
private:
  QVector<QCheckBox*> m_checkBoxes;
  QVector<PostAnalysisCanvas*> m_canvases;
};

#endif
