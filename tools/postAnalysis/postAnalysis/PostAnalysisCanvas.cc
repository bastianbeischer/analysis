#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TFile.h>
#include <TKey.h>

#include <QDebug>

PostAnalysisCanvas::PostAnalysisCanvas(TCanvas* canvas)
  : m_palette(RootStyle::DefaultPalette)
  , m_file(0)
  , m_name(canvas->GetName())
  , m_canvas(canvas)
{
}

PostAnalysisCanvas::PostAnalysisCanvas()
  : m_palette(RootStyle::DefaultPalette)
  , m_file(0)
  , m_name()
  , m_canvas(0)
{
}

PostAnalysisCanvas::PostAnalysisCanvas(TFile* file, const QString& name)
  : m_palette(RootStyle::DefaultPalette)
  , m_file(file)
  , m_name(name)
  , m_canvas(0)
{
}

PostAnalysisCanvas::~PostAnalysisCanvas()
{
}

QStringList PostAnalysisCanvas::savedCanvases(TFile* file, const QRegExp& regularExpression)
{
  TList* rootList = file->GetListOfKeys();
  QStringList list;
  for (int i = 0; i < rootList->GetSize(); ++i) {
    TKey* key = static_cast<TKey*>(rootList->At(i));
    QString name(key->GetName());
    if (name.contains(regularExpression))
      list.append(name);
  }
  QMap<QString, int> duplicates;
  QStringList::ConstIterator begin = list.begin();
  QStringList::ConstIterator end = list.end();
  for (QStringList::ConstIterator i = begin; i != end; ++i) {
    for (QStringList::ConstIterator j = i + 1; j != end; ++j) {
      if (*i == *j) {
        QMap<QString, int>::Iterator position = duplicates.find(*i);
        if (position == duplicates.end()) {
          duplicates.insert(*i, 2);
        } else {
          ++position.value();
        }
        break;
      }
    }
  }
  QMapIterator<QString, int> duplicatesIterator(duplicates);
  while (duplicatesIterator.hasNext()) {
    duplicatesIterator.next();
    qWarning() << "Duplicates found:" << duplicatesIterator.key() << "occurs" << duplicatesIterator.value() << "times!";
  }
  list.removeDuplicates();
  return list;
}

PostAnalysisCanvas* PostAnalysisCanvas::fromFile(TFile* file, const QString& name)
{
  TCanvas* canvas = loadCanvas(file, name);
  if (canvas)
    return new PostAnalysisCanvas(canvas);
  return 0;
}

TCanvas* PostAnalysisCanvas::canvas()
{
  if (!m_canvas)
    loadCanvas();
  return m_canvas;
}

TCanvas* PostAnalysisCanvas::loadCanvas(TFile* file, const QString& name)
{
  Q_ASSERT(file);
  Q_ASSERT(!name.isEmpty());
  QString key = name;
  key.replace(QRegExp("canvas \\(.*\\)$"), "canvas");
  TObject* object = file->Get(qPrintable(key));
  if (!object) {
    qDebug() << key << "does not exist in" << file->GetEndpointUrl()->GetFile();
    return 0;
  }
  if (strcmp(object->ClassName(), "TCanvas")) {
    qDebug() << key << "has type" << object->ClassName() << "not TCanvas!";
    return 0;
  }
  return static_cast<TCanvas*>(object->Clone());
}

void PostAnalysisCanvas::loadCanvas()
{
  Q_ASSERT(m_file);
  Q_ASSERT(!m_name.isEmpty());
  m_canvas = loadCanvas(m_file, m_name);
  Q_ASSERT(m_canvas);
}

void PostAnalysisCanvas::draw(TCanvas* canvas)
{
  if (!m_canvas)
    loadCanvas();
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
  return m_name;
}

QVector<TH1D*> PostAnalysisCanvas::histograms1D()
{
  if (!m_canvas)
    loadCanvas();
  QVector<TH1D*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH1D")) {
      ret.append(static_cast<TH1D*>(m_canvas->GetListOfPrimitives()->At(i)));
    }
  }
  return ret;
}

QVector<TH2D*> PostAnalysisCanvas::histograms2D()
{
  if (!m_canvas)
    loadCanvas();
  QVector<TH2D*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i)
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      ret.append(static_cast<TH2D*>(m_canvas->GetListOfPrimitives()->At(i)));
  return ret;
}

QVector<TH3D*> PostAnalysisCanvas::histograms3D()
{
  if (!m_canvas)
    loadCanvas();
  QVector<TH3D*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i)
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH3D"))
      ret.append(static_cast<TH3D*>(m_canvas->GetListOfPrimitives()->At(i)));
  return ret;
}

QVector<TF1*> PostAnalysisCanvas::functions()
{
  if (!m_canvas)
    loadCanvas();
  QVector<TF1*> ret;
  for (int i = 0; i < m_canvas->GetListOfPrimitives()->GetSize(); ++i)
    if (!strcmp(m_canvas->GetListOfPrimitives()->At(i)->ClassName(), "TF1"))
      ret.append(static_cast<TF1*>(m_canvas->GetListOfPrimitives()->At(i)));
  return ret;
}
