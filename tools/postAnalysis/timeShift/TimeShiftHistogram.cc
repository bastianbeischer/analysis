#include "TimeShiftHistogram.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TimeShiftHistogram::TimeShiftHistogram(TCanvas* canvas, int ch)
  : PostAnalysisPlot()
  , H1DPlot()
{
  QString title;
  TH1D* histogram = 0;
  TF1* function = 0;
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH1D")) {
      title = QString(canvas->GetName()).replace("canvas", QString("%1 histogram").arg(ch));
      if (!strcmp(canvas->GetListOfPrimitives()->At(i)->GetName(), qPrintable(title)))
        histogram = static_cast<TH1D*>(canvas->GetListOfPrimitives()->At(i)->Clone());
    }
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TF1")) {
      title = QString(canvas->GetName()).replace("canvas", QString("%1 function").arg(ch));
      if (!strcmp(canvas->GetListOfPrimitives()->At(i)->GetName(), qPrintable(title))) {
        function = static_cast<TF1*>(canvas->GetListOfPrimitives()->At(i)->Clone());
      }
    }
  }
  title = QString("%1 channel %2").arg(canvas->GetName()).arg(ch);
  setTitle(title);
  histogram->Smooth(5);
  addHistogram(histogram);
  addFunction(function);
}

TimeShiftHistogram::~TimeShiftHistogram()
{}
