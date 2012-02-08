#include "PostAnalysisCanvas.hh"
#include "RootStyle.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TKey.h>
#include <TText.h>

#include <cmath>

#include <QDebug>
#include <QtGlobal>
#include <QFile>
#include <QStringList>

bool verbose = false;

void compareKeys(const QStringList& keys1, const QStringList& keys2,
      QStringList& common, QStringList& missing1, QStringList& missing2)
{
  common.clear();
  missing1.clear();
  missing2.clear();

  foreach (QString s, keys1) {
    if (!keys2.contains(s)) {
      missing2 << s;
    } else {
      common << s;
    }
  }
  foreach (QString s, keys2) {
    if (!keys1.contains(s)) {
      missing1 << s;
    }
  }
}

bool histogramsMatch(TH1D* h1, TH1D* h2, QStringList& comment)
{
  bool ret = true;
  if (strcmp(h1->GetName(), h2->GetName())) {
    comment << QString("Name mismatch: %1 <-> %2").arg(h1->GetName()).arg(h2->GetName());
    ret = false;
  }
  if (h1->GetEntries() != h2->GetEntries()) {
    if (verbose)
      comment << QString("Number of entries mismatch: %1 <-> %2")
        .arg(h1->GetEntries()).arg(h2->GetEntries());
    ret = false;
  }
  int nBins = h1->GetXaxis()->GetNbins();
  for (int bin = 0; bin <= nBins; ++bin)
    if (!qFuzzyCompare(h1->GetBinContent(bin), h2->GetBinContent(bin))
      && !(std::isnan(h1->GetBinContent(bin)) && std::isnan(h1->GetBinContent(bin)))) {
      if (verbose)
        comment << QString("bin %1: first file = %2, second file = %3").arg(bin).arg(h1->GetBinContent(bin)).arg(h2->GetBinContent(bin));
      ret = false;
    }
  return ret;
}

bool histogramsMatch(TH2D* h1, TH2D* h2, QStringList& comment)
{
  bool ret = true;
  if (strcmp(h1->GetName(), h2->GetName())) {
    comment << QString("Name mismatch: %1 <-> %2").arg(h1->GetName()).arg(h2->GetName());
    ret = false;
  }
  if (h1->GetEntries() != h2->GetEntries()) {
    if (verbose)
      comment << QString("Number of entries mismatch: %1 <-> %2")
        .arg(h1->GetEntries()).arg(h2->GetEntries());
    ret = false;
  }
  int nBinsX = h1->GetXaxis()->GetNbins();
  int nBinsY = h1->GetYaxis()->GetNbins();
  for (int binX = 0; binX <= nBinsX; ++binX)
    for (int binY = 0; binY <= nBinsY; ++binY)
      if (!qFuzzyCompare(h1->GetBinContent(binX, binY), h2->GetBinContent(binX, binY))
        && !(std::isnan(h1->GetBinContent(binX, binY)) && std::isnan(h1->GetBinContent(binX, binY)))) {
        if (verbose)
          comment << QString("bin x %1, bin y %2: first file = %3, second file = %4")
            .arg(binX).arg(binY).arg(h1->GetBinContent(binX, binY)).arg(h2->GetBinContent(binX, binY));
        ret = false;
      }
  return ret;
}

bool functionsMatch(TF1* f1, TF1* f2, QStringList& comment)
{
  bool ret = true;
  if (strcmp(f1->GetName(), f2->GetName())) {
    comment << QString("Name mismatch: %1 <-> %2").arg(f1->GetName()).arg(f2->GetName());
    ret = false;
  }
  if (f1->GetNpar() != f2->GetNpar()) {
    if (verbose)
      comment << QString("Number of parameters mismatch: %1 <-> %2")
        .arg(f1->GetNpar()).arg(f2->GetNpar());
    ret = false;
  }
  int nParamters = f1->GetNpar();
  for (int i = 0; i < nParamters; ++i)
    if (!qFuzzyCompare(f1->GetParameter(i), f2->GetParameter(i))) {
      if (verbose)
        comment << QString("parameter %1 mismatch: %3 <-> %4")
          .arg(i).arg(f1->GetParameter(i)).arg(f2->GetParameter(i));
      ret = false;
    }
  return ret;
}

void save(QString name, const QVector<TObject*>& objects, TFile& outputFile) 
{
  outputFile.cd();
  name.replace("/", "per");
  TCanvas canvas(qPrintable(name), qPrintable(name), 1000, 500);
  canvas.cd();
  canvas.Divide(objects.size());
  for (int i = 0; i < objects.size(); ++i) {
    canvas.cd(i + 1);
    objects[i]->DrawClone();
    TText text(0.5, 1, qPrintable("File: " + QString::number(i)));
    text.SetNDC();
    text.SetTextAlign(23);
    text.DrawClone();
  }
  canvas.cd();
  canvas.Write();
  gROOT->cd();
}

void compareKeys(TKey* key1, TKey* key2, QStringList& comment, TFile& outputFile)
{
  TObject* object1 = key1->ReadObj();
  TObject* object2 = key2->ReadObj();
  Q_ASSERT(!strcmp(object1->ClassName(), "TCanvas") && !strcmp(object2->ClassName(), "TCanvas"));
  PostAnalysisCanvas canvas1(static_cast<TCanvas*>(object1));
  PostAnalysisCanvas canvas2(static_cast<TCanvas*>(object2));

  const QVector<TH1D*>& h1d1 = canvas1.histograms1D();
  const QVector<TH1D*>& h1d2 = canvas2.histograms1D();
  for (int i = 0; i < h1d1.count(); ++i) {
    if (!histogramsMatch(h1d1[i], h1d2[i], comment)) {
      comment << QString("canvases \"%1\", histograms \"%2\" differ.").arg(canvas1.name()).arg(h1d1[i]->GetName());
      save(canvas1.name(), QVector<TObject*>() << h1d1[i] << h1d2[i], outputFile);
    }
  }

  const QVector<TH2D*>& h2d1 = canvas1.histograms2D();
  const QVector<TH2D*>& h2d2 = canvas2.histograms2D();
  for (int i = 0; i < h2d1.count(); ++i) {
    if (!histogramsMatch(h2d1[i], h2d2[i], comment)) {
      comment << QString("canvases \"%1\", histograms \"%2\" differ.").arg(canvas1.name()).arg(h2d1[i]->GetName());
      save(canvas1.name(), QVector<TObject*>() << h2d1[i] << h2d2[i], outputFile);
    }
  }

  const QVector<TF1*>& f1 = canvas1.functions();
  const QVector<TF1*>& f2 = canvas2.functions();
  for (int i = 0; i < f1.count(); ++i) {
    if (!functionsMatch(f1[i], f2[i], comment)) {
      comment << QString("canvases \"%1\", functions \"%2\" differ.").arg(canvas1.name()).arg(f1[i]->GetName());
      save(canvas1.name(), QVector<TObject*>() << f1[i] << f2[i], outputFile);
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    qFatal("Usage: \"fileCompare postAnaFile1.txt postAnaFile2.txt\"");
  if (!QFile::exists(argv[1]) || !QFile::exists(argv[2]))
    qFatal("At least one of the files does not exist.");

  TFile file1(argv[1]);
  gROOT->cd();
  TFile file2(argv[2]);
  gROOT->cd();

  QStringList keys1;
  QStringList keys2;
  for (int i = 0; i < file1.GetListOfKeys()->GetSize(); ++i)
    keys1.append(file1.GetListOfKeys()->At(i)->GetName());
  for (int i = 0; i < file2.GetListOfKeys()->GetSize(); ++i)
    keys2.append(file2.GetListOfKeys()->At(i)->GetName());

  QStringList commonKeys;
  QStringList missingKeys1;
  QStringList missingKeys2;
  compareKeys(keys1, keys2, commonKeys, missingKeys1, missingKeys2);
  qDebug() << missingKeys1.count() << "key(s) missing in file" << argv[1];
  foreach (QString s, missingKeys1)
    qDebug() << s;
  qDebug();
  qDebug() << missingKeys2.count() << "key(s) missing in file" << argv[2];
  foreach (QString s, missingKeys2)
    qDebug() << s;
  qDebug();
  qDebug() << "Comparing" << commonKeys.count() << "common key(s)...";
  int nDifferingKeys = 0;
  TFile outputFile("differences.root", "RECREATE");
  gROOT->cd();
  foreach (QString s, commonKeys) {
    TKey* key1 = file1.GetKey(qPrintable(s));
    TKey* key2 = file2.GetKey(qPrintable(s));
    QStringList comment;
    compareKeys(key1, key2, comment, outputFile);
    if (comment.size())
      ++nDifferingKeys;
    foreach (QString s, comment)
      qDebug() << qPrintable(s);
  }
  outputFile.Close();

  qDebug();
  qDebug() << "summary:";
  qDebug() << missingKeys1.count() << "key(s) missing in file" << argv[1];
  qDebug() << missingKeys2.count() << "key(s) missing in file" << argv[2];
  qDebug() << nDifferingKeys << "out of" << commonKeys.count() << "common key(s) differ(s)";

  file2.Close();
  file1.Close();

  return 1;
}
