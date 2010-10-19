#include "Manager.hh"

#include "Matrix.hh"
#include "Strategy.hh"
#include "Parameters.hh"
#include "MatrixStraightLine.hh"
#include "Track.hh"

#include "millepede.h"

#include <QFile>

#include <iostream>

Manager* Manager::m_instance = 0;

Manager::Manager() :
  m_matrix(0)
{
  m_strategy = new Strategy();
  m_parameters = m_strategy->parameters();

}

Manager::~Manager()
{
  foreach(Track* track, m_tracks)
    delete track;
  delete m_strategy;
  delete m_matrix;
}

Manager* Manager::instance()
{
  if (!m_instance) m_instance = new Manager();
  return m_instance;
}

bool Manager::loadStrategyFromFile(QString fileName)
{
  return m_strategy->readFromFile(fileName);
}

void Manager::startAlignment()
{
  if (!m_matrix) {
    std::cout << "Matrix has not been created!" << std::endl;
    return;
  }

  m_strategy->init();
  m_matrix->init();

  for (unsigned int iIteration = 1; iIteration <= m_strategy->numberOfGlobalIterations(); iIteration++) {
    m_strategy->init();

    // Track* track = m_tracks.at(0);
    // unsigned int nHits = track->hits().size();
    // float* z = new float[nHits];
    // float* rot = new float[2*nHits];
    // for (unsigned int i = 0; i < 2*nHits; i++)
    //   rot[i] = 0.;
    // for (unsigned int i = 0; i < track->hits().size(); i++) {
    //   Hit hit = track->hits().at(i);
    //   int uniqueLayer = hit.uniqueLayer();
    //   z[uniqueLayer] = hit.position().z();
    //   if (uniqueLayer != 0 && uniqueLayer != 1 /*&& uniqueLayer != 10*/ && uniqueLayer != 11)
    //     rot[uniqueLayer] = 1.0/(z[uniqueLayer] - z[0]);
    // }
    
    // float rhs;
    // CONSTF(rot, rhs = 0.);

    foreach(Track* track, m_tracks) {
      m_matrix->fillMatrixFromTrack(track);
      FITLOC();
    }

    FITGLO(m_parameters->parameterArray());
  }
}

bool Manager::writeResultsToFile(QString fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly))
    return false;

  if (!m_strategy->writeToFile(&file))
    return false;
  if (!m_parameters->writeToFile(&file))
    return false;

  file.close();

  return true;
}
