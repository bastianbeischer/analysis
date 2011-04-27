#include "Manager.hh"

#include "DataInterface.hh"
#include "AlignmentMatrix.hh"
#include "Strategy.hh"
#include "Parameters.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include "millepede.h"

#include <QFile>

#include <iostream>

Manager* Manager::m_instance = 0;

Manager::Manager() :
  m_strategy(new Strategy),
  m_matrix(new AlignmentMatrix),
  m_parameters(m_strategy->parameters()),
  m_dataInterface(m_strategy->dataInterface())
{
}

Manager::~Manager()
{
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
  m_matrix->init();

  for (unsigned int iIteration = 1; iIteration <= m_strategy->numberOfGlobalIterations(); iIteration++) {
    m_strategy->init();
    m_dataInterface->process(m_matrix);
    FITGLO(m_parameters->parameterArray());
  }
}

void Manager::saveResults() const
{
  Setup* setup = Setup::instance();
  const ElementIterator endIt = setup->lastElement();
  for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
    DetectorElement* element = *it;
    unsigned short detId = element->id();
    unsigned int index = m_parameters->indexForDetId(detId);
    float shift = m_parameters->parameter(index);
    float sigma = m_parameters->parameterSigma(index);
    if (sigma != 0) {
      double oldShift = element->alignmentShift();
      element->setAlignmentShift(oldShift + shift);
    }
  }

  setup->writeSettings();
}
