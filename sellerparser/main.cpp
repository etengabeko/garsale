#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QStringList>

#include "sellerimporter.h"

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  QString inputFileName;
  QStringList args(app.arguments());
  if (args.contains("-f") == true) {
    int index = args.indexOf("-f");
    if (args.size()-1 >= index+1) {
      inputFileName = args.at(index+1);
    }
  }

  garsale::SellerImporter slr;
  bool ok = false;
  if (inputFileName.isEmpty() == false) {
    ok = slr.importFromFile(inputFileName);
    qDebug() << QString::fromUtf8("Загрузка из файла %1 [%2]").arg(inputFileName).arg((ok ? "True" : "False"));
  }
  else {
    qDebug() << QString::fromUtf8("Ошибка загрузки: не указано имя файла");
  }

  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
