#include <cstdlib>
#include <iostream>

#include <QCoreApplication>
#include <QThread>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include "PreAnalysisManager.hh"

// #include "DataChain.hh"
// #include "SimpleEvent.hh"
// #include "DataDescription.hh"

int main(int argc, char** argv)
{
  QCoreApplication app(argc, argv);

  if (argc != 2)
    qFatal("Pass a ROOT file or a list of run files as argument!");

  PreAnalysisManager manager;
  manager.processArgument(argv[1]);
  int nThreads = QThread::idealThreadCount();
  manager.start(nThreads);

  return app.exec();
}
