#include "Plotter.hh"

#include <TApplication.h>
#include <TStyle.h>
#include <TROOT.h>

#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Please give a filename to open. Should be a list of root files. Example: residuals all_runs.txt" << std::endl;
    return -1;
  }
  
  const char* filename = argv[1];

  TApplication app("app", &argc, argv);
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1,0);
  gStyle->SetOptFit(111);

  Plotter plotter;
  plotter.addFiles(filename);
  plotter.process();

  return 0;
}
