#include <cstdlib>
#include <iostream>

#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include <SimpleEvent.hh>
#include <DataDescription.hh>
#include <Hit.hh>

int main(int argc, char** argv)
{
  if (argc != 2)
    qFatal("Pass a run file list file!");
  QFile runFileListFile(argv[1]);
  if (!runFileListFile.open(QIODevice::ReadOnly | QIODevice::Text))
    qFatal("Could not open run file list file.");
	
  TFile destinationTreeFile(qPrintable(QString(argv[1])+".root"), "RECREATE");
  TTree destinationTree("SimpleEventTree", "tree with simple events");
  SimpleEvent* destinationEvent = 0;
  destinationTree.Branch("event", "SimpleEvent", &destinationEvent);
  DataDescription description;
	
  QTextStream inStream(&runFileListFile);
  QString line = inStream.readLine();
  while (!line.isNull()) {
    qDebug() << line;
    description.addRunFile(line.toStdString());
    gROOT->cd();
    TFile sourceTreeFile(qPrintable(line));
    if (sourceTreeFile.IsZombie())
      qFatal("Could not open run file.");
    TTree* sourceTree = static_cast<TTree*>(sourceTreeFile.Get("SimpleEventTree"));
    SimpleEvent* sourceEvent = 0;
    sourceTree->SetBranchAddress("event", &sourceEvent);
    for (long eventIt = 0; eventIt < sourceTree->GetEntries(); ++eventIt) {
      sourceTree->GetEntry(eventIt);



      destinationEvent = sourceEvent;
      destinationTree.Fill();
    }
    line = inStream.readLine();
    sourceTreeFile.Close();
  }
  destinationTree.GetUserInfo()->Add(&description);
  destinationTreeFile.cd();
  destinationTree.Write();
  destinationTreeFile.Close();

  return EXIT_SUCCESS;
}
