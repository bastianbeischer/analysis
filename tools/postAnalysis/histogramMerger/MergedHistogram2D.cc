#include "MergedHistogram2D.hh"
#include "PostAnalysisCanvas.hh"

#include <TH2D.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLatex.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>
#include <QVectorIterator>
#include <QCheckBox>

MergedHistogram2D::MergedHistogram2D(const QVector<PostAnalysisCanvas*>& canvases)
  : PostAnalysisPlot()
  , H2DPlot()
  , m_checkBoxes()
  , m_canvases(canvases)
{
  Q_ASSERT(canvases.count());
  QString title = QString(canvases.first()->name());
  title = title.left(title.lastIndexOf("canvas ("));
  setTitle(title + "merged");

  foreach (TH2D* histogram, canvases.front()->histograms2D()) {
    histogram = static_cast<TH2D*>(histogram->Clone());
    histogram->Reset();
    addHistogram(histogram);
  }
  TLatex* latex = new TLatex(0.1, 0.95, 0);
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.02);
  addLatex(latex);

  QVectorIterator<PostAnalysisCanvas*> canvasIt(canvases);
  QHBoxLayout* layout = new QHBoxLayout();
  while (canvasIt.hasNext()) {
    QString fileName = canvasIt.next()->name();
    fileName.remove(0, fileName.lastIndexOf("canvas (") + 8);
    fileName.chop(1);
    QCheckBox* checkBox = new QCheckBox(fileName);
    checkBox->setCheckState(Qt::Checked);
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(update()));
    m_checkBoxes.append(checkBox);
    layout->addWidget(checkBox);
  }
  layout->addStretch();
  QWidget* widget = new QWidget;
  widget->setLayout(layout);
  setSecondaryWidget(widget);
}

MergedHistogram2D::~MergedHistogram2D()
{
}

void MergedHistogram2D::update()
{
  histogram()->Reset();
  QVectorIterator<QCheckBox*> checkBoxIt(m_checkBoxes);
  QVectorIterator<PostAnalysisCanvas*> canvasIt(m_canvases);
  QString title;
  while (checkBoxIt.hasNext() && canvasIt.hasNext()) {
    PostAnalysisCanvas* canvas = canvasIt.next();
    QCheckBox* checkBox = checkBoxIt.next();
    if (checkBox->checkState() == Qt::Checked) {
      title+= checkBox->text() + ", ";
      QVector<TH2D*> histograms = canvas->histograms2D();
      QVectorIterator<TH2D*> histogramIt(histograms);
      for (int i = 0; i < numberOfHistograms(); ++i) {
        Q_ASSERT(histogramIt.hasNext());
        histogram(i)->Add(histogramIt.next());
      }
    }
  }
  title.chop(2);
  latex()->SetTitle(qPrintable(title));
  Q_ASSERT(!checkBoxIt.hasNext() && !canvasIt.hasNext());
  gPad->Modified();
  gPad->Update();
}

void MergedHistogram2D::draw(TCanvas* canvas)
{
  update();
  H2DPlot::draw(canvas);
}
