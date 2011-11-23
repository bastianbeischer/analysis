#include "ReconstructionMethodGraph.hh"
#include "AnalysisProcessor.hh"

#include <TQtWidget.h>
#include <TPad.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>

#include <QVBoxLayout>
#include <QComboBox>

ReconstructionMethodGraph::ReconstructionMethodGraph(QWidget* parent)
  : QWidget(parent)
  , m_widget(new TQtWidget)
  , m_methodComboBox(new QComboBox)
{
  Enums::ReconstructionMethodIterator end = Enums::reconstructionMethodEnd();
  for (Enums::ReconstructionMethodIterator it = Enums::reconstructionMethodBegin(); it != end; ++it)
    if (it.key() != Enums::UndefinedReconstructionMethod)
      m_methodComboBox->addItem(it.value());

  connect(m_methodComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(methodComboBoxIndexChanged(const QString&)));

  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_methodComboBox);
  layout->addWidget(m_widget);
  setLayout(layout);

  setMethod(Enums::Likelihood);
}

void ReconstructionMethodGraph::methodComboBoxIndexChanged(const QString& string)
{
  m_method = Enums::reconstructionMethod(string);
  emit methodChanged(m_method);
}

ReconstructionMethodGraph::~ReconstructionMethodGraph()
{
}

Enums::ReconstructionMethod ReconstructionMethodGraph::method() const
{
  return m_method;
}

void ReconstructionMethodGraph::setMethod(Enums::ReconstructionMethod method)
{
  m_methodComboBox->setCurrentIndex(m_methodComboBox->findText(Enums::label(method)));
}

void ReconstructionMethodGraph::draw(TMultiGraph* mg, TLegend* legend)
{
  TVirtualPad* previousPad = gPad;
  m_widget->cd();
  m_widget->GetCanvas()->Clear();
  mg->Draw("ALP");
  if (legend)
    legend->Draw("SAME");
  mg->GetYaxis()->SetRangeUser(-10., 100.);
  m_widget->GetCanvas()->Modified();
  m_widget->GetCanvas()->Update();
  previousPad->cd();
}
