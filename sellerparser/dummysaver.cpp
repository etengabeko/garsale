#include "dummysaver.h"

#include "sellergoods.h"

#include <QFile>
#include <QString>
#include <QTextStream>

namespace {
  const QString outputFileName() { return QString("%1.txt"); }
  const QString repeatedOutputFileName() { return QString("%1(%2).txt"); }
}

namespace garsale {

DummySaver::DummySaver() :
  AbstractSaver()
{
}

bool DummySaver::save(const SellerGoods& sgoods)
{
  QFile output(getOutputFileName(sgoods.name));
  if (output.open(QIODevice::WriteOnly) == true) {
    QTextStream stream(&output);
    stream << sgoods;
    return true;
  }
  return false;
}

const QString DummySaver::getOutputFileName(QString sellerName) const
{
  sellerName.replace(" ", "_");
  QString filename(::outputFileName().arg(sellerName));
  int i = 0;
  while (QFile::exists(filename) == true) {
    filename = ::repeatedOutputFileName().arg(sellerName).arg(++i);
  }
  return filename;
}

} // garsale
