#include <QtWidgets/QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>

#include <sellerparser/sellerimporter.h>
#include <ui/sellerwidget/sellerwidget.h>

namespace {
  const QString noGuiArg() { return "--gui"; }

  const QStringList parseArguments(int argc, char* argv[])
  {
    QStringList result;
    for (int i = 0; i < argc; ++i) {
      result.append(QString(argv[i]));
    }
    return result;
  }
}

using namespace garsale;

int main(int argc, char* argv[])
{
  bool nogui = ::parseArguments(argc, argv).contains(::noGuiArg());
  QCoreApplication* app = 0;
  if (nogui == true) {
    app = new QCoreApplication(argc, argv);
  }
  else {
    app = new QApplication(argc, argv);
  }

  QString inputFileName;
  QStringList args(app->arguments());
  if (args.contains("-f") == true) {
    int index = args.indexOf("-f");
    if (args.size()-1 >= index+1) {
      inputFileName = args.at(index+1);
    }
  }

  int result = -1;
  if (nogui == false) {
    SellerWidget* wgt = new SellerWidget();
    wgt->show();
    result = app->exec();
  }
  else {
    SellerImporter slr;
    bool ok = false;
    if (inputFileName.isEmpty() == false) {
      ok = slr.importFromFile(inputFileName);
      qDebug() << QString::fromUtf8("Загрузка из файла %1 [%2]").arg(inputFileName).arg((ok ? "True" : "False"));
    }
    else {
      qDebug() << QString::fromUtf8("Ошибка загрузки: не указано имя файла");
    }
    result = ok ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  delete app;

  return result;
}
