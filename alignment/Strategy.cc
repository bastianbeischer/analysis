#include "Strategy.hh"

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include "DataInterface.hh"
#include "Layer.hh"
#include "DetectorElement.hh"
#include "Constraint.hh"
#include "Parameters.hh"
#include "Manager.hh"
#include "millepede.h"
#include "Setup.hh"

Strategy::Strategy() :
  m_parameters(new Parameters),
  m_constraints(0),
  m_dataInterface(new DataInterface),
  m_nGlobal(0),
  m_nLocal(0),
  m_nStdDev(0),
  m_verbose(0),
  m_nIter(0),
  m_cutValue(0.),
  m_fiberResolution(-1.),
  m_nGlobalIterations(0)
{
}

Strategy::~Strategy()
{
  delete m_parameters;
  foreach(Constraint* cons, m_constraints) 
    delete cons;
  delete m_dataInterface;
}

void Strategy::init()
{
  INITGL(m_nGlobal, m_nLocal, m_nStdDev, m_verbose);

  m_parameters->setMillepedeParameters();

  foreach(Constraint* cons, m_constraints) 
    cons->activate();

  INITUN(m_nIter, m_cutValue);
}

bool Strategy::readFromFile(QString fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cout << "Error opening file: " << qPrintable(fileName) << std::endl;
    return false;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    if (line.startsWith("#"))
      continue;

    QStringList list = line.split(QRegExp("\\s+"));
    if (list.size() < 2)
      continue;

    QString parameterName = list[0];
    QString value = list[1];
    
    if (parameterName == "nGlobal") {
      m_nGlobal = value.toUInt();
      m_parameters->reInitParameterArrays(m_nGlobal);
    }
    else if (parameterName == "nLocal") {
      m_nLocal = value.toUInt();
    }
    else if (parameterName == "verbose") {
      m_verbose = value.toInt();
    }
    else if (parameterName == "nStdDev") {
      m_nStdDev = value.toUInt();
    }
    else if (parameterName == "nLocalIter") {
      m_nIter = value.toUInt();
    }
    else if (parameterName == "cutfactor") {
      m_cutValue = value.toFloat();
    }
    else if (parameterName == "fiberResolution") {
      m_fiberResolution = value.toFloat();
    }
    else if (parameterName == "nGlobalIter") {
      m_nGlobalIterations = value.toUInt();
    }
    else if (parameterName == "fixParameter") {
      unsigned int iPar = value.toUInt();
      m_parameters->setParameterSigma(iPar, 0.);
    }
    else if (parameterName == "fixDetector") {
      unsigned short detId = value.toUShort();
      fixDetector(detId);
    }
    else if (parameterName == "fixLayer") {
      unsigned short layer = value.toUShort();
      fixLayer(layer);
    }
    else if (parameterName == "readDataFrom") {
      m_dataInterface->addFiles(qPrintable(value));
    }
  }

  return true;
}

void Strategy::fixDetector(unsigned short detId)
{
  unsigned int index = m_parameters->indexForDetId(detId);
  m_parameters->setParameterSigma(index, 0.);
}

void Strategy::fixLayer(unsigned short layerNumber)
{
  Setup* setup = Setup::instance();
  unsigned int i = 1;
  Layer* layer = setup->firstLayer();
  while(layer != 0 && i != layerNumber) {
    i++;
    layer = setup->nextLayer();
  }
  
  std::cout << "fixing layer: " << i << " at " << layer->z() << " with detIds: ";

  foreach(DetectorElement* element, layer->elements()) {
    unsigned short detId = element->id();
    unsigned int index = m_parameters->indexForDetId(detId);
    std::cout << std::hex << detId << std::dec <<  "(" << index << ") ";
    m_parameters->setParameterSigma(index, 0.);
  }
  std::cout << std::endl;
}
