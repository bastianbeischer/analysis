#include "LikelihoodFitWindow.hh"
#include "Setup.hh"

#include <QDebug>

LikelihoodFitWindow::LikelihoodFitWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_particles(Setup::instance()->proposedParticles())
{
}

LikelihoodFitWindow::~LikelihoodFitWindow()
{
}
