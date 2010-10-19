#include "Strategy.hh"

//#include <QString>
//#include <QStringList>
//#include <QFile>
//#include <QTextStream>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include "Constraint.hh"
#include "Parameters.hh"
#include "Manager.hh"
#include "millepede.h"

Strategy::Strategy() :
  m_constraints(0),
  m_nModules(0),
  m_nGlobal(0),
  m_nLocal(0),
  m_nStdDev(0),
  m_verbose(0),
  m_nIter(0),
  m_cutValue(0.),
  m_ladderResolutionS(-1.),
  m_ladderResolutionK(-1.),
  m_fiberResolution(-1.),
  m_nGlobalIterations(0)
{
  m_parameters = new Parameters();
}

Strategy::~Strategy()
{
  delete m_parameters;
}

void Strategy::DoInit()
{
  INITGL(m_nGlobal, m_nLocal, m_nStdDev, m_verbose);

  m_parameters->SetMillepedeParameters();

  for (unsigned int i = 0; i < m_constraints.size(); i++)
    m_constraints.at(i)->Activate();

  INITUN(m_nIter, m_cutValue);
}

// bool Strategy::ReadFromFile(QString fileName)
// {
//   QString directory = getenv("WORKDIR");
//   fileName = directory + "/alignment/strategies/" + fileName + ".txt";
//   QFile file(fileName);
//   if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//     std::cout << "Error opening file: " << qPrintable(fileName) << std::endl;
//     return false;
//   }

//   QTextStream in(&file);
//   while (!in.atEnd()) {
//     QString line = in.readLine();
//     QStringList list = line.split(QRegExp("\\s+"));
//     if (list.size() < 2)
//       continue;

//     QString parameterName = list[0];
//     QString value = list[1];
    
//     if (parameterName == "method") {
//       Manager::GetInstance()->ChangeMethodTo(value);
//     }
//     else if (parameterName == "nModules") {
//       m_nModules = value.toUInt();
//     }
//     else if (parameterName == "nGlobal") {
//       m_nGlobal = value.toUInt();
//       m_parameters->ReInitParameterArrays(m_nGlobal, 2*m_nModules);
//     }
//     else if (parameterName == "nLocal") {
//       m_nLocal = value.toUInt();
//     }
//     else if (parameterName == "verbose") {
//       m_verbose = value.toInt();
//     }
//     else if (parameterName == "nStdDev") {
//       m_nStdDev = value.toUInt();
//     }
//     else if (parameterName == "nLocalIter") {
//       m_nIter = value.toUInt();
//     }
//     else if (parameterName == "cutfactor") {
//       m_cutValue = value.toFloat();
//     }
//     else if (parameterName == "ladderResolutionS") {
//       m_ladderResolutionS = value.toFloat();
//     }
//     else if (parameterName == "ladderResolutionK") {
//       m_ladderResolutionK = value.toFloat();
//     }
//     else if (parameterName == "fiberResolution") {
//       m_fiberResolution = value.toFloat();
//     }
//     else if (parameterName == "nGlobalIter") {
//       m_nGlobalIterations = value.toUInt();
//     }
//     else if (parameterName == "angle") {
//       unsigned int module = list[1].toUInt();
//       float        angle  = list[2].toFloat();
//       m_parameters->SetAngle(module, angle);
//     }
//     else if (parameterName == "fixParameter") {
//       unsigned int iPar = value.toUInt();
//       m_parameters->SetParameterSigma(iPar, 0.);
//     }
//     else if (parameterName == "loadSimulationDataFrom") {
//       SimulationInterface* interface = Manager::GetInstance()->GetSimulationInterface();
//       interface->SetFileName(value);
//       interface->FillTracksFromFile();
//     }
//     else if (parameterName == "loadTestbeamDataFrom") {
//       TestbeamInterface* interface = Manager::GetInstance()->GetTestbeamInterface();
//       interface->SetFileName(value);
//       interface->FillTracksFromFile();
//     }
//   }

//   return true;
// }

bool Strategy::WriteToFile(QFile* /*file*/)
{
  return true;
}
