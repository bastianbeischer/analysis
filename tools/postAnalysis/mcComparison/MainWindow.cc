#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "PostAnalysisWindow.hh"
#include "RootStyle.hh"
#include "HistCompare.hh"
#include "HistResiduals.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TKey.h>

#include <QString>
#include <QDebug>
#include <QFileDialog>

#include <iostream>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setAnalysisFile(const QString& file)
{
  m_dataFile = file;
}

void MainWindow::setMCFile(const QString& file)
{
  m_mcFile = file;
}

void MainWindow::SetOldMCFile(const QString& file)
{
  m_oldmcFile = file;
}

void MainWindow::setupAnalysis()
{
  if( m_oldmcFile.size() == 0)
  {
    TFile dataFile(qPrintable(m_dataFile));
    TFile mcFile(qPrintable(m_mcFile));

    gROOT->cd();

    PostAnalysisCanvas* dataCanvas = addCanvas(&dataFile,"complete TRD spectrum (-100 GeV to 100 GeV) canvas");
    PostAnalysisCanvas* mcCanvas = addCanvas(&mcFile, "complete TRD spectrum (-100 GeV to 100 GeV) canvas");
    qDebug() << "dataCanvas " << dataCanvas->name() << " got " << dataCanvas->histograms1D().size() << " 1D histos";
    qDebug() << "mcCanvas " << mcCanvas->name() << " got " << mcCanvas->histograms1D().size() << " 1D histos";

    addPlot(new HistCompare(dataCanvas, mcCanvas));
    addPlot(new HistResiduals(dataCanvas, mcCanvas));

    dataFile.Close();
    mcFile.Close();
  }
  else
  {
    TFile dataFile(qPrintable(m_dataFile));
    gROOT->cd();
    TFile mcFile(qPrintable(m_mcFile));
    gROOT->cd();
    TFile oldmcFile(qPrintable(m_oldmcFile));
    gROOT->cd();

    //    for (int i = 0; i < mcFile.GetListOfKeys()->GetSize(); ++i)
    //    std::cout << ((TKey*)mcFile.GetListOfKeys()->At(i))->ReadObj()->GetName() << std::endl;

    QString dataname = QString("complete TRD spectrum (-100 GeV to 100 GeV) canvas");
    QString oldmcname = QString("old_mc_canvas");

    PostAnalysisCanvas* dataCanvas = addCanvas(&dataFile,"Canvas 1");
    //qPrintable(dataname));
    PostAnalysisCanvas* mcCanvas = addCanvas(&mcFile, "Canvas 1");
    //qPrintable(dataname) );
    PostAnalysisCanvas* oldmcCanvas = addCanvas(&oldmcFile, qPrintable(oldmcname) );

    qDebug() << "dataCanvas " << dataCanvas->name() << " got " << dataCanvas->histograms1D().size() << " 1D histos";
    qDebug() << "mcCanvas " << mcCanvas->name() << " got " << mcCanvas->histograms1D().size() << " 1D histos";
    qDebug() << "oldmcCanvas " << oldmcCanvas->name() << "got" << mcCanvas->histograms1D().size() << "1D histos";

    addPlot(new HistCompare(dataCanvas, mcCanvas, oldmcCanvas) );
    addPlot(new HistResiduals(dataCanvas, mcCanvas) );
    addPlot(new HistResiduals(dataCanvas, oldmcCanvas) );

    dataFile.Close();
    mcFile.Close();
    oldmcFile.Close();
  }
}

