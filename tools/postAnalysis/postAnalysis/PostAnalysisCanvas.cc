#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TFile.h>
#include <THStack.h>

#include <QDebug>

PostAnalysisCanvas::PostAnalysisCanvas(TFile* file, const QString& name)
  : m_palette(RootStyle::DefaultPalette)
  , m_canvas(static_cast<TCanvas*>(file->Get(qPrintable(name))->Clone()))
{
}

PostAnalysisCanvas::~PostAnalysisCanvas()
{}

void PostAnalysisCanvas::draw(TCanvas* canvas)
{
  canvas->cd();
  RootStyle::setPalette(m_palette);
  m_canvas->DrawClonePad();
}
  
void PostAnalysisCanvas::setPalette(RootStyle::PaletteType palette)
{
  m_palette = palette;
}
  
QString PostAnalysisCanvas::name()
{
  return m_canvas->GetName();
}
  
QVector<TH2D*> PostAnalysisCanvas::histograms2D()
{
  QVector<TH2D*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i)
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      ret.append(static_cast<TH2D*>(m_canvas->GetListOfPrimitives()->At(i)));
  return ret;
}

QVector<TH1D*> PostAnalysisCanvas::histograms1D()
{
  QVector<TH1D*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH1D")) {
      ret.append(static_cast<TH1D*>(m_canvas->GetListOfPrimitives()->At(i)));
    }
  }
  return ret;
}

QVector<TF1*> PostAnalysisCanvas::functions()
{
  QVector<TF1*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i)
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TF1"))
      ret.append(static_cast<TF1*>(m_canvas->GetListOfPrimitives()->At(i)));
  return ret;
}
