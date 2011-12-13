#include "MainWindow.hh"

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : LikelihoodFitWindow(parent)
{
  setupAnalysis();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
}
