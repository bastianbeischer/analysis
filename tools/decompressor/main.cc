#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>

#include <TFileMerger.h>

#include <cstdlib>

int main(int argc, char** argv)
{
  QStringList sourceFiles;
  bool overwrite = false;
  for (int i = 1; i < argc; ++i) {
    QString argument = argv[i];
    if (argument.contains("--")) {
      if (argument.contains("overwrite")) {
        overwrite = true;
      } else {
        qDebug() << "Unknown option" << argument;
        return EXIT_FAILURE;
      }
    } else {
      sourceFiles.append(argument);
    }
  }
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
    if (overwrite) {
      qDebug() << "rm" << sourceFileName;
      QFile::remove(sourceFileName);
      qDebug() << "mv" << destinationFileName << "  ---->  " << sourceFileName;
      QFile::rename(destinationFileName, sourceFileName);
    }
  }
  return EXIT_SUCCESS;
}
