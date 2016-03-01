#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>

#include <memory>

#include "sellerparser/sellerimporter.h"
#include "ui/sellerwidget/sellerwidget.h"

namespace {
  const QString noGuiArg() { return "--no-gui"; }

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
  std::unique_ptr<QCoreApplication> app(nullptr);
  if (nogui == true) {
    app.reset(new QCoreApplication(argc, argv));
  }
  else {
    app.reset(new QApplication(argc, argv));
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
    SellerWidget wgt;
    wgt.show();
    result = app->exec();
  }
  else {
    bool ok = false;
    if (inputFileName.isEmpty() == false) {
      ok = SellerImporter::importFromFile(inputFileName);
      qDebug() << QObject::tr(QString("Load from file %1 [%2]")
                              .arg(inputFileName)
                              .arg((ok ? "True" : "False"))
                              .toStdString().c_str());
    }
    else {
      qDebug() << QObject::tr(QString("Error load: empty filename").toStdString().c_str());
    }
    result = ok ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  return result;
}
