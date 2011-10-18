#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>

#include <TFileMerger.h>

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

    TFileMerger merger;
    merger.OutputFile(qPrintable(destinationFileName), true, 0);
    merger.AddFile(qPrintable(sourceFileName));
    merger.Merge();
  }
  return 0;
}
