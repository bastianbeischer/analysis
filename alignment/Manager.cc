#include "Manager.hh"

#include "Matrix.hh"
#include "Strategy.hh"
#include "Parameters.hh"
#include "Matrix.hh"
#include "Track.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include "millepede.h"

#include <QFile>

#include <iostream>

Manager* Manager::m_instance = 0;

Manager::Manager() :
  m_strategy(new Strategy),
  m_matrix(new Matrix),
  m_parameters(m_strategy->parameters())
{
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
  m_strategy->init();
  m_matrix->init();

  for (unsigned int iIteration = 1; iIteration <= m_strategy->numberOfGlobalIterations(); iIteration++) {
    m_strategy->init();

    foreach(Track* track, m_tracks) {
      m_matrix->fillMatrixFromTrack(track);
      FITLOC();
    }

    FITGLO(m_parameters->parameterArray());
  }
}

void Manager::saveResults() const
{
  Setup* setup = Setup::instance();

  DetectorElement* element = setup->firstElement();
  while(element) {
    unsigned short detId = element->id();
    unsigned int index = m_parameters->indexForDetId(detId);
    float shift = m_parameters->parameter(index);
    element->setAlignmentShift(shift);
    element = setup->nextElement();
  }

  setup->writeSettings();
}
