#include "LikelihoodFitWindow.hh"
#include "LikelihoodPDFFitPlot.hh"
#include "Setup.hh"

#include <QDebug>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>

LikelihoodFitWindow::LikelihoodFitWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_particles(Setup::instance()->proposedParticles())
  , m_fitPlots()
  , m_comboBox(new QComboBox)
{
  m_comboBox->addItem("all (bad + good + low statistics)");
  m_comboBox->addItem("bad");
  m_comboBox->addItem("good");
  m_comboBox->addItem("low statistics");
  m_comboBox->addItem("bad + good");
  m_comboBox->addItem("bad + low statistics");
  m_comboBox->addItem("good + low statistics");
  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(plotSelectionChanged()));
  addWidget(m_comboBox);

  QPushButton* button = 0;
  button = new QPushButton("save");
  connect(button, SIGNAL(clicked()), this, SLOT(save()));
  addWidget(button);

  button = new QPushButton("normalize all");
  connect(button, SIGNAL(clicked()), this, SLOT(normalizeAll()));
  addWidget(button);

  button = new QPushButton("fit all");
  connect(button, SIGNAL(clicked()), this, SLOT(fitAll()));
  addWidget(button);
}

LikelihoodFitWindow::~LikelihoodFitWindow()
{
}

void LikelihoodFitWindow::configFileChanged()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    plot->update();
}

void LikelihoodFitWindow::fitAll()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    plot->fit();
}

void LikelihoodFitWindow::normalizeAll()
{
  qDebug() << "TODO: void LikelihoodFitWindow::normalizeAll()";
}

void LikelihoodFitWindow::save()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    if (passes(plot))
      plot->save();
}

bool LikelihoodFitWindow::passes(const LikelihoodPDFFitPlot* plot)
{
  const QString& text = m_comboBox->currentText();
  return (plot->bad() && text.contains("bad")) || (plot->good() && text.contains("good"))
    || (plot->lowStatistics() && text.contains("low statistics"));
}

void LikelihoodFitWindow::plotSelectionChanged()
{
  clearPlots();
  foreach (PostAnalysisPlot* plot, m_otherPlots)
    addPlot(plot);
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    if (passes(plot))
      addPlot(plot);
}
