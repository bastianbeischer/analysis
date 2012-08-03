#include "MergedHistogram1D.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1D.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLatex.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>
#include <QVectorIterator>
#include <QCheckBox>

MergedHistogram1D::MergedHistogram1D(const QVector<PostAnalysisCanvas*>& canvases)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_checkBoxes()
  , m_canvases(canvases)
{
  Q_ASSERT(canvases.count());
  QString title = QString(canvases.first()->name());
  title = title.left(title.lastIndexOf("canvas ("));
  setTitle(title + "merged");

  foreach (TH1D* histogram, canvases.front()->histograms1D()) {
    histogram = static_cast<TH1D*>(histogram->Clone());
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

MergedHistogram1D::~MergedHistogram1D()
{
}

void MergedHistogram1D::update()
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
      QVector<TH1D*> histograms = canvas->histograms1D();
      QVectorIterator<TH1D*> histogramIt(histograms);
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

void MergedHistogram1D::draw(TCanvas* canvas)
{
  update();
  H1DPlot::draw(canvas);
}
