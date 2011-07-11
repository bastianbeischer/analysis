#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>

#include <TROOT.h>
#include <TFileMerger.h>
#include <TFile.h>
#include <TList.h>
#include <TObject.h>

int main(int argc, char** argv)
{
  QStringList sourceFiles;
  for (int i = 1; i < argc; ++i)
    sourceFiles.append(argv[i]);
  foreach (QString sourceFileName, sourceFiles) {
    if (!QFile::exists(sourceFileName)) {
      qDebug() << "File" << sourceFileName << "does not exist!";
      continue;
    }
    if (sourceFileName.right(5) != ".root") {
      qDebug() << "File" << sourceFileName << "is not a root file!";
      continue;
    }
    QString destinationFileName = sourceFileName;
    destinationFileName.chop(5);
    destinationFileName.append("Decompressed.root");
    qDebug() << sourceFileName << "  ---->  " << destinationFileName;

    /* For some reason TFileMerger is not compiled into libRIO.so on i7
    TFileMerger merger;
    merger.AddFile(qPrintable(sourceFileName));
    merger.OutputFile(qPrintable(destinationFileName));
    merger.Merge();
    */

    TFile inputFile(qPrintable(sourceFileName), "READ", "inputFile");
    gROOT->cd();
    inputFile.GetListOfKeys();
    TIter next(inputFile.GetListOfKeys());
    TFile outputFile(qPrintable(destinationFileName), "RECREATE", "outputFile", 0);
    TObject* key;
    while ((key = next())) {
      key->Write();
    }
    outputFile.Close();
    inputFile.Close();
  }
  return 0;
}
