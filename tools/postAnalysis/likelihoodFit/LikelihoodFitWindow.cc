#include "LikelihoodFitWindow.hh"
#include "LikelihoodPDFFitPlot.hh"
#include "Setup.hh"

#include <QDebug>
#include <QPushButton>
#include <QWidget>

LikelihoodFitWindow::LikelihoodFitWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_particles(Setup::instance()->proposedParticles())
  , m_fitPlots()
{
  QPushButton* button = 0;

  button = new QPushButton("save good");
  connect(button, SIGNAL(clicked()), this, SLOT(saveGood()));
  addWidget(button);

  button = new QPushButton("save all");
  connect(button, SIGNAL(clicked()), this, SLOT(saveAll()));
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
  qDebug() << "void LikelihoodFitWindow::normalizeAll()";
}

void LikelihoodFitWindow::saveAll()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    plot->save();
}

void LikelihoodFitWindow::saveGood()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    if (plot->goodFit())
      plot->save();
}
